#pragma once
#include "FSStdTypes.h"
#include <ghc/fs_fwd.hpp>

#ifdef _WIN32
#define DIR_SEPARATOR_CHAR '\\'
#define DIR_SEPARATOR_STR  "\\"
#else
#define DIR_SEPARATOR_CHAR '/'
#define DIR_SEPARATOR_STR  "/"
#endif

class SCPPath : public ghc::filesystem::path
{
private:
public:
	SCPPath()
		: ghc::filesystem::path() {};

	static SCP_string GetCompatiblePath(SCP_string InPath);
	static const char* GetFilename(const char* InPath);
	static char* GetFilename(char* InPath);
	
	SCPPath(const SCP_string& InString)
		:ghc::filesystem::path(InString) {};
	SCPPath(const char* InString)
		:ghc::filesystem::path(InString) {};
};
