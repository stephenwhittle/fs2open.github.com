#include "cfile/SCPCFileModule.h"
#include "cfile/cfilesystem.h"
#include "cfile/SCPCFile.h"
#include "cfile/SCPRootInfo.h"
#include "cfile/SCPCFileInfo.h"
#include "def_files/def_files.h"
#include "filesystem/SCPFilesystemModule.h"
#include "filesystem/SCPDirectoryIterator.h"
#include "filesystem/SCPFile.h"
#include "module/SCPModuleManager.h"
#include "SCPCFilePathType.h"
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


void SCPCFileModule::BuildPackListForRoot(uint32_t RootID)
{
	tl::optional<SCPRootInfo> Root = CFileDatabase().GetRootByID(RootID);
	Assert(Root.has_value());

	for (auto Pair : PathTypes) {
		SCPCFilePathType PathInfo = Pair.second;
		if (ghc::filesystem::exists(Root->GetPath() / PathInfo.Path)) {
			for (const auto& DirectoryEntry : ghc::filesystem::directory_iterator(Root->GetPath() / PathInfo.Path)) {
				if (SCPPath::Compare(DirectoryEntry.path().extension(), ".vp")) {
					SCPRootInfo NewRoot =
						SCPRootInfo(DirectoryEntry.path(), SCPRootType::PackFile, Root->GetLocationFlags());
					CFileDatabase().AddRoot(NewRoot);
				}
			}
		}
	}
}

void SCPCFileModule::AddModRoots(const char* Directory, SCPCFileLocationFlags basic_location)
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

			SCPPath RootPath = SCPPath(Directory) / cur_pos;

		/*	if (RootPath.string().size() + 1 >= CF_MAX_PATHNAME_LENGTH) {
				GOutputDevice->Error(LOCATION,
									 "The length of mod directory path '%s' exceeds the maximum of %d!\n",
									 RootPath.c_str(),
									 CF_MAX_PATHNAME_LENGTH);
			}
*/

			SCPRootInfo root(RootPath,
							 SCPRootType::Path,
							 basic_location |
								 (primary ? SCPCFileLocation::PrimaryMod : SCPCFileLocation::SecondaryMods));
			auto RootID = AddRoot(root);
			// add the root to the database and then pass the ID to the next function?
			BuildPackListForRoot(RootID);

			primary = false;
		}
	}
}

void SCPCFileModule::AddModRoots(const SCPPath Directory, SCPCFileLocationFlags BasicLocation) 
{
	AddModRoots(Directory.c_str(), BasicLocation);
}

void SCPCFileModule::BuildRootList(const char* cdrom_dir)
{
	auto CmdLineModule = SCPModuleManager::GetModule<SCPCmdlineModule>();

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
		AddModRoots(UserDirectory, SCPCFileLocation::UserDirectory);
		// =========================================================================

		// =========================================================================
		// set users HOME directory as default for loading and saving files
		SCPCFileLocationFlags HomeDirFlags = SCPCFileLocationFlags({ SCPCFileLocation::UserDirectory , SCPCFileLocation::TopLevelDirectory });
		if (!(CmdLineModule && CmdLineModule->CurrentOptions->ModList.has_value())) {
			HomeDirFlags.SetFlag(SCPCFileLocation::PrimaryMod);
		}
		SCPRootInfo HomeDirRoot = SCPRootInfo(UserDirectory, SCPRootType::Path, HomeDirFlags);
		
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
			SCPRootInfo(FSModule->WorkingDirectory.GetCurrent(), SCPRootType::Path, { SCPCFileLocation::GameRootDirectory, SCPCFileLocation::TopLevelDirectory });
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
	if (cdrom_dir) {
		//======================================================
		// Next, check any VP files in the CD-ROM directory.
		SCPRootInfo CDRoot = SCPRootInfo(cdrom_dir, SCPRootType::Path, SCPCFileLocationFlags::Empty());
		uint32_t CDRootID = AddRoot(CDRoot);
		BuildPackListForRoot(CDRootID);

	}

	// The final root is the in-memory root
	SCPRootInfo MemoryRoot = SCPRootInfo("", SCPRootType::InMemory, { SCPCFileLocation::MemoryRoot , SCPCFileLocation::TopLevelDirectory });
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
	
}

void SCPCFileModule::BuildFileList()
{
	
	//query database for all roots
	//then switch on root type
	RootFilter Filter;
	for (SCPRootInfo CurrentRoot : CFileDatabase().Roots(Filter))
	{
		switch (CurrentRoot.GetType())
		{
		case SCPRootType::Path:
			PopulateLooseFilesInRoot(CurrentRoot.GetUID());
			break;
		case SCPRootType::PackFile:
			PopulateFilesInPackFile(CurrentRoot.GetUID());
			break;
		case SCPRootType::InMemory:
			PopulateFilesInMemoryRoot(CurrentRoot.GetUID());
			break;
		}
	}
	
}

