#pragma once
namespace core
{
	void Warning(const char* filename, int line, const char* format, ...);
	void WarningEx(const char* filename, int line, const char* format, ...);

	void ReleaseWarning(const char* filename, int line, const char* format, ...);

	
	void Error(const char* filename, int line, const char* format, ...);
	void Error(const char* msg);
	
	template <typename T>
	extern void Verify(T x, const char* msg = nullptr);
    }
