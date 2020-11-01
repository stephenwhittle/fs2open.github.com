#pragma once
#include "module/SCPModuleBase.h"

class SCPLocalizationModule : public SCPModule<SCPLocalizationModule>
{
public:

	static SCPLocalizationModule ConstructModule(SCPCFileModule& CFileModule) {
		SCPLocalizationModule ModuleInstance;

		return ModuleInstance;
	}

	SCP_string GetCurrentLanguageName();

	SCP_string LocalizeString(SCP_string Default, std::uint32_t StringID);


};