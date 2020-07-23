#pragma once
#include "FSStdTypes.h"
#include <ghc/fs_fwd.hpp>

#include <ctime>

class SCPFile
{
	ghc::filesystem::path CachedPath;
public:
	SCPFile(SCP_string FilePath);
	std::time_t LastModifiedTime();
	std::uintmax_t FileSize();
	bool Exists();
	bool IsReadOnly();
	static std::time_t LastModifiedTime(SCP_string FilePath);
	static bool Exists(SCP_string FilePath);
	static std::uintmax_t FileSize(SCP_string FilePath);
	static bool Delete(SCP_string FilePath);
};