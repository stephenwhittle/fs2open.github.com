#pragma once
#include "module/SCPModuleManager.h"
#include "module/SCPModuleBase.h"
#include "cfile/SCPCFileModule.h"
#include "cfile/SCPCFileDatabase.h"
#include "cfile/SCPCFileInfo.h"
#include "def_files/def_files.h"
#include "parse/SCPParser.h"

namespace SCP
{
	class SpeciesDefaultsModule : SCPModuleBase<SpeciesDefaultsModule>
	{

public:
		bool StartupModule() override
		{
			auto CFileModule = SCPModuleManager::GetModule<SCPCFileModule>();
			Assert(CFileModule);
			auto BaseTable = CFileModule->FindFileInfo("species_defs.tbl", SCPCFilePathTypeID::Tables);
			if (BaseTable) {

			}
			else
			{
				default_file DefaultSpeciesFile = defaults_get_file("species_defs.tbl");
				
			}


			return true;
		}

		void ShutdownModule() override
		{
			
		}
		static SpeciesDefaultsModule ConstructModule(SCPCFileModule&) {
			return SpeciesDefaultsModule();
		}
	};
}