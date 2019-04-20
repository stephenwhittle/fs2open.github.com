#include "format.h"
#include <cstdarg>
#include <regex>
namespace core
{
	void vsprintf(std::string& dest, const char* format, va_list ap)
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

	void sprintf(std::string& dest, const char* format, ...)
    {
	    va_list args;
	    va_start(args, format);
	    vsprintf(dest, format, args);
	    va_end(args);
    }
    std::vector<std::string> SplitStrings(const std::string& s, const std::string& delim)
    {
	    std::vector<std::string> Tokens;
	    std::regex Delimiter(delim);
	    std::copy(std::sregex_token_iterator(s.begin(), s.end(), Delimiter, -1), std::sregex_token_iterator(),
	              std::back_inserter(Tokens));
	    return Tokens;
    }

}
