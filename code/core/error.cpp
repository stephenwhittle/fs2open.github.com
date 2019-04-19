#include "error.h"
#include <SimpleSignal.h>
#include <globalincs/pstypes.h>
#include "format.h"
#include "path.h"
namespace core 
{
	Simple::Signal<void(const char*)> error_handler;
	void Error(const char* filename, int line, const char* format, ...)
	{
		std::string formatText;
		filename = core::path::clean_filename(filename);

		va_list args;
		va_start(args, format);
		core::vsprintf(formatText, format, args);
		va_end(args);

		std::stringstream messageStream;
		messageStream << "Error: " << formatText << "\n";
		messageStream << "File: " << filename << "\n";
		messageStream << "Line: " << line << "\n";

		error_handler.emit(messageStream.str().c_str());
	}

	void Error(const char* msg) 
	{
		error_handler.emit(msg);
	}

    template <typename T>
    void Verify(T x)
    {
	    if (!(x)) {

		    Error(LOCATION, "Verify failure: %s\n", x);
	    }
	    ASSUME(x);
    }
	template void Verify<bool>(bool x);
} // namespace core
