#include <ghc/fs_impl.hpp>
#include "SCPPath.h"
#include "SCPStringUtils.h"
#include <algorithm>

bool SCPPath::Compare(const SCPPath& Left, const SCPPath& Right, bool CaseSensitive /*= false*/) 
{
	if (CaseSensitive)
	{
		return Left.string() == Right.string();
	}
	else
	{
		return StringUtils::ToLower(Left.string()) == StringUtils::ToLower(Right.string());
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