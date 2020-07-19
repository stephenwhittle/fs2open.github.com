#include "cfile/SCPCFileModule.h"
#include "cfile/cfile.h"
#include "cfile/cfilesystem.h"
#include "cfile/SCPCFile.h"
#include "def_files/def_files.h"
#include "filesystem/SCPFilesystemModule.h"
#include "filesystem/SCPDirectoryIterator.h"
#include "module/SCPModuleManager.h"
#include "SCPEndian.h"
#include "FSIntegerTypes.h"
#include "FSAssert.h"
#include "FSOutputDeviceBase.h"
#include "SCPApplication.h"
#include <utility>


typedef struct VP_FILE_HEADER {
	char id[4];
	int version;
	int index_offset;
	int num_files;
} VP_FILE_HEADER;

typedef struct VP_FILE {
	int offset;
	int size;
	char filename[32];
	_fs_time_t write_time;
} VP_FILE;


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
			PopulateLooseFilesInRoot(CurrentRoot.GetUID());
			break;
		case SCPRootInfo::RootType::PackFile:
			PopulateFilesInPackFile(CurrentRoot.GetUID());
			break;
		case SCPRootInfo::RootType::InMemory:
			PopulateFilesInMemoryRoot(CurrentRoot.GetUID());
			break;
		}
	}
	
}

void SCPCFileModule::PopulateFilesInMemoryRoot(uint32_t RootID) {
	int num_files = 0;
	mprintf(("Searching memory root ... "));

	auto default_files = defaults_get_all();
	for (auto& default_file : default_files) {
		// Pure built in files have an empty path_type string
		if (strlen(default_file.path_type) <= 0) {
			// Ignore pure builtin files. They should only be accessible with defaults_get_file
			continue;
		}

		SCPCFilePathTypeID pathtype = GetPathTypeID(default_file);

		Assertion(pathtype != SCPCFilePathTypeID::Invalid, "Default file '%s' does not use a valid path type!",
			default_file.filename);

		SCPCFileInfo NewFile(default_file.filename, RootID, pathtype, default_file.size, default_file.data);
		CFileDatabase().AddFile(NewFile);
		num_files++;
	}

	mprintf(("%i files\n", num_files));
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

void SCPCFileModule::PopulateFilesInPackFile(uint32_t RootID)
{
	int num_files = 0;
	tl::optional<SCPRootInfo> root = CFileDatabase().GetRootByID(RootID);

	Assert(root);

	// Open data		
	FILE* fp = fopen(root->GetPath().c_str(), "rb");
	// Read the file header
	if (!fp) {
		return;
	}

	if (filelength(fileno(fp)) < (int)(sizeof(VP_FILE_HEADER) + (sizeof(int) * 3))) {
		mprintf(("Skipping VP file ('%s') of invalid size...\n", root->GetPath()));
		fclose(fp);
		return;
	}

	VP_FILE_HEADER VP_header;

	Assert(sizeof(VP_header) == 16);
	if (fread(&VP_header, sizeof(VP_header), 1, fp) != 1) {
		mprintf(("Skipping VP file ('%s') because the header could not be read...\n", root->GetPath()));
		fclose(fp);
		return;
	}

	VP_header.version = INTEL_INT(VP_header.version); //-V570
	VP_header.index_offset = INTEL_INT(VP_header.index_offset); //-V570
	VP_header.num_files = INTEL_INT(VP_header.num_files); //-V570

	mprintf(("Searching root pack '%s' ... ", root->GetPath()));

	// Read index info
	fseek(fp, VP_header.index_offset, SEEK_SET);

	// Go through all the files
	SCPPath CurrentFolder;
	int i;
	for (i = 0; i < VP_header.num_files; i++) {
		VP_FILE find;

		if (fread(&find, sizeof(VP_FILE), 1, fp) != 1) {
			mprintf(("Failed to read file entry (currently in directory %s)!\n", CurrentFolder));
			break;
		}
		//if the file size is 0 then we have a directory
		//each directory pushes onto the stack, can use normalize to go up directories if there's a ..
		//if its a file, then check the extension
		//if the extension matches, store it into the database

		find.offset = INTEL_INT(find.offset); //-V570
		find.size = INTEL_INT(find.size); //-V570
		find.write_time = INTEL_INT(find.write_time); //-V570
		find.filename[sizeof(find.filename) - 1] = '\0';

		if (find.size == 0) {
			//append the next 
			CurrentFolder /= find.filename;
			CurrentFolder /= ""; //add empty filename to ensure this is a directory
			CurrentFolder = CurrentFolder.lexically_normal(); //resolve . and ..

			//mprintf(( "Current dir = '%s'\n", search_path ));
		}
		else {
			for (auto PathTypePair : PathTypes)
			{
				SCPCFilePathType& PathTypeInfo = PathTypePair.second;

				if (SCPPath::Compare(CurrentFolder, PathTypeInfo.Path))
				{
					if (PathTypeInfo.Extensions.find(SCPPath(find.filename).extension()) != PathTypeInfo.Extensions.end())
					{
						SCPCFileInfo PackedFile = SCPCFileInfo(find.filename, RootID, PathTypeInfo.Type, find.write_time, find.size, find.offset);
						CFileDatabase().AddFile(PackedFile);
						num_files++;
					}
				}
			}
		}
	}

	fclose(fp);

	mprintf(("%i files\n", num_files));
}

void SCPCFileModule::PopulateLooseFilesInRoot(uint32_t RootID)
{
	int i;
	int num_files = 0;

	tl::optional<SCPRootInfo> root = CFileDatabase().GetRootByID(RootID);
	Assert(root);
	mprintf(("Searching root '%s' ... ", root->GetPath()));

#ifndef WIN32
	try {
		auto current = root->path;
		const auto prefPathEnd = root->path + strlen(root->path);
		while (current != prefPathEnd) {
			const auto cp = utf8::next(current, prefPathEnd);
			if (cp > 127) {
				// On Windows, we currently do not support Unicode paths so catch this early and let the user
				// know
				const auto invalid_end = current;
				utf8::prior(current, root->path);
				Error(LOCATION,
					"Trying to use path \"%s\" as a data root. That path is not supported since it "
					"contains a Unicode character (%s). If possible, change this path to something that only uses "
					"ASCII characters.",
					root->path, std::string(current, invalid_end).c_str());
			}
		}
	}
	catch (const std::exception& e) {
		Error(LOCATION, "UTF-8 error while checking the root path \"%s\": %s", root->path, e.what());
	}
#endif

	char search_path[CF_MAX_PATHNAME_LENGTH];
#ifdef SCP_UNIX
	// This map stores the mapping between a specific path type and the actual path that we use for it
	SCP_unordered_map<int, SCP_string> pathTypeToRealPath;
#endif

	for (auto Pair : PathTypes)
	{
		if (Pair.first == SCPCFilePathTypeID::SinglePlayers || Pair.first == SCPCFilePathTypeID::MultiPlayers)
		{
			continue;
		}
		SCPPath FullPath = root->GetPath() / Pair.second.Path;
		SCPDirectoryIterator DirIterator(FullPath, Pair.second.Extensions, SCPDirectoryIterator::Options{ SCPDirectoryIterator::Flags::Recursive });
		for (auto File : DirIterator)
		{
			SCPCFileInfo FileInfo(File, RootID, Pair.first);
			CFileDatabase().AddFile(FileInfo);
			num_files++;
		}
		//iterate using directory iterator
		//then check extensions of all files against the Pair.second.Extensions vector
		//if theres a match
		//construct SCPCFileInfo
		//add to database
	}

	mprintf(("%i files\n", num_files));
}
