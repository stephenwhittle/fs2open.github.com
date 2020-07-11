#include "cfile/SCPCFileModule.h"
#include "SCPApplication.h"
#include "cfile/cfile.h"
#include "FSOutputDeviceBase.h"

#include "module/SCPModuleManager.h"
#include "filesystem/SCPFilesystemModule.h"
#include "FSIntegerTypes.h"
#include <utility>
#include "cfile/cfilesystem.h"
#include "cfile/SCPCFile.h"



int SCPCFileModule::GetNextEmptyBlockIndex() 
{
	int i;
	CFILE* cfile;

	for (i = 0; i < SCPCFileModule::MAX_CFILE_BLOCKS; i++) {
		cfile = &Cfile_block_list[i];
		if (cfile->type == CFILE_BLOCK_UNUSED) {
			cfile->data = nullptr;
			cfile->fp   = nullptr;
			cfile->type = CFILE_BLOCK_USED;
			return i;
		}
	}

	// If we've reached this point, a free Cfile_block could not be found
	nprintf(("Warning", "A free Cfile_block could not be found.\n"));

	// Dump a list of all opened files
	mprintf(("Out of cfile blocks! Currently opened files:\n"));
	DumpOpenedFileList();

	UNREACHABLE(
		"There are no more free cfile blocks. This means that there are too many files opened by FSO.\n"
		"This is probably caused by a programming or scripting error where a file does not get closed."); // out of free
																										  // cfile
																										  // blocks
	return -1;
}

//todo: @SCPCFileModule ascertain if we need to hold onto the pointer for a Cfile or not
//destructor can call cfclose if need be
tl::optional<CFILE&> SCPCFileModule::GetNextEmptyBlock() 
{
	/*for (CFILE& CurrentFile : Cfile_block_list)
	{
		if (CurrentFile.type == CFILE_BLOCK_UNUSED)
		{
			CurrentFile.data = nullptr;
			CurrentFile.fp = nullptr;
			CurrentFile.type = CFILE_BLOCK_UNUSED;
			return CurrentFile;
		}
	}*/
	nprintf(("Warning", "A free Cfile_block could not be found.\n"));

	// Dump a list of all opened files
	mprintf(("Out of cfile blocks! Currently opened files:\n"));
	DumpOpenedFileList();

	UNREACHABLE(
		"There are no more free cfile blocks. This means that there are too many files opened by FSO.\n"
		"This is probably caused by a programming or scripting error where a file does not get closed."); // out of free
																										  // cfile
																										  // blocks
	return {};
}

bool SCPCFileModule::StartupModule() 
{
	static bool InitializationGuard = false;
	if (InitializationGuard)
	{
		return false;
	}


	cf_build_secondary_filelist(nullptr);
	
	InitializationGuard = true;
	return true;
}

void SCPCFileModule::ShutdownModule() 
{
	mprintf(("Still opened files:\n"));
	DumpOpenedFileList();

	//cf_free_secondary_filelist();

	//cfile_inited = 0;
}

void SCPCFileModule::DumpOpenedFileList() 
{
	for (auto& CFileBlock : Cfile_block_list)
	{
		if (CFileBlock.type != CFILE_BLOCK_UNUSED)
		{
			mprintf(("    %s:%d\n", CFileBlock.source_file, CFileBlock.line_num));
		}
	}
}

uint32_t SCPCFileModule::AddRoot(SCPRootInfo Root) 
{
	return CFileDatabase().insert(Root);
}

///this will have to be exposed as a view on the table?
/*
bool cf_packfile_sort_func(const cf_root_sort &r1, const cf_root_sort &r2)
{
	// if the 2 directory types are the same, do a string compare
	if (r1.cf_type == r2.cf_type) {
		return (stricmp(r1.path, r2.path) < 0);
	}

	// otherwise return them in order of CF_TYPE_* precedence
	return (r1.cf_type < r2.cf_type);
}
*/


//font manager is the only thing that ever calls localize - true
//could probably expose something on the localization service that localizes the file path passed in actually

