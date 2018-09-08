#ifndef ASSERTHANDLER_H
#define ASSERTHANDLER_H

#include "globalincs/toolchain.h"

class AssertHandler
{
	public:
    virtual void AssertMessage(const char* text, const char* filename, int linenum, SCP_FORMAT_STRING
						   const char* format = nullptr, ...)
		SCP_FORMAT_STRING_ARGS(4, 5) CLANG_ANALYZER_NORETURN = 0;
};


/*******************NEVER UNCOMMENT Assert ************************************************/
// Please never uncomment the functionality of Assert in debug
// The code, as with all development like this is littered with Asserts which are designed to throw
// up an error message if variables are out of range.
// Disabling this functionality is dangerous, crazy values can run rampent unchecked and the longer its disabled
// the more likely you are to have problems getting it working again.
#if defined(NDEBUG)
#	define Assert(expr) do { ASSUME(expr); } while (false)
#else
#	define Assert(expr) do {\
		if (!(expr)) {\
			gAssertHandler->AssertMessage(#expr,__FILE__,__LINE__);\
		}\
		ASSUME( expr );\
	} while (false)
#endif
/*******************NEVER COMMENT Assert ************************************************/


extern void outwnd_printf2(const char* format, ...);
extern void outwnd_printf(const char* id, const char* format, ...);

// To debug printf do this:
// mprintf(( "Error opening %s\n", filename ));
#ifndef NDEBUG
#define mprintf(args) outwnd_printf2 args
#define nprintf(args) outwnd_printf args
#else
#define mprintf(args)
#define nprintf(args)
#endif

#define LOCATION __FILE__,__LINE__

// To flag an error, you can do this:
// Error( __FILE__, __LINE__, "Error opening %s", filename );
// or,
// Error( LOCATION, "Error opening %s", filename );


#if defined(NDEBUG)
#if HAS_VARIADIC_MACROS == 1 /* MSVC 2005 or newer */
#define Assertion(expr, msg, ...)                                                                                      \
	do {                                                                                                               \
		ASSUME(expr);                                                                                                  \
	} while (false)
#else
#define Assertion(expr, msg)                                                                                           \
	do {                                                                                                               \
	} while (false)
#endif
#else
/*
 * NOTE: Assertion() can only use its proper functionality in compilers
 * that support variadic macros.
 */
#if HAS_VARIADIC_MACROS == 1 /* MSVC 2005 or newer */
#define Assertion(expr, msg, ...)                                                                                      \
	do {                                                                                                               \
		if (!(expr)) {                                                                                                 \
			gAssertHandler->AssertMessage(#expr, __FILE__, __LINE__, msg, __VA_ARGS__);                                   \
		}                                                                                                              \
	} while (false)
#else /* Older MSVC compilers */
#define Assertion(expr, msg)                                                                                           \
	do {                                                                                                               \
		if (!(expr)) {                                                                                                 \
			gAssertHandler->AssertMessage(#expr, __FILE__, __LINE__);                                                     \
		}                                                                                                              \
		while (false)
#endif
#endif



extern AssertHandler* gAssertHandler;
#endif