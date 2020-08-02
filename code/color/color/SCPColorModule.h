#pragma once

#include "module/SCPModuleBase.h"

class SCPColorModule : public SCPModule<SCPColorModule>
{

public:
	bool StartupModule() override;

	void ShutdownModule() override {}
	
	std::unique_ptr<class SCPColorSet> Colors;

	static SCPColorModule ConstructModule(class SCPCFileModule&)
	{
		SCPColorModule Module;
		return Module;
	}
};