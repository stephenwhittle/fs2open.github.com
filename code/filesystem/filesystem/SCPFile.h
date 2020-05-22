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

	bool Exists();

	static std::time_t LastModifiedTime(SCP_string FilePath);
	static bool Exists(SCP_string FilePath);
};