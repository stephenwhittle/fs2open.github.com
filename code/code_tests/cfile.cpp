#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"
#include "cfile/SCPCFileModule.h"
#include "cfile/SCPCFileInfo.h"
TEST_SUITE("CFile Module")
{
	tl::optional<SCPCFileModule> ModuleHandle;
	
	TEST_CASE("CFile Module Initialization")
	{
		ModuleHandle = SCPModuleManager::GetModule<SCPCFileModule>();
		REQUIRE(ModuleHandle);
	}
	TEST_CASE("CFileModule GetDefaultFilePath")
	{

	}
	TEST_CASE("CFile Database Find file")
	{
		REQUIRE(ModuleHandle->FindFileInfo("ships.tbl", SCPCFilePathTypeID::Tables, false, SCPCFileLocationALL, ""));
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
			REQUIRE(!SCPCFileModule::CheckLocationFlags({SCPCFileLocation::GameRootDirectory},
														{SCPCFileLocation::UserDirectory}));
		}
		SUBCASE("Filter only specifying root type should accept flags that match but also have a mod location set")
		{
			REQUIRE(SCPCFileModule::CheckLocationFlags({SCPCFileLocation::GameRootDirectory, SCPCFileLocation::TopLevelDirectory},{SCPCFileLocation::GameRootDirectory}));
		}
	}
}