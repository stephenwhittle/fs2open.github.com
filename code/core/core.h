#include <SimpleSignal.h>
#include <globalincs/vmallocator.h>
#include <globalincs/pstypes.h>

namespace core 
{
	Simple::Signal<void(const char*)> error_handler;

	const char* clean_filename(const char* filename)
    {
	    auto separator = strrchr(filename, DIR_SEPARATOR_CHAR);
	    if (separator != nullptr) {
		    filename = separator + 1;
	    }

	    return filename;
    }


	// Goober5000
    // yoinked gratefully from dbugfile.cpp
    void vsprintf(SCP_string& dest, const char* format, va_list ap)
    {
	    va_list copy;

#if defined(_MSC_VER) && _MSC_VER < 1800
	    // Only Visual Studio >= 2013 supports va_copy
	    // This isn't portable but should work for Visual Studio
	    copy = ap;
#else
	    va_copy(copy, ap);
#endif

	    int needed_length = vsnprintf(nullptr, 0, format, copy);
	    va_end(copy);

	    if (needed_length < 0) {
		    // Error
		    return;
	    }

	    dest.resize(static_cast<size_t>(needed_length));
	    vsnprintf(&dest[0], dest.size() + 1, format, ap);
    }

    void sprintf(SCP_string& dest, const char* format, ...)
    {
	    va_list args;
	    va_start(args, format);
	    vsprintf(dest, format, args);
	    va_end(args);
    }

	void Error(const char* filename, int line, const char* format, ...)
	{
		SCP_string formatText;
		filename = clean_filename(filename);

		va_list args;
		va_start(args, format);
		vsprintf(formatText, format, args);
		va_end(args);

		SCP_stringstream messageStream;
		messageStream << "Error: " << formatText << "\n";
		messageStream << "File: " << filename << "\n";
		messageStream << "Line: " << line << "\n";

		error_handler.emit(messageStream.str().c_str());
	}

}

using core::Error;
using core::vsprintf;
using core::sprintf;
using core::clean_filename;