#pragma once

#include "FSOutputDeviceBase.h"

extern int Global_warning_count;
extern int Global_error_count;

// To debug printf do this:
// mprintf(( "Error opening %s\n", filename ));
#ifndef NDEBUG
constexpr bool LoggingEnabled = true;
#else
#ifdef SCP_RELEASE_LOGGING
constexpr bool LoggingEnabled = true;
#else
constexpr bool LoggingEnabled = false;
#endif
#endif

#define mprintf(args) do { if (LoggingEnabled) { GOutputDevice->PrintGeneral args; } } while (false)
#define nprintf(args) do { if (LoggingEnabled) { GOutputDevice->Print args; } } while (false)

#define LOCATION __FILE__,__LINE__

// To flag an error, you can do this:
// Error( __FILE__, __LINE__, "Error opening %s", filename );
// or,
// Error( LOCATION, "Error opening %s", filename );

/*******************NEVER COMMENT Assert ************************************************/
// Please never comment the functionality of Assert in debug
// The code, as with all development like this is littered with Asserts which are designed to throw
// up an error message if variables are out of range.
// Disabling this functionality is dangerous, crazy values can run rampant unchecked, and the longer it's disabled
// the more likely you are to have problems getting it working again.
#if defined(NDEBUG)
#	define Assert(expr) do { } while (false)
#else
#	define Assert(expr) do {\
		if (!(expr)) {\
			GOutputDevice->AssertMessage(#expr,__FILE__,__LINE__);\
		}\
	} while (false)
#endif
/*******************NEVER COMMENT Assert ************************************************/

// Goober5000 - define Verify for use in both release and debug mode
#define Verify(x) do { if (!(x)){ Error(LOCATION, "Verify failure: %s\n", #x); } ASSUME(x); } while(false)

// Verification (like Assertion)
#ifndef _MSC_VER   // non MS compilers
#	define Verification(x, y, ...) do { if (!(x)) { Error(LOCATION, "Verify failure: %s with help text " #y "\n", #x, ##__VA_ARGS__); } ASSUME(x); } while(false)
#else
#	define Verification(x, y, ...) do { if (!(x)) { Error(LOCATION, "Verify failure: %s with help text " #y "\n", #x, __VA_ARGS__); } ASSUME(x); } while(false)
#endif

#if defined(NDEBUG)
	// No debug version of Int3
	#define Int3() do { } while (false)
#else
	void debug_int3(const char *file, int line);

	// Debug version of Int3
	#define Int3() debug_int3(__FILE__, __LINE__)
#endif	// NDEBUG
