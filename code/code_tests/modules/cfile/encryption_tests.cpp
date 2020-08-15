#include "cfile/SCPCFileDatabase.h"
#include "cfile/SCPCFileInfo.h"
#include "cfile/SCPCFileModule.h"
#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"

TEST_CASE("Encryption" * doctest::test_suite("CFile Module"))
{
	auto EncodingModuleHandle = SCPModuleManager::GetModule<SCPCFileModule>();
	SUBCASE("Encrypted file decryption")
	{
		auto EncryptedFileInfo =
			EncodingModuleHandle->FindFileInfo("weapons_encrypted.tbl", SCPCFilePathTypeID::Tables);
		if (EncryptedFileInfo) {
			auto CFile = EncodingModuleHandle->CFileOpen(EncryptedFileInfo.value(), { SCPCFileMode::Read });
			REQUIRE(CFile->DetectFileEncryption() != CFILE::CFileEncryptionMagic::NotEncrypted);
			//perform decryption and compare the results here

		}
	}
}