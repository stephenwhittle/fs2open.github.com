
#include "mod_table/SCPGameSettingsModule.h"
#include "FSStdTypes.h"
#include "cfile/SCPCFileModule.h"
#include "def_files/def_files.h"
#include "mod_table/SCPGameSettingsTable.h"
#include "mod_table/SCPGameSettingsTableDescriptor.h"
#include "module/SCPModuleManager.h"
#include "cfile/SCPCFileInfo.h"
#include "parse/SCPParser.h"
#include "FSAssert.h"
bool SCP::GameSettingsModule::StartupModule()
{
	auto DefaultSettings = defaults_get_file("game_settings.tbl");
	SCP_buffer SettingsData(DefaultSettings.size);
	std::copy((uint8_t*) DefaultSettings.data, (uint8_t*) DefaultSettings.data + DefaultSettings.size,
			  SettingsData.begin());
	LoadedSettings = SCPParser::CreateBaseTable<SCP::GameSettingsTable>(SCP::GameSettingsFile, std::move(SettingsData),
																		"game_settings.tbl");
	if (!LoadedSettings)
	{
		return false;
	}
	tl::optional<SCPCFileModule&> CFileModule = SCPModuleManager::GetModule<SCPCFileModule>();
	
	Assert(CFileModule);

	tl::optional<SCPCFileInfo> SettingsTblInfo = CFileModule->FindFileInfo("game_settings.tbl", SCPCFilePathTypeID::Tables);

	if (SettingsTblInfo)
	{
		std::unique_ptr<CFILE> SettingsTbl =
			CFileModule->CFileOpen(*SettingsTblInfo, {SCPCFileMode::Binary , SCPCFileMode::Read});
		SCP_buffer RootSettingsTableBuffer = SettingsTbl->LoadAsText();
		SCPParser::LoadIntoTable(*LoadedSettings, SCP::GameSettingsFile, std::move(RootSettingsTableBuffer), "game_settings.tbl");	
	}
	
	FileFilter SettingsTableFilter;
	SettingsTableFilter.FilenameMatchesRegex(".*-mod\.tbm");
	SettingsTableFilter.PathTypeIs(SCPCFilePathTypeID::Tables);
	for (SCPCFileInfo FileInfo : CFileModule->CFileDatabase().Files(SettingsTableFilter))
	{
		auto Table = CFileModule->CFileOpen(FileInfo, {SCPCFileMode::Read});
		if (Table)
		{
			SCP_buffer TableBuffer = Table->LoadAsText();
			SCPParser::LoadIntoTable(*LoadedSettings, SCP::GameSettingsFile, std::move(TableBuffer), FileInfo.GetFileName());
		}
	}
	return true;
}

SCP::GameSettingsModule SCP::GameSettingsModule::ConstructModule(SCPCFileModule& CFileDependency)
{
	return GameSettingsModule();
}
