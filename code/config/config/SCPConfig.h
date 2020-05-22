#pragma once
#include <FSStdTypes.h>
#include "filesystem/SCPPath.h"
#include <ctime>

class SCPConfig
{
private:
	SCPPath GetPrefsPath();
	SCPPath GetLegacyUserPath();
	static const SCP_string ConfigFileName;
	SCPPath CachedPrefsPath;
	bool bLegacyMode;
	static SCP_string LegacyModeStatus;
public:
	//Allows for the overriding of legacy mode, skipping potentially expensive first check
	SCPConfig(bool LegacyMode)
		: bLegacyMode(LegacyMode) {};
	//Automatically determine legacy mode from presence and filetime modification of config files
	SCPConfig();

	SCP_string GetConfigPath(SCPPath Subpath);

	bool IsLegacyMode() const { return bLegacyMode; }
	void SetLegacyMode(bool IsLegacyMode) { bLegacyMode = IsLegacyMode; }
	SCP_string GetLegacyModeStatus() { return LegacyModeStatus; }
};

extern SCPConfig* GConfig;