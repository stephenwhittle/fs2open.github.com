#pragma once
#include "FSStdTypes.h"
#include "module/SCPModuleBase.h"

namespace SCP
{
	class GameSettingsModule : public SCPModule<GameSettingsModule>
	{
	public:
		virtual bool StartupModule() override
		{
			return true;
		};
		virtual void ShutdownModule() override {}

		static GameSettingsModule ConstructModule()
		{
			return GameSettingsModule();
		}

	private:
	};

}
