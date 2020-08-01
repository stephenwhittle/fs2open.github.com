#pragma once
#include "FSAssert.h"
#include "module/SCPModuleBase.h"
#include "cfile/SCPCFile.h"
#include "cfile/SCPCFileModule.h"

enum class SCPLogType
{
	Multiplayer = 0,
	Events = 1
};

class SCPLogModule : public SCPModule<SCPLogModule>
{
	std::map < SCPLogType, std::unique_ptr<CFILE>> OpenLogFiles;
	const std::map<SCPLogType, std::string> LogFileNames
	{
		{SCPLogType::Multiplayer, "Multiplayer.log"},
		{SCPLogType::Events, "Events.log"}
	};
public:
	virtual bool StartupModule() override { return true; }
	virtual void ShutdownModule() override {}

	static SCPLogModule ConstructModule(class SCPCFileModule&)
	{
		return SCPLogModule();
	}

	template<typename... ArgTypes>
	void LogPrintF(SCPLogType LogType, const char* Format, const ArgTypes&... Args)
	{
		Assert(Format);
		if (OpenLogFiles.find(LogType) == OpenLogFiles.end())
		{
			auto CFileModule = SCPModuleManager::GetModule<SCPCFileModule>();
			SCP_string LogFilePath = CFileModule->GetDefaultFilePath(SCPCFilePathTypeID::Data, LogFileNames.at(LogType));

		}
	}
};