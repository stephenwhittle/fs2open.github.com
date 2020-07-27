#include <ghc/fs_fwd.hpp>
#include "cmdline/SCPCmdLine.h"
#include "cfile/SCPCFileModule.h"
#include "module/SCPModuleManager.h"
#include "SCPApplication.h"
#include "SCPProfiling.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

int main(int argc, char* const argv[])
{
	FS2_PROF_APP("Code Tests");

	SCPApplication::Get().SetCommandLine(argc, argv);
	//auto ParsedOptions = GCmdLineParser.GetOptions(argc, argv);
	//
	doctest::Context CurrentContext;
	int Result = CurrentContext.run();
	return Result;
}