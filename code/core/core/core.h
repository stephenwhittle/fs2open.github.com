#pragma once
#include <functional>
namespace core 
{
	extern void RegisterErrorHandler(std::function<void(const char*)> cb);
	extern void RegisterWarningHandler(std::function<void(const char*, int, const char*)> cb);
    extern void RegisterAssertHandler(std::function<void(const char*, const char*, int)> cb);
}
