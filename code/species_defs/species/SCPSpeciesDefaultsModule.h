#pragma once

#include "module/SCPModuleBase.h"

namespace SCP
{
	class SpeciesDefaultsModule : SCPModuleBase<SpeciesDefaultsModule>
	{

public:
		bool StartupModule() override
		{
			return true;
		}

		void ShutdownModule() override
		{
			
		}
		static SpeciesDefaultsModule ConstructModule() {
			return SpeciesDefaultsModule();
		}
	};
}