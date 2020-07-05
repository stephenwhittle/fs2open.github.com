#include "cmdline/SCPCmdlineModule.h"
#include "cmdline/SCPCmdLine.h"
#include "SCPApplication.h"

void SCPCmdlineModule::StartupModule() 
{
	SCPCmdLineParser Parser;
	CurrentOptions = std::make_unique<SCPCmdLineOptions>(std::move(Parser.GetOptions(SCPApplication::Get().GetCommandLineArgs())));
}
