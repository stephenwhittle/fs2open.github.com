#pragma once
#include "module/SCPModuleBase.h"
#include "filesystem/SCPWorkingDirectory.h"

class SCPFilesystemModule : public SCPModule<SCPFilesystemModule> {
public:
	virtual bool StartupModule() override;
	virtual void ShutdownModule() override {}
	SCPWorkingDirectory WorkingDirectory;

	static SCPFilesystemModule ConstructModule() 
	{ 
		SCPFilesystemModule FSModuleInstance;
		return FSModuleInstance;
	}
};
