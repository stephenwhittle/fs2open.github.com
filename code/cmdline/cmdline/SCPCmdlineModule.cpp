#include "cmdline/SCPCmdlineModule.h"
#include "cmdline/SCPCmdLine.h"
#include "SCPApplication.h"

bool SCPCmdlineModule::StartupModule() 
{
	SCPCmdLineParser Parser;
	CurrentOptions = std::make_unique<SCPCmdLineOptions>(Parser.GetOptions(SCPApplication::Get().GetCommandLineArgs()));
	return true;
}
