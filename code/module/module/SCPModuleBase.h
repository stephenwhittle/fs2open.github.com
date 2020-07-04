#include <memory>
#include <string>
#include <vector>
#include <map>
#include <optional>

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
    return;
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
    std::map<std::string, std::unique_ptr<SCPModuleBase>> Modules;
    public:
    template<typename T>
    std::optional<T&> GetModule(std::string ModuleName)
    {
        auto Dependencies = GetDependencies<T>();

    }
};