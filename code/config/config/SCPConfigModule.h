#pragma once
#include "cmdline/SCPCmdlineModule.h"
#include "module/SCPModuleBase.h"
#include "config/ConfigProfile.h"
#include "SCPApplication.h"

class SCPConfigModule : public SCPModule<SCPConfigModule> {
  public:
	virtual bool StartupModule() override { return true; };
	virtual void ShutdownModule() override {}

	SCPConfigModule() 
	:Config(SCPPath(SCP::Constants::ConfigFileName.c_str()))
	{

	}

	static SCPConfigModule ConstructModule(SCPCmdlineModule& CmdLineDependency)
	{
		return SCPConfigModule();
	}

	template<typename ValueType>
	void WriteConfigValue(class SCP_string SectionName, SCP_string KeyName, ValueType KeyValue)
	{
		Config->WriteConfigValue(SectionName, KeyName, KeyValue);
	}

	template<typename ValueType>
	tl::optional<ValueType> ReadConfigValue(SCP_string SectionName, SCP_string KeyName)
	{
		return Config->ReadConfigValue(SectionName, KeyName);
	}

private:

	SCP::ConfigProfile Config;

};
