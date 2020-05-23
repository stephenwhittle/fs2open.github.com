#include "SCPConfig.h"
#include "filesystem/SCPPath.h"
#include "filesystem/SCPFile.h"
#include "config/osregistry.h"

#include "SCPCmdOptions.h"
#include "FSAssert.h"
#include "SDL_filesystem.h"
#include "SDL_error.h"
static const char* ORGANIZATION_NAME = "HardLightProductions";
static const char* APPLICATION_NAME = "FreeSpaceOpen";

SCP_string SCPConfig::LegacyModeStatus;

SCPPath SCPConfig::GetPrefsPath()
{
	// Lazily initialize the preferences path
	if (CachedPrefsPath.empty()) {
		const char* DetectedPath = SDL_GetPrefPath(ORGANIZATION_NAME, APPLICATION_NAME);
		
		if (DetectedPath == nullptr) {
			mprintf(("Failed to get preferences path from SDL: %s\n", SDL_GetError()));
		}else{
			CachedPrefsPath = SCPPath(DetectedPath);
		}
//this may no longer be a problem
/*
#ifdef WIN32
		try {
			auto current = CachedPrefsPath;
			const auto prefPathEnd = preferencesPath + strlen(preferencesPath);
			while (current != prefPathEnd) {
				const auto cp = utf8::next(current, prefPathEnd);
				if (cp > 127) {
					// On Windows, we currently do not support Unicode paths so catch this early and let the user
					// know
					const auto invalid_end = current;
					utf8::prior(current, preferencesPath);
					Error(LOCATION,
						"Determined the preferences path as \"%s\". That path is not supported since it "
						"contains a Unicode character (%s). If possible, choose a different user name or "
						"use portable mode.",
						preferencesPath, std::string(current, invalid_end).c_str());
				}
			}
		}
		catch (const std::exception & e) {
			Error(LOCATION, "UTF-8 error while checking the preferences path \"%s\": %s", preferencesPath,
				e.what());
		}
#endif
*/
	}

	if (!CachedPrefsPath.empty()) {
		return CachedPrefsPath;
	}
	else {
		// No preferences path, try current directory
		return SCPPath(".");
	}
}

SCPPath SCPConfig::GetLegacyUserPath()
{
#ifdef WIN32
	return SCPPath("");
#else
	static bool user_dir_initialized = false;
	static SCP_string Os_user_dir_legacy;

	if (user_dir_initialized) {
		return Os_user_dir_legacy.c_str();
	}

	extern const char* Osreg_user_dir_legacy;
	fmt::sprintf(Os_user_dir_legacy, "%s/%s", getenv("HOME"), Osreg_user_dir_legacy);
	user_dir_initialized = true;

	return Os_user_dir_legacy.c_str();
#endif
}

const SCP_string SCPConfig::ConfigFileName = "fs2open.ini";

 SCPConfig::SCPConfig()
{

	 if (Cmdline_portable_mode) {
		 // When the portable mode option is given, non-legacy is implied
		 bLegacyMode = false;
		 LegacyModeStatus = "Legacy mode disabled since portable mode was enabled.";
	 }
	 else {

		 SCPPath ConfigFilePath = GetPrefsPath() / ConfigFileName;
		 SCPFile ConfigFile = SCPFile(ConfigFilePath.string());
		 time_t new_config_time = 0;
		 if (ConfigFile.Exists()) {
			 new_config_time = ConfigFile.LastModifiedTime();
		 }

		 // Also check the modification times of the command line files in case the launcher did not change the settings
		 // file

		 SCPPath PrefFilePath = GetPrefsPath() / "data" / "cmdline_fso.cfg";
		 SCPFile PrefFile = SCPFile(PrefFilePath.string());
		 if (PrefFile.Exists())
		 {
			 new_config_time = std::max(new_config_time, PrefFile.LastModifiedTime());
		 }

		 
#ifdef SCP_UNIX
		 SCPPath OldConfigPath = GetLegacyUserPath() / ConfigFileName;
		 SCPFile OldConfigFile = SCPFile(OldConfigPath.string());
		 time_t old_config_time = 0;
		 if (OldConfigFile.Exists()) {
			 old_config_time = OldConfigFile.LastModifiedTime();
		 }

		 SCPPath OldPrefFilePath = GetLegacyUserPath() / "data" / "cmdline_fso.cfg";
		 SCPFile OldPrefFile = SCPFile(OldPrefFilePath.string());
		 if (OldPrefFile.Exists())
		 {
			 old_config_time = std::max(old_config_time, OldPrefFile.LastModifiedTime());
		 }
#else
		 // At this point we can't determine if the old config exists so just assume that it does
		 time_t old_config_time = os_registry_get_last_modification_time();

		 // On Windows the cmdline_fso file was stored in the game root directory which should be in the current directory
		 SCPPath CmdlineFilePath = SCPPath(".") / "data" / "cmdline_fso.cfg";
		 SCPFile CmdlineFile = SCPFile(CmdlineFilePath.string());
		 if (CmdlineFile.Exists())
		 {
			old_config_time = std::max(old_config_time, CmdlineFile.LastModifiedTime());
		 }
#endif

		 if (new_config_time > 0 && old_config_time > 0) {
			 // Both config files exists so we need to decide which to use based on their last modification times
			 // if the old config was modified more recently than the new config then we use the legacy mode since the
			 // user probably used an outdated launcher after using a more recent one
			 bLegacyMode = old_config_time > new_config_time;

			 if (bLegacyMode) {
				 LegacyModeStatus = "Legacy mode enabled since the old config location was used more recently than the new location.";
			 }
			 else {
				 LegacyModeStatus = "Legacy mode disabled since the new config location was used more recently than the old location.";
			 }
		 }
		 else if (new_config_time > 0) {
			 // If the new config exists and the old one doesn't then we can safely disable the legacy mode
			 bLegacyMode = false;

			 LegacyModeStatus = "Legacy mode disabled since the old config does not exist while the new config exists.";
		 }
		 else if (old_config_time > 0) {
			 // Old config exists but new doesn't -> use legacy mode
			 bLegacyMode = true;

			 LegacyModeStatus = "Legacy mode enabled since the old config exists while the new config does not exist.";
		 }
		 else {
			 // Neither old nor new config exists -> this is a new install
			 bLegacyMode = false;

			 LegacyModeStatus = "Legacy mode disabled since no existing config was detected.";
		 }
	 }

	 if (bLegacyMode) {
		 // Print a message for the people running it from the terminal
		 fprintf(stdout, "FSO is running in legacy config mode. Please either update your launcher or"
			 " copy the configuration and pilot files to '%s' for better future compatibility.\n", GetPrefsPath().c_str());
	 }

}

SCP_string SCPConfig::GetConfigPath(SCPPath Subpath)
{
	Verify(Subpath.is_relative() == true);
	if (Cmdline_portable_mode)
	{
		return SCPPath(".") / Subpath;
	}
	if (IsLegacyMode())
	{
#ifdef WIN32
		return SCPPath(".") / Subpath;
#else
		extern const char* Osreg_user_dir_legacy;
		return SCPPath(getenv("HOME")) / Osreg_user_dir_legacy / Subpath;
#endif
	}
	return GetPrefsPath() / Subpath;
}

//TODO: need to fix to ensure static initialization order isn't violated
SCPConfig* GConfig = new SCPConfig();