CFILE* SCPCFileModule::CFOpenFileFillBlock(const char* source, int line, FILE* fp, int type)
{

	tl::optional<CFILE&> File = GetNextEmptyBlock();
	if (!File) {
		fclose(fp);
		return nullptr;
	} else {
		File->data         = nullptr;
		File->mem_mapped   = false;
		File->fp           = fp;
		File->dir_type     = type;
		File->max_read_len = 0;

		File->source_file = source;
		File->line_num    = line;

		int pos = ftell(fp);
		if (pos == -1L)
			pos = 0;
		cf_init_lowlevel_read_code(&File.value(), 0, filelength(fileno(fp)), 0);

		return &File.value();
	}
}

CFILE*
SCPCFileModule::CFOpenInMemoryFileFillBlock(const char* source, int line, const void* data, size_t size, int dir_type)
{
	int cfile_block_index;

	cfile_block_index = GetNextEmptyBlockIndex();
	if (cfile_block_index == -1) {
		return NULL;
	} else {
		CFILE* cfp = &Cfile_block_list[cfile_block_index];

		cfp->max_read_len = 0;
		cfp->fp           = nullptr;
		cfp->mem_mapped   = false;
		cfp->dir_type     = dir_type;

		cfp->source_file = source;
		cfp->line_num    = line;

		cf_init_lowlevel_read_code(cfp, 0, size, 0);
		cfp->data = data;

		return cfp;
	}
}


void SCPCFileModule::BuildPackListForRoot(uint32_t RootID)
{
	std::unique_ptr<SCPRootInfo> Root = CFileDatabase().get_pointer<SCPRootInfo>(RootID);

	for (auto Pair : PathTypes) {
		SCPCFilePathType PathInfo = Pair.second;
		for (const auto& DirectoryEntry : ghc::filesystem::directory_iterator(Root->Path / PathInfo.Path)) {
			if (DirectoryEntry.path().extension() == ".vp") {
				SCPRootInfo NewRoot =
					SCPRootInfo(DirectoryEntry.path(), SCPRootInfo::RootType::PackFile, Root->location_flags);
				CFileDatabase().insert<SCPRootInfo>(NewRoot);
			}
		}
	}
}

void SCPCFileModule::AddModRoots(const char* rootDirectory, SCPCFileLocationFlags basic_location)
{
	auto CmdlineModule = SCPModuleManager::GetModule<SCPCmdlineModule>();
	auto& ModList      = CmdlineModule->CurrentOptions->ModList;
	if (ModList) {
		bool primary = true;
		std::replace(ModList->begin(), ModList->end(), ',', '\0');
		ModList->push_back('\0');
		for (const char* cur_pos = ModList->c_str(); strlen(cur_pos) != 0; cur_pos += (strlen(cur_pos) + 1))
		// for (const char* cur_pos=Cmdline_mod; strlen(cur_pos) != 0; cur_pos+= (strlen(cur_pos)+1))
		{

			SCPPath RootPath = SCPPath(rootDirectory) / cur_pos;

			if (RootPath.string().size() + 1 >= CF_MAX_PATHNAME_LENGTH) {
				GOutputDevice->Error(LOCATION,
									 "The length of mod directory path '%s' exceeds the maximum of %d!\n",
									 rootPath.c_str(),
									 CF_MAX_PATHNAME_LENGTH);
			}

			SCPRootInfo root(RootPath,
							 SCPRootInfo::RootType::Path,
							 basic_location |
								 (primary ? SCPCFileLocation::PrimaryMod : SCPCFileLocation::SecondaryMods));
			auto CFileModule = SCPModuleManager::GetModule<SCPCFileModule>();
			if (!CFileModule.has_value()) {
				// throw
			}
			auto RootID = CFileModule->AddRoot(root);
			// add the root to the database and then pass the ID to the next function?
			CFileModule->BuildPackListForRoot(RootID);

			primary = false;
		}
	}
}