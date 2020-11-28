#pragma once
#include "FSStdTypes.h"
#include "cfile/SCPCFileModule.h"
#include "mod_table/SCPGameSettingsTable.h"
#include "module/SCPModuleBase.h"
namespace SCP
{
	class GameSettingsModule : public SCPModule<GameSettingsModule>
	{
	public:
		virtual bool StartupModule() override;
		virtual void ShutdownModule() override {}

		static GameSettingsModule ConstructModule(class SCPCFileModule& CFileDependency);
		tl::optional<GameSettingsTable> GameSettings()
		{
			return LoadedSettings;
		}
	private:
		tl::optional<GameSettingsTable> LoadedSettings;
	};

} // namespace SCP
