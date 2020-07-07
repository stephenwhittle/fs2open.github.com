#include "cmdline/SCPCmdlineModule.h"
#include "cmdline/SCPCmdLine.h"
#include "SCPApplication.h"
#include "filesystem/SCPFile.h"
#include "filesystem/SCPPath.h"
#include "module/SCPModuleManager.h"
#include "FSStdTypes.h"

const SCP_string ConfigFileName = "fs2_open.ini";

bool SCPCmdlineModule::StartupModule() 
{
	SCPCmdLineParser Parser;
	CurrentOptions = std::make_unique<SCPCmdLineOptions>(Parser.GetOptions(SCPApplication::Get().GetCommandLineArgs()));
	
	//if portable mode option is not set or it is set to false
	if (CurrentOptions->bEnablePortableMode.has_value() == false || *CurrentOptions->bEnablePortableMode == false)
	{
		bool bLegacyModeNewValue = false;
		SCP_string LegacyModeStatus;
		
		SCPPath PrefsPath = SCPApplication::Get().GetConfigPath();
		SCPPath ConfigFilePath = PrefsPath / ConfigFileName;
		SCPFile ConfigFile     = SCPFile(ConfigFilePath.string());
		time_t new_config_time = 0;
		if (ConfigFile.Exists()) {
			new_config_time = ConfigFile.LastModifiedTime();
		}

		// Also check the modification times of the command line files in case the launcher did not change the
		// settings file

		SCPPath PrefFilePath = PrefsPath / "data" / "cmdline_fso.cfg";
		SCPFile PrefFile     = SCPFile(PrefFilePath.string());
		if (PrefFile.Exists()) {
			new_config_time = std::max(new_config_time, PrefFile.LastModifiedTime());
		}


#ifdef SCP_UNIX
		SCPPath OldConfigPath  = GetLegacyUserPath() / ConfigFileName;
		SCPFile OldConfigFile  = SCPFile(OldConfigPath.string());
		time_t old_config_time = 0;
		if (OldConfigFile.Exists()) {
			old_config_time = OldConfigFile.LastModifiedTime();
		}

		SCPPath OldPrefFilePath = GetLegacyUserPath() / "data" / "cmdline_fso.cfg";
		SCPFile OldPrefFile     = SCPFile(OldPrefFilePath.string());
		if (OldPrefFile.Exists()) {
			old_config_time = std::max(old_config_time, OldPrefFile.LastModifiedTime());
		}
#else
		//TODO: @SCPCmdLineModule properly check if the registy exists
		// At this point we can't determine if the old config exists so just assume that it does
		//time_t old_config_time = os_registry_get_last_modification_time();

		// On Windows the cmdline_fso file was stored in the game root directory which should be in the current
		// directory
		SCPPath CmdlineFilePath = SCPPath(".") / "data" / "cmdline_fso.cfg";
		SCPFile CmdlineFile     = SCPFile(CmdlineFilePath.string());
		time_t old_config_time = CmdlineFile.Exists() ? CmdlineFile.LastModifiedTime() : 0;
		/*if (CmdlineFile.Exists()) {
			old_config_time = std::max(old_config_time, CmdlineFile.LastModifiedTime());
		}*/
#endif

		if (new_config_time > 0 && old_config_time > 0) {
			// Both config files exists so we need to decide which to use based on their last modification times
			// if the old config was modified more recently than the new config then we use the legacy mode since
			// the user probably used an outdated launcher after using a more recent one
			bLegacyModeNewValue = old_config_time > new_config_time;

			if (bLegacyModeNewValue) {
				LegacyModeStatus = "Legacy mode enabled since the old config location was used more recently than "
									"the new location.";
			} else {
				LegacyModeStatus = "Legacy mode disabled since the new config location was used more recently than "
									"the old location.";
			}
		} else if (new_config_time > 0) {
			// If the new config exists and the old one doesn't then we can safely disable the legacy mode
			bLegacyModeNewValue = false;

			LegacyModeStatus =
				"Legacy mode disabled since the old config does not exist while the new config exists.";
		} else if (old_config_time > 0) {
			// Old config exists but new doesn't -> use legacy mode
			bLegacyModeNewValue = true;

			LegacyModeStatus =
				"Legacy mode enabled since the old config exists while the new config does not exist.";
		} else {
			// Neither old nor new config exists -> this is a new install
			bLegacyModeNewValue = false;

			LegacyModeStatus = "Legacy mode disabled since no existing config was detected.";
		}

		if (bLegacyModeNewValue) {
			// Print a message for the people running it from the terminal    
			GOutputDevice->PrintGeneral(
					"FSO is running in legacy config mode. %s Please either update your launcher or"
					" copy the configuration and pilot files to '%s' for better future compatibility.\n",
				LegacyModeStatus.c_str(),
				PrefsPath.c_str());
		}
		SCPApplication::Get().SetLegacyMode(bLegacyModeNewValue);
	}
	return true;
}
