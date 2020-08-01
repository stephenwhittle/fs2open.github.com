#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"
#include "cfile/SCPCFileModule.h"
#include "cfile/SCPCFileInfo.h"
#include "cfile/SCPCFileDatabase.h"
#include "cfile/SCPRootInfo.h"
TEST_SUITE("CFile Module")
{
	tl::optional<SCPCFileModule&> ModuleHandle;
	//TODO: additional tests for seeking etc
	//TODO: regex matching
	//TODO: sort reverse test
	TEST_CASE("CFile Module Initialization")
	{
		ModuleHandle = SCPModuleManager::GetModule<SCPCFileModule>();
		REQUIRE(ModuleHandle);
	}
	TEST_CASE("CFileModule GetDefaultFilePath")
	{

	}
	TEST_CASE("CFile Raw Query for filename")
	{
		FileFilter default_settings;
		default_settings.FilenameIs("game_settings.tbl");
		auto Results = ModuleHandle->CFileDatabase().Files(default_settings);
		REQUIRE(Results.begin() != Results.end());
	}
	TEST_CASE("CFile find in-memory root")
	{
		RootFilter InMemoryRoot;
		InMemoryRoot.TypeIs(SCPRootType::InMemory);
		auto Results = ModuleHandle->CFileDatabase().Roots(InMemoryRoot);
		REQUIRE(Results.begin() != Results.end());
	}
	TEST_CASE("CFile Database Find in-memory file")
	{
		REQUIRE(ModuleHandle->FindFileInfo("game_settings.tbl", SCPCFilePathTypeID::Tables, false, {SCPCFileLocation::MemoryRoot}, ""));
	}
	TEST_CASE("CFile Open in-memory file")
	{
		auto FileInfo = ModuleHandle->FindFileInfo("game_settings.tbl", SCPCFilePathTypeID::Tables, false, { SCPCFileLocation::MemoryRoot }, "");
		REQUIRE(FileInfo);
		
		SUBCASE("CFile can't be opened in write-only mode")
		{
			REQUIRE(!ModuleHandle->CFileOpen(*FileInfo, {SCPCFileMode::Write}));
		}

		SUBCASE("In-memory file opened in read-only mode")
		{
			REQUIRE(ModuleHandle->CFileOpen(*FileInfo, {SCPCFileMode::Read}));
		}
	}
	
	TEST_CASE("CFile Open loose file") 
	{
		auto FileInfo =
			ModuleHandle->FindFileInfo("ships.tbl", SCPCFilePathTypeID::Tables, false, SCPCFileLocationALL, "");

		SUBCASE("Find loose file")
		{
			REQUIRE(FileInfo);
		}
		SUBCASE("Open it")
		{
			REQUIRE(ModuleHandle->CFileOpen(*FileInfo, { SCPCFileMode::Read }));
		}
		SUBCASE("Read it")
		{

		}
	}

	TEST_CASE("CFile Open packed file") 
	{
		auto FileInfo = ModuleHandle->FindFileInfo("test2.tbl", SCPCFilePathTypeID::Tables, false, SCPCFileLocationALL, "");
		REQUIRE(FileInfo);
		REQUIRE(ModuleHandle->CFileOpen(*FileInfo, { SCPCFileMode::Read }));
		SUBCASE("Read it") {}
	}
	TEST_CASE("CFile Open memory-mapped file")
	{
		SUBCASE("Read it") {}
	}
	TEST_CASE("Override default file")
	{
		auto FileInfo = ModuleHandle->FindFileInfo("controlconfigdefaults.tbl", SCPCFilePathTypeID::Tables, false, SCPCFileLocationALL, "");
		REQUIRE(FileInfo);
		REQUIRE(ModuleHandle->CFileOpen(*FileInfo, { SCPCFileMode::Read }));
	}
	TEST_CASE("FindFileInfo returns results in the correct order")
	{

	}
	TEST_CASE("CFile Module CheckLocationFlags")
	{
		SUBCASE("Filter same as checked flags")
		{
			REQUIRE(SCPCFileModule::CheckLocationFlags({ SCPCFileLocation::GameRootDirectory }, { SCPCFileLocation::GameRootDirectory }));
			REQUIRE(SCPCFileModule::CheckLocationFlags({ SCPCFileLocation::UserDirectory }, { SCPCFileLocation::UserDirectory }));
			REQUIRE(SCPCFileModule::CheckLocationFlags({ SCPCFileLocation::MemoryRoot}, { SCPCFileLocation::MemoryRoot }));
			REQUIRE(SCPCFileModule::CheckLocationFlags({ SCPCFileLocation::TopLevelDirectory}, { SCPCFileLocation::TopLevelDirectory}));
			REQUIRE(SCPCFileModule::CheckLocationFlags({ SCPCFileLocation::PrimaryMod}, { SCPCFileLocation::PrimaryMod }));
			REQUIRE(SCPCFileModule::CheckLocationFlags({ SCPCFileLocation::SecondaryMods}, { SCPCFileLocation::SecondaryMods}));
		}
		
		SUBCASE("ALL filter matches everything")
		{
			REQUIRE(SCPCFileModule::CheckLocationFlags({ SCPCFileLocation::GameRootDirectory }, { SCPCFileLocationALL }));
			REQUIRE(SCPCFileModule::CheckLocationFlags({ SCPCFileLocation::UserDirectory }, { SCPCFileLocationALL }));
			REQUIRE(SCPCFileModule::CheckLocationFlags({ SCPCFileLocation::MemoryRoot }, { SCPCFileLocationALL }));
			REQUIRE(SCPCFileModule::CheckLocationFlags({ SCPCFileLocation::TopLevelDirectory }, { SCPCFileLocationALL }));
			REQUIRE(SCPCFileModule::CheckLocationFlags({ SCPCFileLocation::PrimaryMod }, { SCPCFileLocationALL }));
			REQUIRE(SCPCFileModule::CheckLocationFlags({ SCPCFileLocation::SecondaryMods }, { SCPCFileLocationALL }));
		}
		SUBCASE("Filter requires different value")
		{
			REQUIRE_FALSE(SCPCFileModule::CheckLocationFlags({SCPCFileLocation::GameRootDirectory},
														{SCPCFileLocation::UserDirectory}));
		}
		SUBCASE("Filter only specifying root type should accept flags that match but also have a mod location set")
		{
			REQUIRE(SCPCFileModule::CheckLocationFlags({SCPCFileLocation::GameRootDirectory, SCPCFileLocation::TopLevelDirectory},{SCPCFileLocation::GameRootDirectory}));
		}
		SUBCASE("Original fs2open check_location_flags test")
		{
			REQUIRE_FALSE(SCPCFileModule::CheckLocationFlags({ SCPCFileLocation::GameRootDirectory, SCPCFileLocation::TopLevelDirectory }, { SCPCFileLocation::UserDirectory }));
			REQUIRE(SCPCFileModule::CheckLocationFlags({SCPCFileLocation::GameRootDirectory , SCPCFileLocation::TopLevelDirectory}, {SCPCFileLocation::GameRootDirectory}));
			REQUIRE(SCPCFileModule::CheckLocationFlags({SCPCFileLocation::GameRootDirectory , SCPCFileLocation::TopLevelDirectory}, {SCPCFileLocation::TopLevelDirectory}));
			REQUIRE_FALSE(SCPCFileModule::CheckLocationFlags({SCPCFileLocation::GameRootDirectory , SCPCFileLocation::TopLevelDirectory}, {SCPCFileLocation::PrimaryMod}));
			REQUIRE_FALSE(SCPCFileModule::CheckLocationFlags({SCPCFileLocation::GameRootDirectory , SCPCFileLocation::TopLevelDirectory}, {SCPCFileLocation::SecondaryMods}));
			REQUIRE_FALSE(SCPCFileModule::CheckLocationFlags({SCPCFileLocation::GameRootDirectory , SCPCFileLocation::TopLevelDirectory}, {SCPCFileLocation::GameRootDirectory , SCPCFileLocation::PrimaryMod}));
			REQUIRE(SCPCFileModule::CheckLocationFlags({SCPCFileLocation::GameRootDirectory , SCPCFileLocation::TopLevelDirectory}, {SCPCFileLocation::GameRootDirectory , SCPCFileLocation::TopLevelDirectory}));
		}
	}
}