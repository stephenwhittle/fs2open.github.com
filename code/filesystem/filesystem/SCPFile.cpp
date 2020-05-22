#include "SCPFile.h"
#include <ghc/fs_fwd.hpp>



 SCPFile::SCPFile(SCP_string FilePath) : CachedPath(FilePath, ghc::filesystem::path::auto_format){}

std::time_t SCPFile::LastModifiedTime()
{
	return std::chrono::system_clock::to_time_t(ghc::filesystem::last_write_time(CachedPath));
}

std::time_t SCPFile::LastModifiedTime(SCP_string FilePath)
{
	return std::chrono::system_clock::to_time_t(ghc::filesystem::last_write_time(FilePath));
}
