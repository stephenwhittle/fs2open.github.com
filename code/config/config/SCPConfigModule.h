#pragma once
#include "cmdline/SCPCmdlineModule.h"
#include "module/SCPModuleBase.h"

class SCPConfigModule : public SCPModule<SCPConfigModule> {
  public:
	virtual bool StartupModule() override { return true; };
	virtual void ShutdownModule() override {}

	static SCPConfigModule ConstructModule(SCPCmdlineModule& CmdLineDependency)
	{
		return SCPConfigModule();
	}
};
