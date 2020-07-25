#pragma once
#include <memory>
#include <string>
#include <atomic>
#include <vector>


class SCPModuleBase
{
	static int GetNextID()
	{
		static std::atomic_int LastTypeId(0);
		LastTypeId++;
		return LastTypeId;
	}
public:
	
	virtual ~SCPModuleBase() = 0;

    virtual bool StartupModule() = 0;
    virtual void ShutdownModule() = 0;
	
	virtual int GetModuleID() = 0;

	template<typename T>
	static int GetIDForModule()
	{
		static const int id = GetNextID();
		return id;
	}

	template<typename T, typename... Deps>
	T ConstructModule(Deps... Dependencies) {};
};

template<typename T>
class SCPModule : public SCPModuleBase
{
	int GetModuleID() override
	{
		return SCPModuleBase::GetIDForModule<T>();
	}
};


