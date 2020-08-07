#include "FSStdTypes.h"
#include "cfile/SCPCFileModule.h"
#include "cfile/SCPCFileInfo.h"
#include "color/SCPColorModule.h"
#include "color/SCPColorTable.h"
#include "color/SCPColorTableDescriptor.h"
#include "module/SCPModuleManager.h"
#include "parse/SCPParser.h"


bool SCPColorModule::StartupModule() 
{
	Colors = std::make_unique<SCPColorSet>(SCPColorSet::DefaultColors());
	auto CFileModule = SCPModuleManager::GetModule<SCPCFileModule>();
	Assert(CFileModule);
	auto ColorsTable = CFileModule->FindFileInfo("colors.tbl", SCPCFilePathTypeID::Tables);
	if (ColorsTable.has_value()) 
	{
		auto RootTable                  = CFileModule->CFileOpen(*ColorsTable, {SCPCFileMode::Read});
		SCP_buffer RootColorTableBuffer = RootTable->ReadAllContentsIntoBuffer();
		SCPParser::LoadIntoTable(*Colors, ColorsFile, std::move(RootColorTableBuffer), "colors.tbl");
	}

	FileFilter ColorTableFilter;
	ColorTableFilter.FilenameMatchesRegex("*-clr.tbm");
	ColorTableFilter.PathTypeIs(SCPCFilePathTypeID::Tables);
	for (SCPCFileInfo FileInfo : CFileModule->CFileDatabase().Files(ColorTableFilter)) {
		auto Table = CFileModule->CFileOpen(FileInfo, {SCPCFileMode::Read});
		if (Table) {
			SCP_buffer TableBuffer = Table->ReadAllContentsIntoBuffer();
			SCPParser::LoadIntoTable(*Colors, ColorsFile, std::move(TableBuffer), FileInfo.GetFileName());
		}
	}
}
