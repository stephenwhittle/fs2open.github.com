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


 SCPConfig::SCPConfig()
{

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