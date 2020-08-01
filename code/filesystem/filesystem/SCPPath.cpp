#include <ghc/fs_impl.hpp>
#include "SCPPath.h"
#include "SCPStringUtils.h"
#include <algorithm>

bool SCPPath::Compare(const SCPPath& Left, const SCPPath& Right, bool CaseSensitive /*= false*/) 
{
	std::string LeftString = Left.string();
	std::string RightString = Right.string();
	if (LeftString.size() != RightString.size())
	{
		return false;
	}
	if (CaseSensitive)
	{
		return Left.string() == Right.string();
	}
	else
	{
		return StringUtils::ToLower(LeftString) == StringUtils::ToLower(RightString);
	}
}

SCP_string SCPPath::GetCompatiblePath(SCP_string InPath)
{
	SCP_string CompatiblePath = InPath;
	std::replace(CompatiblePath.begin(), CompatiblePath.end(), '/', DIR_SEPARATOR_CHAR);
	return CompatiblePath;
}

const char* SCPPath::GetFilename(const char* InPath)
{
	const char* p = strrchr(InPath, DIR_SEPARATOR_CHAR);
	if (p == NULL) {
		p = InPath;
	}
	else {
		p++;
	}
	return p;

}
char* SCPPath::GetFilename(char* InPath)
{
	char* p = strrchr(InPath, DIR_SEPARATOR_CHAR);
	if (p == NULL) {
		p = InPath;
	}
	else {
		p++;
	}
	return p;

}

SCPPath SCPPath::AddExtensionIfMissing(SCP_string Filename, SCP_string ExtensionToAdd) 
{
	SCPPath TmpFilePath(Filename);
	if (!TmpFilePath.has_extension())
	{
		TmpFilePath.replace_extension(ExtensionToAdd);
	}
	return TmpFilePath;
}

bool SCPPath::CreateDirectories(SCPPath DirectoryPath) 
{
	//TODO: @SCPPath make this more robust
	if (DirectoryPath.has_extension())
	{
		return ghc::filesystem::create_directories(DirectoryPath.remove_filename());
	}
	else
	{
		return ghc::filesystem::create_directories(DirectoryPath);
	}
	
}
