#include <ghc/fs_fwd.hpp>
#include "cmdline/SCPCmdLine.h"
#include "cfile/SCPCFileModule.h"
#include "module/SCPModuleManager.h"
#include "SCPApplication.h"
int main(int argc, char* const argv[])
{
	SCPApplication::Get().SetCommandLine(argc, argv);
	//auto ParsedOptions = GCmdLineParser.GetOptions(argc, argv);
	//
	auto CfileModule = SCPModuleManager::GetModule<SCPCFileModule>();
	
}