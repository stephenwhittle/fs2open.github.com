#pragma once
#include "cfile/SCPCFileDatabase.h"
#include "cfile/SCPCFileInfo.h"
#include "cfile/SCPCFileModule.h"
#include "def_files/def_files.h"
#include "module/SCPModuleBase.h"
#include "module/SCPModuleManager.h"
#include "parse/SCPParser.h"
#include "species/SCPSpeciesDefaultsTable.h"
#include "species/SCPSpeciesDefaultsTableDescriptor.h"

namespace SCP
{
	class SpeciesDefaultsModule : public SCPModule<SpeciesDefaultsModule>
	{
	public:
		bool StartupModule() override
		{
			auto CFileModule = SCPModuleManager::GetModule<SCPCFileModule>();
			Assert(CFileModule);
			auto BaseTable = CFileModule->FindFileInfo("species_defs.tbl", SCPCFilePathTypeID::Tables);
			if (BaseTable)
			{
				std::unique_ptr<CFILE> SpeciesDefsTbl =
					CFileModule->CFileOpen(*BaseTable, {SCPCFileMode::Binary, SCPCFileMode::Read});
				SCP_buffer RootSpeciesDefsTblBuffer = SpeciesDefsTbl->LoadAsText();
				LoadedSpeciesDefaults = SCPParser::CreateBaseTable<SCP::SpeciesDefaultsTable>(
					SCP::SpeciesDefaultsFile, std::move(RootSpeciesDefsTblBuffer), "species_defs.tbl");
			}
			else
			{
				default_file DefaultSpeciesFile = defaults_get_file("species_defs.tbl");
				SCP_buffer SpeciesDefaultsData(DefaultSpeciesFile.size);
				std::copy((uint8_t*) DefaultSpeciesFile.data,
						  (uint8_t*) DefaultSpeciesFile.data + DefaultSpeciesFile.size, SpeciesDefaultsData.begin());
				LoadedSpeciesDefaults = SCPParser::CreateBaseTable<SCP::SpeciesDefaultsTable>(
					SCP::SpeciesDefaultsFile, std::move(SpeciesDefaultsData), "species_defs.tbl");
			}

			FileFilter SpeciesTableFilter;
			SpeciesTableFilter.FilenameMatchesRegex(".*-sdf\\.tbm");
			SpeciesTableFilter.PathTypeIs(SCPCFilePathTypeID::Tables);
			for (SCPCFileInfo FileInfo : CFileModule->CFileDatabase().Files(SpeciesTableFilter)) {
				auto TableFile = CFileModule->CFileOpen(FileInfo, {SCPCFileMode::Read});
				if (TableFile) {
					SCP_buffer TableBuffer = TableFile->LoadAsText();
					SCPParser::LoadIntoTable(*LoadedSpeciesDefaults, SCP::SpeciesDefaultsFile, std::move(TableBuffer),
											 FileInfo.GetFileName());
				}
			}

			return true;
		}

		void ShutdownModule() override {}
		static SpeciesDefaultsModule ConstructModule(SCPCFileModule&)
		{
			return SpeciesDefaultsModule();
		}

	private:
		tl::optional<SCP::SpeciesDefaultsTable> LoadedSpeciesDefaults;
	};
} // namespace SCP