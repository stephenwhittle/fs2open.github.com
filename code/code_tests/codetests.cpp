#include "color/alphacolors.h"
#include "cfile/cfile.h"
#include <ghc/fs_fwd.hpp>
#include "cmdline/SCPCmdLine.h"

SCPCmdLineParser GCmdLineParser;
int main(int argc, char* const argv[])
{
	auto ParsedOptions = GCmdLineParser.GetOptions(argc, argv);
	//cfile_init((ghc::filesystem::current_path() / "fs2.exe").c_str());
	//alpha_colors_init();
}