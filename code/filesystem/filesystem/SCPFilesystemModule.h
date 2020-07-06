#pragma once
#include "module/SCPModuleBase.h"
#include "filesystem/SCPWorkingDirectory.h"

class SCPFilesystemModule : public SCPModule<SCPFilesystemModule> {
	class SCPPath ConfigPath;
	void GetPreferencesPath();
public:
	virtual bool StartupModule() override { return true; }
	virtual void ShutdownModule() override {}
	SCPWorkingDirectory WorkingDirectory;
	SCPPath GetConfigPath()
	{
		return ConfigPath;
	}
	static SCPFilesystemModule ConstructModule() 
	{ 
		return SCPFilesystemModule(); 
	}
};
