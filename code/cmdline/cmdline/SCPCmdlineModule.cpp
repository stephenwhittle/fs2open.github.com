#include "SCPCmdlineModule.h"
#include "SCPCmdLine.h"


void SCPCmdlineModule::StartupModule() 
{
	SCPCmdLineParser Parser;
	CurrentOptions = Parser.GetOptions();
}