SCPCFilePathTypeID GetPathTypeID(default_file DefaultFile)
{
	SCPPath DefFilePath(DefaultFile.path_type);
	DefFilePath /= "";
	for (auto PathInfoPair : PathTypes) {
		if (SCPPath::Compare(PathInfoPair.second.Path, DefFilePath)) {
			return PathInfoPair.first;
		}
	}
	return SCPCFilePathTypeID::Invalid;
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
		SCPFilesystemView DirectoryIterator = SCPFilesystemView(FullDirectoryPath, Pair.second.Extensions, { SCPFilesystemView::Flags::IncludeFiles, SCPFilesystemView::Flags::Recursive });

		for (SCPPath FilePath : DirectoryIterator)
		{
			SCPCFileInfo FileInfo = SCPCFileInfo(FilePath, Root.GetUID(), Pair.first);
			CFileDatabase().AddFile(FileInfo);
		}
	}
	
}


int SCPCFileModule::GetDefaultFilePath(char* path, uint path_max, int pathtype, const char* filename, bool localize,
	SCPCFileLocationFlags location_flags, SCP_string LanguagePrefix)
{
	if (SCPPath(filename).has_parent_path())
	{
		// Already has full path
		strncpy(path, filename, path_max);
	}
	else {

		for (SCPRootInfo Root : CFileDatabase().AllRootsOfType(SCPRootType::Path))
		{
			if (CheckLocationFlags(Root.GetLocationFlags(), location_flags)) {
				// We found a valid root
				SCPPath RawPath = Root.GetPath();
				if (filename) {
					RawPath /= filename;
					// localize filename
					if (localize) {

						SCPPath LocalizedPath = RawPath;
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
				strncpy(path, RawPath.c_str(), path_max);
				//don't we have to return here if theres no filename but a root was valid?
				break;
			}
		}
	}

	return 1;
}

SCP_string SCPCFileModule::GetDefaultFilePath( SCPCFilePathTypeID PathType, SCP_string Filename,
									   bool Localize /*= false*/,
									   SCPCFileLocationFlags LocationFlags /*= SCPCFileLocationALL*/,
									   SCP_string LanguagePrefix /*= ""*/)
{
	if (SCPPath(Filename).has_parent_path())
	{
		// Already has full path
		return Filename;
	}
	else {
		RootFilter Filter;
		Filter.TypeIs(SCPRootType::Path).LocationMatches(LocationFlags);

		for (SCPRootInfo Root : CFileDatabase().Roots(Filter))
		{
			if (CheckLocationFlags(Root.GetLocationFlags(), LocationFlags)) {
				// We found a valid root
				SCPPath RawPath = Root.GetPath();
				if (!Filename.empty()) {
					RawPath /= Filename;
					// localize filename
					if (Localize) {

						SCPPath LocalizedPath = RawPath;
						LocalizedPath = LocalizedPath.remove_filename() / LanguagePrefix / LocalizedPath.filename();
						return LocalizedPath;
					}
				}
				return RawPath;
				//don't we have to return here if theres no filename but a root was valid?
				break;
			}
		}
	}

	return "";
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
	SCPFile PackFile(root->GetPath());

	if (PackFile.FileSize() < (int)(sizeof(VP_FILE_HEADER) + (sizeof(int) * 3))) {
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
	int num_files = 0;

	tl::optional<SCPRootInfo> root = CFileDatabase().GetRootByID(RootID);
	Assert(root);
	mprintf(("Searching root '%s' ... ", root->GetPath()));

/*

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
*/


	for (auto Pair : PathTypes)
	{
		if (Pair.first == SCPCFilePathTypeID::Invalid || Pair.first == SCPCFilePathTypeID::SinglePlayers || Pair.first == SCPCFilePathTypeID::MultiPlayers)
		{
			continue;
		}
		SCPPath FullPath = root->GetPath() / Pair.second.Path;
		SCPFilesystemView DirIterator(FullPath, Pair.second.Extensions, SCPFilesystemView::Options{ SCPFilesystemView::Flags::IncludeFiles, SCPFilesystemView::Flags::Recursive });
		for (auto File : DirIterator)
		{
			if (File.has_filename()) 
			{
				SCPCFileInfo FileInfo(File, RootID, Pair.first);
				CFileDatabase().AddFile(FileInfo);
				num_files++;
			}
		}
	}

	mprintf(("%i files\n", num_files));
}


bool SCPCFileModule::CheckLocationFlags(const SCPCFileLocationFlags FlagsToCheck, const SCPCFileLocationFlags DesiredFlags) 
{
	bool RootOK = false;
	if (DesiredFlags.HasFlag(SCPCFileLocation::GameRootDirectory) ||
		DesiredFlags.HasFlag(SCPCFileLocation::UserDirectory) || 
		DesiredFlags.HasFlag(SCPCFileLocation::MemoryRoot)) 
	{
		RootOK =
			(DesiredFlags.HasFlag(SCPCFileLocation::GameRootDirectory) && FlagsToCheck.HasFlag(SCPCFileLocation::GameRootDirectory)) ||
			(DesiredFlags.HasFlag(SCPCFileLocation::UserDirectory) && FlagsToCheck.HasFlag(SCPCFileLocation::UserDirectory)) ||
			(DesiredFlags.HasFlag(SCPCFileLocation::MemoryRoot) && FlagsToCheck.HasFlag(SCPCFileLocation::MemoryRoot));
	} else 
	{
		RootOK = true;
	}

	bool ModOK = false;
	if (DesiredFlags.HasFlag(SCPCFileLocation::TopLevelDirectory) ||
		DesiredFlags.HasFlag(SCPCFileLocation::PrimaryMod) || 
		DesiredFlags.HasFlag(SCPCFileLocation::SecondaryMods)) 
	{
		ModOK = (DesiredFlags.HasFlag(SCPCFileLocation::TopLevelDirectory) && FlagsToCheck.HasFlag(SCPCFileLocation::TopLevelDirectory)) ||
				(DesiredFlags.HasFlag(SCPCFileLocation::PrimaryMod) && FlagsToCheck.HasFlag(SCPCFileLocation::PrimaryMod)) ||
				(DesiredFlags.HasFlag(SCPCFileLocation::SecondaryMods) && FlagsToCheck.HasFlag(SCPCFileLocation::SecondaryMods));
	} else 
	{
		ModOK = true;
	}
	return RootOK && ModOK;
}

std::unique_ptr<CFILE> SCPCFileModule::CFileOpen(const class SCPCFileInfo FileInfo, SCPCFileModeFlags Mode) 
{
	if (!Mode.HasFlag(SCPCFileMode::Read))
	{
		GOutputDevice->Error("Attempted to open a file in write-only mode");
		return {};
	}

	tl::optional<SCPRootInfo> FileRoot = CFileDatabase().GetRootByID(FileInfo.GetAssociatedRootID());
	
	if (!FileRoot.has_value())
	{
		GOutputDevice->Warning(LOCATION, "Open requested for a SCPCFileInfo with an invalid Root ID");
		return {};
	}
	switch (FileRoot->GetType() )
	{
	case SCPRootType::Path:
		
		if (Mode.HasFlag(SCPCFileMode::MemoryMapped))
		{
			SCPFile MemMappedFile(FileInfo.GetFullPath());
			if (!MemMappedFile.Exists())
			{
				GOutputDevice->Error("Attempted to open a memory-mapped file that does not exist!");
				return {};
			}
			if (!Mode.Is({ SCPCFileMode::MemoryMapped, SCPCFileMode::Read, SCPCFileMode::Binary }))
			{
				GOutputDevice->Error("Memory-mapped IO only supports read-only, binary mode!");
				return {};
			}
		}
		// use the loose file constructor
		return std::make_unique<CFILE>(SCPCallPermit<SCPCFileModule>{}, FileInfo.GetFullPath(), Mode);
		break;
	case SCPRootType::PackFile:
		//use the file-in-packfile constructor
		return std::make_unique<CFILE>(SCPCallPermit<SCPCFileModule>{}, FileRoot->GetPath(), FileInfo.GetPackFileOffset(), FileInfo.GetFileSize());
		break;
	case SCPRootType::InMemory:
		//use in-memory constructor
		return std::make_unique<CFILE>(SCPCallPermit<SCPCFileModule>{}, FileInfo.GetFileSize(), FileInfo.GetDataPointer());
	}
	return {};
}



tl::optional<SCPCFileInfo> SCPCFileModule::FindFileInfo(const SCPPath FilePath, SCPCFilePathTypeID PathType, bool localize /*= false*/, uint32_t location_flags /*= CF_LOCATION_ALL*/, SCP_string LanguagePrefix /*= ""*/)
{
	Assert(!FilePath.empty());

	//search for the exact path first
	if (FilePath.has_parent_path())
	{
		for (auto FileResult : CFileDatabase().Files(FileFilter().FullPathIs(FilePath.string())))
		{
			//if we have a result we just want the first one that matches
			return FileResult;
		}
	}

	FileFilter Filter;
	Filter.LocationMatches(location_flags);
	Filter.SortByPathType(true);

	if (localize)
	{
		SCPPath LocalizedPath = SCPPath(FilePath);
		LocalizedPath = LocalizedPath.remove_filename() / LanguagePrefix / LocalizedPath.filename();

		Filter.FilenameIs(LocalizedPath.string());
	}
	else
	{
		Filter.FilenameIs(FilePath.string());
	}

	if (PathType != SCPCFilePathTypeID::Any)
	{
		Filter.PathTypeIs(PathType);
	}

	for (auto FileResult : CFileDatabase().Files(Filter))
	{
		return FileResult;
	}

	//return empty optional
	return {};
}