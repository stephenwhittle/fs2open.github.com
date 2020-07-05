#include "SCPModuleManager.h"

std::map<int, std::unique_ptr<SCPModuleBase>> SCPModuleManager::Modules{};

