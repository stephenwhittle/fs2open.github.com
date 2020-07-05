#pragma once
#include "SCPTUtils.h"
#include <map>
#include <memory>
#include <tuple>
#include "tl/optional.hpp"
#include "module/SCPModuleBase.h"

template <typename... Ts>
struct ModuleDependencyList {
};

class SCPModuleManager
{
	static std::map<int, std::unique_ptr<SCPModuleBase>> Modules;

	static bool IsModuleLoaded(int ModuleID)
	{
		auto ModuleIterator = Modules.find(ModuleID);
		return (ModuleIterator != Modules.end() && ModuleIterator->second.get() != nullptr);
	}

	template<typename T, typename... Deps>
	static auto DeduceDependencies(T(*Constructor)(Deps...))
	{
		return ModuleDependencyList<std::remove_reference_t<Deps>...>{};
	}
	template<typename T>
	static auto DeduceDependencies(T(*Constructor)())
	{
		return ModuleDependencyList<>{};
	}

	template<typename Dependency, typename... Rest>
	static bool CheckDependencies(ModuleDependencyList<Dependency, Rest...> Dependencies)
	{
		auto ModuleID = SCPModuleBase::GetIDForModule<Dependency>();
		if (IsModuleLoaded(ModuleID)) 
		{
			return CheckDependencies(ModuleDependencyList<Rest...>());
		} 
		else 
		{
			auto DependencyInstance = CreateModule<Dependency>();
			if (DependencyInstance != nullptr) {
				Modules[ModuleID] = std::move(DependencyInstance);
				return CheckDependencies(ModuleDependencyList<Rest...>());
			} else {
				return false;
			}
		}
	}

	template <typename Dependency>
	static bool CheckDependencies(ModuleDependencyList<Dependency> Dependencies)
	{
		auto ModuleID = SCPModuleBase::GetIDForModule<Dependency>();
		if (IsModuleLoaded(ModuleID))
		{
			return true;
		}
		else
		{
			auto DependencyInstance = CreateModule<Dependency>();
			if (DependencyInstance != nullptr) 
			{
				Modules[ModuleID] = std::move(DependencyInstance);
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	
	static bool CheckDependencies(ModuleDependencyList<>)
	{
		return true;
	}

	template<typename T, typename... Deps>
	static T ConstructModuleWithDeps(ModuleDependencyList<Deps...>)
	{
		return T::ConstructModule(*GetModule<Deps>()...);
	}

	template<typename T, typename SCPRestrictToChildren < SCPModuleBase, T> = nullptr>
	static std::unique_ptr< T> CreateModule()
	{
		auto Dependencies = DeduceDependencies(&T::ConstructModule);

		if (CheckDependencies(Dependencies))
		{
			return std::make_unique<T>(std::move(ConstructModuleWithDeps<T>(Dependencies)));
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
		auto ModuleID = SCPModuleBase::GetIDForModule<T>();

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

