#include "doctest/doctest.h"
#include "species/SCPSpeciesDefaultsModule.h"
#include "module/SCPModuleManager.h"

TEST_CASE("game settings init")
{
	tl::optional<SCP::SpeciesDefaultsModule&> ModuleHandle = SCPModuleManager::GetModule<SCP::SpeciesDefaultsModule>();
	REQUIRE(ModuleHandle);
}