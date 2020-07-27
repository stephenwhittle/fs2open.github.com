#include <ghc/fs_fwd.hpp>
#include "cmdline/SCPCmdLine.h"
#include "cfile/SCPCFileModule.h"
#include "module/SCPModuleManager.h"
#include "SCPApplication.h"
#include "SCPProfiling.h"
int main(int argc, char* const argv[])
{
	FS2_PROF_APP("Code Tests");

	SCPApplication::Get().SetCommandLine(argc, argv);
	//auto ParsedOptions = GCmdLineParser.GetOptions(argc, argv);
	//
	auto CfileModule = SCPModuleManager::GetModule<SCPCFileModule>();
	
}