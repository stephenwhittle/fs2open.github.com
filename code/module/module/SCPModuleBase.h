#include <memory>
#include <string>
#include <map>
#include <atomic>
#include "tl/optional.hpp"
#include "SCPTUtils.h"

//mock cmdline options container
class SCPCmdlineOptions {
    public:
    std::string Mods;
};

class SCPModuleBase
{
    public:

    virtual void StartupModule() = 0;
    virtual void ShutdownModule() = 0;
};

template<typename T>
auto GetDependencies()
{
	return std::tuple<>{};
}

class SCPCFileModule : public SCPModuleBase
{
    public:
    virtual void StartupModule() override
    {

    }
    virtual void ShutdownModule() override {}
};


class SCPCmdlineModule : public SCPModuleBase
{
public:

    virtual void StartupModule() override {}

    virtual void ShutdownModule() override {}
    std::unique_ptr<SCPCmdlineOptions> CurrentOptions;
};

template<>
auto GetDependencies<SCPCFileModule>()
{
    return std::tuple<SCPCmdlineModule>();
}

class SCPModuleManager
{
    static std::map<int, std::unique_ptr<SCPModuleBase>> Modules;

	template <class Key>
	inline static int getTypeId()
	{
		static const int id = LastTypeId++;
		return id;
	}

	static std::atomic_int LastTypeId;

	template<typename First, typename... Rest>
	static bool CheckDependencies(std::tuple<First, Rest...> DependencyList)
	{
		tl::optional<First> ModuleRef = GetModule<First>();
		if (ModuleRef.has_value())
		{
			return CheckDependencies(std::tuple<Rest...>());
		}
		else
		{
			return false;
		}
	}
	template<typename First>
	static bool CheckDependencies(std::tuple<First> DependencyList)
	{
		tl::optional<First> ModuleRef = GetModule<First>();
		if (ModuleRef.has_value()) 
		{
			return true;
		} else 
		{
			return false;
		}
	}

	static bool CheckDependencies(std::tuple<> EmptyDependencyList)
	{
		return true;
	}
	
	template<typename T, typename SCPRestrictToChildren < SCPModuleBase, T> = nullptr>
	static std::unique_ptr< T> CreateModule()
	{
		if (CheckDependencies(GetDependencies<T>()))
		{
			return std::make_unique<T>();
		}
		else
		{
			return {};
		}
	}

    public:
    template<typename T, typename SCPRestrictToChildren < SCPModuleBase, T> = nullptr>
    static tl::optional<T&> GetModule()
    {
		auto ModuleID = getTypeId<T>();

		auto ModuleIterator = Modules.find(ModuleID);
		if (ModuleIterator != Modules.end() && ModuleIterator->second.get() != nullptr)
		{
			//get the module unique_ptr, get non-owning raw ptr, cast to derived*, dereference
			return tl::optional<T&>(*(static_cast<T*>(ModuleIterator->second.get())));
		}
		else
		{
			std::unique_ptr<T> ModulePtr = CreateModule<T>();
			if (ModulePtr != nullptr)
			{
				Modules[ModuleID] = std::move(ModulePtr);
				return *ModulePtr;
			}
			else
			{
				return {};
			}
		}
    }
};