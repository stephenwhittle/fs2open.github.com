#pragma once

#include "module/SCPModuleBase.h"

namespace SCP
{
	class SoundModule : public SCPModule<SoundModule>
	{
	public:
		bool StartupModule() override
		{
			return true;
		}

		void ShutdownModule() override {}

		static SoundModule ConstructModule()
		{
			SoundModule ModuleInstance;

			return ModuleInstance;
		}
	};
} // namespace SCP
