#pragma once
#include "module/SCPModuleBase.h"
#include "cmdline/SCPCmdLine.h"
#include <memory>


class SCPCmdlineModule : public SCPModule<SCPCmdlineModule> {
  public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override {}
	std::unique_ptr<SCPCmdLineOptions> CurrentOptions;
	
	static SCPCmdlineModule ConstructModule() 
	{ 
		SCPCmdlineModule ModuleInstance;
		ModuleInstance.StartupModule();
		return ModuleInstance;
	}
};