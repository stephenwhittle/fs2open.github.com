#pragma once
#include <string>
namespace core
{

		// Goober5000
	// yoinked gratefully from dbugfile.cpp
	void vsprintf(std::string& dest, const char* format, va_list ap);

	void sprintf(std::string& dest, const char* format, ...);

}

