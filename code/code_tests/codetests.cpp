#include "color/alphacolors.h"
#include "cfile/cfile.h"
#include <ghc/fs_fwd.hpp>
#include "cmdline/SCPCmdLine.h"
#include "cfile/SCPCFileModule.h"
#include "module/SCPModuleManager.h"
#include "SCPApplication.h"
int main(int argc, char* const argv[])
{
	SCPApplication::Get().SetCommandLine(argc, argv);
	//auto ParsedOptions = GCmdLineParser.GetOptions(argc, argv);
	cfile_init((ghc::filesystem::current_path() / "fs2.exe").c_str());
	//
	auto CfileModule = SCPModuleManager::GetModule<SCPCFileModule>();
	alpha_colors_init();
}