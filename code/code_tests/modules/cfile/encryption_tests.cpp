#include "cfile/SCPCFileDatabase.h"
#include "cfile/SCPCFileInfo.h"
#include "cfile/SCPCFileModule.h"
#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"
#include <algorithm>

TEST_CASE("Encryption" * doctest::test_suite("CFile Module"))
{
	auto EncryptionModuleHandle = SCPModuleManager::GetModule<SCPCFileModule>();
	SUBCASE("Encrypted file decryption")
	{
		auto EncryptedFileInfo =
			EncryptionModuleHandle->FindFileInfo("weapons_encrypted.tbl", SCPCFilePathTypeID::Tables);
		REQUIRE(EncryptedFileInfo);
		auto CFile = EncryptionModuleHandle->CFileOpen(EncryptedFileInfo.value(), { SCPCFileMode::Read });
			auto FileEncryptionType = CFile->DetectFileEncryption();
			REQUIRE(FileEncryptionType != CFILE::CFileEncryptionMagic::NotEncrypted);
			//perform decryption and compare the results here
			SCP_buffer EncryptedFileBuffer = SCP_buffer(CFile->GetSize());
			CFile->ReadBytes(EncryptedFileBuffer.Data(), EncryptedFileBuffer.Size);

			CFile->Decrypt(EncryptedFileBuffer, FileEncryptionType);

		auto NonEncryptedFileInfo =
			EncryptionModuleHandle->FindFileInfo("weapons.tbl", SCPCFilePathTypeID::Tables);
		REQUIRE(NonEncryptedFileInfo);
		auto PlainFile = EncryptionModuleHandle->CFileOpen(NonEncryptedFileInfo.value(), { SCPCFileMode::Read });
		SCP_buffer PlainFileBuffer = SCP_buffer(PlainFile->GetSize());
		PlainFile->ReadBytes(PlainFileBuffer.Data(), PlainFileBuffer.Size);
		REQUIRE(std::equal(PlainFileBuffer.begin(), PlainFileBuffer.end(), EncryptedFileBuffer.begin()));
	}
}