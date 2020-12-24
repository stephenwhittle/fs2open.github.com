#include "doctest/doctest.h"
#include "species/SCPSpeciesDefaultsModule.h"
#include "module/SCPModuleManager.h"

TEST_CASE("species init")
{
	tl::optional<SCP::SpeciesDefaultsModule&> ModuleHandle = SCPModuleManager::GetModule<SCP::SpeciesDefaultsModule>();
	REQUIRE(ModuleHandle);
}