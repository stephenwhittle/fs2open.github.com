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

	static bool Compare(const SCPPath& Left, const SCPPath& Right, bool CaseSensitive = false);
	static SCP_string GetCompatiblePath(SCP_string InPath);
	static const char* GetFilename(const char* InPath);
	static char* GetFilename(char* InPath);
	static SCPPath AddExtensionIfMissing(SCP_string Filename, SCP_string ExtensionToAdd);
	SCPPath(const ghc::filesystem::path& InPath)
		:ghc::filesystem::path(InPath) {};
	SCPPath(const SCP_string& InString)
		:ghc::filesystem::path(InString) {};
	SCPPath(const char* InString)
		:ghc::filesystem::path(InString) {};
	SCPPath& operator =(const ghc::filesystem::path& InPath)
	{
		assign(InPath.c_str());
		return *this;
	}
};
