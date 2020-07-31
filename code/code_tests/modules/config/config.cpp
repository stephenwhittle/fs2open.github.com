#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"
#include "config/SCPConfigModule.h"

TEST_SUITE("Config Module")
{
	tl::optional<SCPConfigModule&> ModuleHandle;
	TEST_CASE("Config Module Init")
	{
		ModuleHandle = SCPModuleManager::GetModule<SCPConfigModule>();
		REQUIRE(ModuleHandle);
	}
}