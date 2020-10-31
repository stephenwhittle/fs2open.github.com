#include "cfile/SCPCFileDatabase.h"
#include "cfile/SCPCFileInfo.h"
#include "cfile/SCPCFileModule.h"
#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"
#include <algorithm>
#include <fstream>
TEST_CASE("Encryption" * doctest::test_suite("CFile Module"))
{
	auto EncryptionModuleHandle = SCPModuleManager::GetModule<SCPCFileModule>();
	SUBCASE("Encrypted file decryption")
	{
		std::vector<std::string> FileNames = {"ai",       "asteroid", "fireball", "icons", "medals",
											  "messages", "music",    "rank",     "ships", "sounds",
											  "tooltips", "traitor",  "weapons"};
		for (std::string Filename : FileNames)
		{
			auto EncryptedFileInfo =
				EncryptionModuleHandle->FindFileInfo(Filename + "_encoded.tbl", SCPCFilePathTypeID::Tables);
			REQUIRE(EncryptedFileInfo);
			auto CFile = EncryptionModuleHandle->CFileOpen(EncryptedFileInfo.value(), {SCPCFileMode::Read});
			auto FileEncryptionType = CFile->DetectFileEncryption();
			REQUIRE(FileEncryptionType != CFILE::CFileEncryptionMagic::NotEncrypted);
			// perform decryption and compare the results here
			
			//TODO: @CFile implement DecryptWholeFile
			SCP_buffer EncryptedFileBuffer = SCP_buffer(CFile->GetSize() - 4);
			CFile->SeekAbsolute(4);
			CFile->ReadBytes(EncryptedFileBuffer.Data(), EncryptedFileBuffer.Size);

			CFile->DecryptBuffer(EncryptedFileBuffer, FileEncryptionType);
			
			auto NonEncryptedFileInfo =
				EncryptionModuleHandle->FindFileInfo(Filename + ".tbl", SCPCFilePathTypeID::Tables);
			REQUIRE(NonEncryptedFileInfo);
			
			auto PlainFile = EncryptionModuleHandle->CFileOpen(NonEncryptedFileInfo.value(), {SCPCFileMode::Read});
			SCP_buffer PlainFileBuffer = SCP_buffer(PlainFile->GetSize());
			PlainFile->ReadBytes(PlainFileBuffer.Data(), PlainFileBuffer.Size);
			
			auto Predicate = [](const char& A, const char& B) {
				if (A == B)
				{
					return true;
				}
				else
				{
					return false;
				}
			};
			std::size_t NumElementsToCompare = PlainFileBuffer.Size;
			if (PlainFileBuffer.Size != EncryptedFileBuffer.Size)
			{
				NumElementsToCompare = std::min(PlainFileBuffer.Size, EncryptedFileBuffer.Size);
			}
			REQUIRE(std::equal(PlainFileBuffer.begin(), PlainFileBuffer.begin() + NumElementsToCompare,
							   EncryptedFileBuffer.begin(), Predicate));
		}
	}
}