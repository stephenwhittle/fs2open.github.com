#include "cfile/SCPCFileModule.h"
#include "SCPApplication.h"
#include "cfile/cfile.h"
#include "FSOutputDeviceBase.h"

#include "module/SCPModuleManager.h"
#include "filesystem/SCPFilesystemModule.h"
#include "filesystem/SCPDirectoryIterator.h"
#include "FSIntegerTypes.h"
#include <utility>
#include "cfile/cfilesystem.h"
#include "cfile/SCPCFile.h"



int SCPCFileModule::GetNextEmptyBlockIndex() 
{
	int i;
	CFILE* cfile;
	/*

	for (i = 0; i < SCPCFileModule::MAX_CFILE_BLOCKS; i++) {
		cfile = &Cfile_block_list[i];
		if (cfile->type == CFILE_BLOCK_UNUSED) {
			cfile->data = nullptr;
			cfile->fp   = nullptr;
			cfile->type = CFILE_BLOCK_USED;
			return i;
		}
	}*/

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


	BuildCFileDatabase(nullptr);
	
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
	/*for (auto& CFileBlock : Cfile_block_list)
	{
		if (CFileBlock.type != CFILE_BLOCK_UNUSED)
		{
			mprintf(("    %s:%d\n", CFileBlock.source_file, CFileBlock.line_num));
		}
	}*/
}

