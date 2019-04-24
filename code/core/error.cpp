#include <SimpleSignal.h>
#include <core/error.h>
#include <core/format.h>
#include <core/path.h>
#include <dbgtools/callstack.h>

#include <sstream>

extern int Global_warning_count;
extern int Global_error_count;
extern int Cmdline_extra_warn;

namespace core {
Simple::Signal<void(const char*)> error_handler;
Simple::Signal<void(const char*, int line, const char* msg)> warning_handler;
Simple::Signal<void(const char* msg, const char*, int line)> assert_handler;
Simple::Signal<void(const char* id, const char* msg)> console_output_handler;
void RegisterErrorHandler(std::function<void(const char*)> cb) { error_handler.connect(cb); }

void RegisterWarningHandler(std::function<void(const char*, int, const char*)> cb) { warning_handler.connect(cb); }

void RegisterAssertHandler(std::function<void(const char*, const char*, int)> cb) { assert_handler.connect(cb); }

void RegisterConsoleOutputHandler(std::function<void(const char*, const char*)> cb)
{
	console_output_handler.connect(cb);
}

void Warning(const char* filename, int line, const char* format, ...)
{
	Global_warning_count++;

#ifndef NDEBUG
	std::string msg;
	va_list args;

	va_start(args, format);
	core::vsprintf(msg, format, args);
	va_end(args);

	warning_handler.emit(filename, line, msg.c_str());
#endif
}
void WarningEx(const char* filename, int line, const char* format, ...)
{
#ifndef NDEBUG
	if (Cmdline_extra_warn) {
		std::string msg;
		va_list args;

		va_start(args, format);
		core::vsprintf(msg, format, args);
		va_end(args);

		warning_handler.emit(filename, line, msg.c_str());
	}
#endif
}

void ReleaseWarning(const char* filename, int line, const char* format, ...)
{
	Global_warning_count++;

	std::string msg;
	va_list args;

	va_start(args, format);
	core::vsprintf(msg, format, args);
	va_end(args);

	warning_handler.emit(filename, line, msg.c_str());
}

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

// todo: return formatted callstack with the error
void Error(const char* msg) { error_handler.emit(msg); }

void AssertImpl(const char* expr, const char* file, int line) { assert_handler.emit(expr, file, line); }

void mprintf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	core::nprintf("General", format, args);
	va_end(args);
}

void nprintf(const char* id, const char* format, ...)
{
	std::string formatString;
	va_list args;
	va_start(args, format);
	core::vsprintf(formatString, format, args);
	va_end(args);
	console_output_handler.emit(id, formatString.c_str());
}

std::string GetFormattedCallstack()
{
	void* addresses[256];
	int num_addresses = callstack(0, addresses, 256);

	callstack_symbol_t symbols[256];
	char symbols_buffer[2048];
	num_addresses = callstack_symbols(addresses, symbols, num_addresses, symbols_buffer, 2048);

	std::string CallstackString;
	std::string LineString;
	int i;
	// only print 10 lines max
	for (i = 0; (i < 10) && (i < num_addresses); ++i) {
		core::sprintf(LineString, "%s %s(%u)\n", symbols[i].function, symbols[i].file, symbols[i].line);
		CallstackString += LineString;
	}
	return CallstackString;
}

template <typename T>
void Verify(T x, const char* msg)
{
	if (!(x)) {
		if (msg != nullptr) {
			error_handler.emit(msg);
		} else {
			std::string formatText;
			core::sprintf(formatText, "Verify failure!\n %s", GetFormattedCallstack().c_str());
			error_handler.emit(formatText.c_str());
		}
	}
}
template void Verify<bool>(bool x, const char* msg);
} // namespace core
