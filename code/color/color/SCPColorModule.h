#pragma once

#include "module/SCPModuleBase.h"
#include "SCPColorTable.h"
class SCPColorModule : public SCPModule<SCPColorModule>
{

public:
	bool StartupModule() override;

	void ShutdownModule() override {}
	
	std::unique_ptr<SCPColorSet> Colors;

	static SCPColorModule ConstructModule(class SCPCFileModule&)
	{
		SCPColorModule Module;
		return Module;
	}
};