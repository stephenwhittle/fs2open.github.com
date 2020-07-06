#pragma once
#include <vector>
//TODO: @SCPApplication refactor FredRunning into some other kind of thing, preferably a global function or static member function for SCPApp?
extern int Fred_running;  // Is Fred running, or FreeSpace?
extern int Lcl_current_lang;
class SCPApplication
{
private:
	std::vector<std::string> CmdlineArguments;
public:
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
	int GetCurrentLanguage() { return 0; };
};