uint32_t SCPCFileModule::AddRoot(SCPRootInfo Root) 
{
	return CFileDatabase().AddRoot(Root);
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
	tl::optional<SCPRootInfo> Root = CFileDatabase().GetRootByID(RootID);
	Assert(Root.has_value());

	for (auto Pair : PathTypes) {
		SCPCFilePathType PathInfo = Pair.second;
		for (const auto& DirectoryEntry : ghc::filesystem::directory_iterator(Root->GetPath() / PathInfo.Path)) {
			if (SCPPath::Compare(DirectoryEntry.path().extension(), ".vp")) 
			{
				SCPRootInfo NewRoot =
					SCPRootInfo(DirectoryEntry.path(), SCPRootInfo::RootType::PackFile, Root->GetLocationFlags());
				CFileDatabase().AddRoot(NewRoot);
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
									 RootPath.c_str(),
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

void SCPCFileModule::BuildRootList(const char* cdrom_dir)
{
	auto CmdLineModule = SCPModuleManager::GetModule<SCPCmdlineModule>();


	SCPRootInfo* root = nullptr;

	if (SCPApplication::Get().GetLegacyMode())
	{
		// =========================================================================
#ifdef WIN32
		// Nothing to do here, Windows uses the current directory as the base
#else
		AddModRoots(os_get_legacy_user_dir(), SCPCFileLocation::UserDirectory);


		SCPCFileLocationFlags LegacyDirFlags = SCPCFileLocationFlags({ SCPCFileLocation::UserDirectory , SCPCFileLocation::TopLevelDirectory });
		if (!(CmdLineModule && CmdLineModule->CurrentOptions->ModList.has_value())) {
			LegacyDirFlags.SetFlag(SCPCFileLocation::PrimaryMod);
		}
		SCPRootInfo LegacyRoot = SCPRootInfo(os_get_legacy_user_dir(), SCPRootInfo::RootType::Path, LegacyDirFlags);
		
		uint32_t LegacyRootID = AddRoot(LegacyRoot);
		BuildPackListForRoot(LegacyRootID);

#endif
		// =========================================================================
	}
	else if (!(CmdLineModule && CmdLineModule->CurrentOptions->bEnablePortableMode.has_value() && *CmdLineModule->CurrentOptions->bEnablePortableMode == true))
	{
		// =========================================================================
		// now look for mods under the users HOME directory to use before system ones
		AddModRoots(Cfile_user_dir, SCPCFileLocation::UserDirectory);
		// =========================================================================

		// =========================================================================
		// set users HOME directory as default for loading and saving files
		SCPCFileLocationFlags HomeDirFlags = SCPCFileLocationFlags({ SCPCFileLocation::UserDirectory , SCPCFileLocation::TopLevelDirectory });
		if (!(CmdLineModule && CmdLineModule->CurrentOptions->ModList.has_value())) {
			HomeDirFlags.SetFlag(SCPCFileLocation::PrimaryMod);
		}
		SCPRootInfo HomeDirRoot = SCPRootInfo(Cfile_user_dir, SCPRootInfo::RootType::Path, HomeDirFlags);
		
		uint32_t HomeDirRootID = AddRoot(HomeDirRoot);
		BuildPackListForRoot(HomeDirRootID);
		// =========================================================================
	}

	//======================================================
	// Next, check any VP files under the current directory.
	auto FSModule = SCPModuleManager::GetModule<SCPFilesystemModule>();
	if (FSModule)
	{
		SCPRootInfo WorkingDirectoryRoot = 
			SCPRootInfo(FSModule->WorkingDirectory.GetCurrent(), SCPRootInfo::RootType::Path, { SCPCFileLocation::GameRootDirectory, SCPCFileLocation::TopLevelDirectory });
		uint32_t WorkingDirectoryRootID = AddRoot(WorkingDirectoryRoot);

		AddModRoots(FSModule->WorkingDirectory.GetCurrent().c_str(), SCPCFileLocation::GameRootDirectory);
		BuildPackListForRoot(WorkingDirectoryRootID);

	}
	else
	{
		GOutputDevice->Error(LOCATION, "Can't get filesystem module to determine working directory");
	}

	//======================================================
	 // Check the real CD if one...
	if (cdrom_dir && (strlen(cdrom_dir) < CF_MAX_PATHNAME_LENGTH)) {
		//======================================================
		// Next, check any VP files in the CD-ROM directory.
		SCPRootInfo CDRoot = SCPRootInfo(cdrom_dir, SCPRootInfo::RootType::Path, SCPCFileLocationFlags::Empty());
		uint32_t CDRootID = AddRoot(CDRoot);
		BuildPackListForRoot(CDRootID);

	}

	// The final root is the in-memory root
	SCPRootInfo MemoryRoot = SCPRootInfo("", SCPRootInfo::RootType::InMemory, SCPCFileLocation::MemoryRoot | SCPCFileLocation::TopLevelDirectory);
	AddRoot(MemoryRoot);

}


void SCPCFileModule::BuildCFileDatabase(const char* cdrom_dir)
{

	mprintf(("Building file index...\n"));

	// build the list of searchable roots
	BuildRootList(cdrom_dir);

	// build the list of files themselves
	BuildFileList();

	//query database for all roots and all files
	mprintf(("Found %d roots and %d files.\n", Num_roots, Num_files));
}

void SCPCFileModule::BuildFileList()
{
	
	//query database for all roots
	//then switch on root type
	for (SCPRootInfo& CurrentRoot : CFileDatabase().AllRoots())
	{
		switch (CurrentRoot.GetType())
		{
		case SCPRootInfo::RootType::Path:
			cf_search_root_path(CurrentRoot.GetUID());
			break;
		case SCPRootInfo::RootType::PackFile:
			cf_search_root_pack(CurrentRoot.GetUID());
			break;
		case SCPRootInfo::RootType::InMemory:
			cf_search_memory_root(CurrentRoot.GetUID());
			break;
		}
	}
	
}
void SCPCFileModule::AddFilesFromRoot(SCPRootInfo Root)
{
	for (auto Pair : PathTypes) {
		if (Pair.first == SCPCFilePathTypeID::SinglePlayers || Pair.first == SCPCFilePathTypeID::MultiPlayers)
		{
			continue;
		}
		SCPPath FullDirectoryPath = Root.GetPath() / Pair.second.Path;
		SCPDirectoryIterator DirectoryIterator = SCPDirectoryIterator(FullDirectoryPath, Pair.second.Extensions);

		for (SCPPath FilePath : DirectoryIterator)
		{
			SCPCFileInfo FileInfo = SCPCFileInfo(FilePath, Root.GetUID(), Pair.first);
			CFileDatabase().AddFile(FileInfo);
		}
	}
	
}


int SCPCFileModule::GetDefaultFilePath(char* path, uint path_max, int pathtype, const char* filename, bool localize,
	uint32_t location_flags, SCP_string LanguagePrefix)
{
	if (SCPPath(filename).has_parent_path())
	{
		// Already has full path
		strncpy(path, filename, path_max);

	}
	else {

		for (SCPRootInfo Root : CFileDatabase().AllRootsOfType(SCPRootInfo::RootType::Path))
		{
			if (cf_check_location_flags(current_root->location_flags, location_flags)) {
				// We found a valid root
				Assert(CF_TYPE_SPECIFIED(pathtype));
				strncpy(path, root->path, path_max);
				if (filename) {
					strcat_s(path, path_max, filename);

					// localize filename
					if (localize) {

						SCPPath LocalizedPath = SCPPath(path);
						LocalizedPath = LocalizedPath.remove_filename() / LanguagePrefix / LocalizedPath.filename();
						strncpy(path, LocalizedPath.c_str(), path_max);
						// verify localized path
						FILE* fp = fopen(path, "rb");
						if (fp) {
							fclose(fp);
							return 1;
						}
					}
				}
				break;
			}
			auto RootFlags = Root.GetLocationFlags();
			if (RootFlags.HasFlag())
			{

			}
		}
		/*

		SCPRootInfo* root = nullptr;
		//Select * from roots where type == CF_ROOTTYPE_PATH
		for (auto i = 0; i < Num_roots; ++i) {
			auto current_root = cf_get_root(i);

			if (current_root->roottype != CF_ROOTTYPE_PATH) {
				// We want a "real" path here so only path roots are valid
				continue;
			}

			if (cf_check_location_flags(current_root->location_flags, location_flags)) {
				// We found a valid root
				root = current_root;
				break;
			}
		}

		if (!root) {
			Assert(filename != NULL);
			strncpy(path, filename, path_max);
			return 1;
		}

		Assert(CF_TYPE_SPECIFIED(pathtype));

		strncpy(path, root->path, path_max);

		strcat_s(path, path_max, Pathtypes[pathtype].path);

		// Don't add slash for root directory
		if (Pathtypes[pathtype].path[0] != '\0') {
			if (path[strlen(path) - 1] != DIR_SEPARATOR_CHAR) {
				strcat_s(path, path_max, DIR_SEPARATOR_STR);
			}
		}

		// add filename
		if (filename) {
			strcat_s(path, path_max, filename);

			// localize filename
			if (localize) {

				SCPPath LocalizedPath = SCPPath(path);
				LocalizedPath = LocalizedPath.remove_filename() / LanguagePrefix / LocalizedPath.filename();
				strncpy(path, LocalizedPath.c_str(), path_max);
				// verify localized path
				FILE* fp = fopen(path, "rb");
				if (fp) {
					fclose(fp);
					return 1;
				}
			}
		}
		*/
	}

	return 1;
}