#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"
#include "cfile/SCPCFileModule.h"
#include "cfile/SCPCFileDatabase.h"
#include "cfile/SCPCFileInfo.h"

TEST_CASE("Encoding" * doctest::test_suite("CFile Module"))
{
	auto EncodingModuleHandle = SCPModuleManager::GetModule<SCPCFileModule>();
	SUBCASE("Encrypted file")
	{
		auto EncryptedFileInfo = EncodingModuleHandle->FindFileInfo("weapons_encrypted.tbl", SCPCFilePathTypeID::Tables);
		if (EncryptedFileInfo)
		{
			auto CFile = EncodingModuleHandle->CFileOpen(EncryptedFileInfo.value(), { SCPCFileMode::Read });
			REQUIRE(CFile->DetectFileEncryption() != CFILE::CFileEncryptionMagic::NotEncrypted);
		}
	}
	SUBCASE("Handle UTF8")
	{
		auto UTF8FileInfo = EncodingModuleHandle->FindFileInfo("utf8.tbl", SCPCFilePathTypeID::Tables);
		if (UTF8FileInfo)
		{
			auto CFile = EncodingModuleHandle->CFileOpen(UTF8FileInfo.value(), { SCPCFileMode::Read });
			SCP_buffer FileBuffer = SCP_buffer(CFile->GetSize());
			CFile->ReadBytes(FileBuffer.Data(), FileBuffer.Size);
			REQUIRE(CFile->DetectFileEncoding(FileBuffer.Data(), FileBuffer.Size) == CFILE::CFileTextEncoding::UTF8);
		}
	}
	SUBCASE("Handle UTF8 BOM")
	{
		auto UTF8FileInfo = EncodingModuleHandle->FindFileInfo("utf8bom.tbl", SCPCFilePathTypeID::Tables);
		if (UTF8FileInfo) {
			auto CFile = EncodingModuleHandle->CFileOpen(UTF8FileInfo.value(), {SCPCFileMode::Read});
			SCP_buffer FileBuffer = SCP_buffer(CFile->GetSize());
			CFile->ReadBytes(FileBuffer.Data(), FileBuffer.Size);
			REQUIRE(CFile->DetectFileEncoding(FileBuffer.Data(), FileBuffer.Size) == CFILE::CFileTextEncoding::UTF8BOM);
		}
	}
	SUBCASE("Handle Windows/Latin")
	{
		auto UTF8FileInfo = EncodingModuleHandle->FindFileInfo("latin.tbl", SCPCFilePathTypeID::Tables);
		if (UTF8FileInfo)
		{
			auto CFile = EncodingModuleHandle->CFileOpen(UTF8FileInfo.value(), { SCPCFileMode::Read });
			SCP_buffer FileBuffer = SCP_buffer(CFile->GetSize());
			CFile->ReadBytes(FileBuffer.Data(), FileBuffer.Size);
			REQUIRE(CFile->DetectFileEncoding(FileBuffer.Data(), FileBuffer.Size) == CFILE::CFileTextEncoding::WindowsLatin1);
		}
	}
	SUBCASE("Handle Unknown UTF encodings")
	{
		auto UTF8FileInfo = EncodingModuleHandle->FindFileInfo("utf32be.tbl", SCPCFilePathTypeID::Tables);
		if (UTF8FileInfo)
		{
			auto CFile = EncodingModuleHandle->CFileOpen(UTF8FileInfo.value(), { SCPCFileMode::Read });
			CFile->UTF8Normalize();
		}
		auto UTF16FileInfo = EncodingModuleHandle->FindFileInfo("utf16le.tbl", SCPCFilePathTypeID::Tables);
		if (UTF16FileInfo)
		{
			auto CFile = EncodingModuleHandle->CFileOpen(UTF16FileInfo.value(), { SCPCFileMode::Read });
			CFile->UTF8Normalize();
		}
	}
	SUBCASE("Handle ASCII")
	{
		auto UTF8FileInfo = EncodingModuleHandle->FindFileInfo("ascii.tbl", SCPCFilePathTypeID::Tables);
		if (UTF8FileInfo)
		{
			auto CFile = EncodingModuleHandle->CFileOpen(UTF8FileInfo.value(), { SCPCFileMode::Read });
			CFile->UTF8Normalize();
		}
	}
}