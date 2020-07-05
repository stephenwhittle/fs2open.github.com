#pragma once
#include "module/SCPModuleBase.h"

#include "cmdline/SCPCmdlineModule.h"

#include <tuple>


class SCPCFileModule : public SCPModule<SCPCFileModule> {
  public:
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
	
	static SCPCFileModule ConstructModule(SCPCmdlineModule& Dependency)
	{
		return SCPCFileModule();
	}
};

