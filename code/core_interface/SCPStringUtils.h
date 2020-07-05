#pragma once
#include "FSStdTypes.h"

namespace StringUtils
{
	SCP_string ToLower(const SCP_string& InString)
	{
		SCP_string LoweredString;
		std::transform(InString.begin(), InString.end(), LoweredString.begin(), [](unsigned char c) {return (char) std::tolower(c); });
		return LoweredString;
	}
	void MakeLower(SCP_string& InString)
	{
		std::transform(InString.begin(), InString.end(), InString.begin(), [](unsigned char c) { return (char) std::tolower(c); });
	}
	SCP_string TrimWhitespace(const SCP_string& InString)
	{
		static const char* WhitespaceCharacters = " \t\n\r\f\v";
		SCP_string Result = InString;
		Result.erase(Result.find_last_not_of(WhitespaceCharacters) + 1);
		Result.erase(0, Result.find_first_not_of(WhitespaceCharacters));
		return Result;
	}
}