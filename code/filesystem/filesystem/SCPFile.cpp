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

bool SCPFile::Exists()
{
	return ghc::filesystem::exists(CachedPath);
}

bool SCPFile::Exists(SCP_string FilePath)
{
	return ghc::filesystem::exists(ghc::filesystem::path(FilePath));
}

std::uintmax_t SCPFile::FileSize(SCP_string FilePath) 
{
	return ghc::filesystem::file_size(ghc::filesystem::path(FilePath));
}

std::uintmax_t SCPFile::FileSize() 
{
	return ghc::filesystem::file_size(CachedPath);
}
