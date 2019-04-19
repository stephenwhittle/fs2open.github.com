#pragma once

#include <globalincs/vmallocator.h>

namespace core
{

		// Goober5000
	// yoinked gratefully from dbugfile.cpp
	void vsprintf(SCP_string& dest, const char* format, va_list ap);

	void sprintf(SCP_string& dest, const char* format, ...);

}

