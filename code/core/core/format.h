#pragma once
#include <vector>
#include <string>
namespace core
{

		// Goober5000
	// yoinked gratefully from dbugfile.cpp
	void vsprintf(std::string& dest, const char* format, va_list ap);

	void sprintf(std::string& dest, const char* format, ...);

	
	std::vector<std::string> SplitStrings(const std::string& s, const std::string& delim);

}

