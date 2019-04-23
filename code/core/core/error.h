#pragma once
namespace core {
void Warning(const char* filename, int line, const char* format, ...);
void WarningEx(const char* filename, int line, const char* format, ...);

void ReleaseWarning(const char* filename, int line, const char* format, ...);

void Error(const char* filename, int line, const char* format, ...);
void Error(const char* msg);

#if defined(NDEBUG)
#define Assert(expr)                                                                                                   \
	do {                                                                                                               \
		ASSUME(expr);                                                                                                  \
	} while (false)
#else
void AssertImpl(const char* expr, const char* file, int line);
#define Assert(expr)                                                                                                   \
	do {                                                                                                               \
		if (!(expr)) {                                                                                                 \
			core::AssertImpl(#expr, __FILE__, __LINE__);                                                               \
		}                                                                                                              \
		ASSUME(expr);                                                                                                  \
	} while (false)
#endif

// To debug printf do this:
// mprintf(( "Error opening %s\n", filename ));
void mprintf(const char* format, ...);
void nprintf(const char* id, const char* format, ...);

template <typename T>
extern void Verify(T x, const char* msg = nullptr);
} // namespace core
