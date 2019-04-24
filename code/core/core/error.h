#pragma once
namespace core {
void Warning(const char* filename, int line, const char* format, ...);
void WarningEx(const char* filename, int line, const char* format, ...);

void ReleaseWarning(const char* filename, int line, const char* format, ...);

void Error(const char* filename, int line, const char* format, ...);
void Error(const char* msg);



// To debug printf do this:
// mprintf(( "Error opening %s\n", filename ));
void mprintf(const char* format, ...);
void nprintf(const char* id, const char* format, ...);
inline void noop() { void(0); };
void AssertImpl(bool expr, const char* expression, const char* file, int line, const char* msg);

void AssertImplWithMessage(bool expr, const char* expression, const char* file, int line, const char* format, ...);

template <typename T>
extern void Verify(T x, const char* msg = nullptr);
} // namespace core

#if defined(NDEBUG)
#define Assert(expr) noop()
#define Assertion(expr) noop()
#else
#define Assert(expr) AssertImpl((bool)(expr), #expr, __FILE__, __LINE__, nullptr)
#define Assertion(expr, format, ...) AssertImplWithMessage((bool)(expr), #expr, __FILE__, __LINE__, format, __VA_ARGS__)
#endif