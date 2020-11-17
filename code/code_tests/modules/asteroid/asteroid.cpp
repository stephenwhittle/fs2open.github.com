#include "asteroid/SCPAsteroidTable.h"
#include "asteroid/SCPAsteroidTableDescriptor.h"
#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"
#include "cfile/SCPCFileModule.h"
#include "cfile/SCPCFileInfo.h"
#include "parse/SCPParser.h"


TEST_CASE("Asteroid table ")
{
	tl::optional<SCPCFileModule&> ModuleHandle = SCPModuleManager::GetModule<SCPCFileModule>();
	REQUIRE(ModuleHandle);
	auto AsteroidTestTableInfo = ModuleHandle->FindFileInfo("asteroid.tbl", SCPCFilePathTypeID::Tables);
	REQUIRE(AsteroidTestTableInfo);
	auto AsteroidTestCFile = ModuleHandle->CFileOpen(*AsteroidTestTableInfo, {SCPCFileMode::Read});
	REQUIRE(AsteroidTestCFile);
	
	tl::optional<SCP::AsteroidTable> Table =
		SCPParser::CreateBaseTable<SCP::AsteroidTable>(AsteroidsFile, AsteroidTestCFile->LoadAsText(), "asteroid.tbl");

	REQUIRE(Table);
}
