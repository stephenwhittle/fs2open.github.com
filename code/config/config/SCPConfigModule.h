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

	template<typename ValueType>
	void WriteConfigValue(class SCP_string SectionName, SCP_string KeyName, ValueType KeyValue);

	template<typename ValueType>
	tl::optional<ValueType> ReadConfigValue(SCP_string SectionName, SCP_string KeyName);


};
