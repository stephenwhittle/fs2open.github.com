#pragma once
#include <vector>
#include <ghc/fs_fwd.hpp>

#include platform_header_core_SCPGetPrefsPath

//TODO: @SCPApplication refactor FredRunning into some other kind of thing, preferably a global function or static member function for SCPApp?
extern int Fred_running;  // Is Fred running, or FreeSpace?
extern int Lcl_current_lang;

/// <summary>
/// Core singleton class, contains raw command-line args and configuration paths
/// </summary> 
class SCPApplication
{
private:
	std::vector<std::string> CmdlineArguments;
	bool bLegacyMode = false;
	SCP_string ConfigPath;
public:
	SCPApplication()
	{
		ghc::filesystem::path PrefsPathBase = SCPGetPrefsPath();
		ConfigPath = PrefsPathBase / "HardLightProductions" / "FreeSpaceOpen";
	}
	static inline SCPApplication& Get()
	{
		static SCPApplication Instance;
		return Instance;
	}
	const std::vector<std::string>& GetCommandLineArgs()
	{
		return CmdlineArguments;
	}
	std::string ExecutablePath;

	void SetCommandLine(int argc, char* const argv[])
	{
		ExecutablePath = std::string(argv[0]);
		for (int ArgIndex = 1; ArgIndex < argc; ArgIndex++)
		{
			CmdlineArguments.push_back(std::string(argv[ArgIndex]));
		}
	}
	void SetLegacyMode(bool bNewValue) { bLegacyMode = bNewValue; }
	bool GetLegacyMode() { return bLegacyMode; }
	
	SCP_string GetConfigPath() { return ConfigPath; }
	int GetCurrentLanguage() { return 0; };
};
