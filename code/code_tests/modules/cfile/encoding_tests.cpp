#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"
#include "cfile/SCPCFileModule.h"
#include "cfile/SCPCFileDatabase.h"
#include "cfile/SCPCFileInfo.h"

TEST_CASE("Encoding" * doctest::test_suite("CFile Module"))
{
	auto ModuleHandle = SCPModuleManager::GetModule<SCPCFileModule>();
	SUBCASE("Handle UTF8")
	{
		auto UTF8FileInfo = ModuleHandle->FindFileInfo("utf8.tbl", SCPCFilePathTypeID::Tables);
		if (UTF8FileInfo)
		{
			auto CFile = ModuleHandle->CFileOpen(UTF8FileInfo.value(), { SCPCFileMode::Read });
			CFile->UTF8Normalize();
		}
	}
	SUBCASE("Handle UTF8 BOM")
	{
		auto UTF8FileInfo = ModuleHandle->FindFileInfo("utf8bom.tbl", SCPCFilePathTypeID::Tables);
		if (UTF8FileInfo) {
			auto CFile = ModuleHandle->CFileOpen(UTF8FileInfo.value(), {SCPCFileMode::Read});
			CFile->UTF8Normalize();
		}
	}
	SUBCASE("Handle Windows/Latin")
	{
		auto UTF8FileInfo = ModuleHandle->FindFileInfo("latin.tbl", SCPCFilePathTypeID::Tables);
		if (UTF8FileInfo)
		{
			auto CFile = ModuleHandle->CFileOpen(UTF8FileInfo.value(), { SCPCFileMode::Read });
			CFile->UTF8Normalize();
		}
	}
	SUBCASE("Handle Unknown UTF encodings")
	{
		auto UTF8FileInfo = ModuleHandle->FindFileInfo("utf32be.tbl", SCPCFilePathTypeID::Tables);
		if (UTF8FileInfo)
		{
			auto CFile = ModuleHandle->CFileOpen(UTF8FileInfo.value(), { SCPCFileMode::Read });
			CFile->UTF8Normalize();
		}
		auto UTF16FileInfo = ModuleHandle->FindFileInfo("utf16le.tbl", SCPCFilePathTypeID::Tables);
		if (UTF16FileInfo)
		{
			auto CFile = ModuleHandle->CFileOpen(UTF16FileInfo.value(), { SCPCFileMode::Read });
			CFile->UTF8Normalize();
		}
	}
	SUBCASE("Handle ASCII")
	{
		auto UTF8FileInfo = ModuleHandle->FindFileInfo("ascii.tbl", SCPCFilePathTypeID::Tables);
		if (UTF8FileInfo)
		{
			auto CFile = ModuleHandle->CFileOpen(UTF8FileInfo.value(), { SCPCFileMode::Read });
			CFile->UTF8Normalize();
		}
	}
}