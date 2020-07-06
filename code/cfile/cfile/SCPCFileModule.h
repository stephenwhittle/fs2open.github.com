#pragma once
#include "module/SCPModuleBase.h"

#include "cmdline/SCPCmdlineModule.h"
#include "filesystem/SCPFilesystemModule.h"
#include <tuple>


class SCPCFileModule : public SCPModule<SCPCFileModule> {
  public:
	virtual bool StartupModule() override;
	virtual void ShutdownModule() override {}
	
	class SCPPath UserDirectory;

	static SCPCFileModule ConstructModule(SCPCmdlineModule& Dependency, SCPFilesystemModule& FSDependency)
	{
		return SCPCFileModule();
	}
};

