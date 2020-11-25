#include "doctest/doctest.h"
#include "mod_table/SCPGameSettingsModule.h"
#include "module/SCPModuleManager.h"

TEST_CASE("game settings init") 
{
	tl::optional<SCP::GameSettingsModule&> ModuleHandle = SCPModuleManager::GetModule<SCP::GameSettingsModule>();
}