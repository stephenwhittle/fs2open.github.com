#include "cfile/SCPCFileDatabase.h"
#include "cfile/SCPCFileInfo.h"
#include "cfile/SCPCFileModule.h"
#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"

TEST_CASE("Encoding" * doctest::test_suite("CFile Module"))
{
	auto EncodingModuleHandle = SCPModuleManager::GetModule<SCPCFileModule>();
	SUBCASE("Encrypted file")
	{
		auto EncryptedFileInfo =
			EncodingModuleHandle->FindFileInfo("weapons_encrypted.tbl", SCPCFilePathTypeID::Tables);
		if (EncryptedFileInfo)
		{
			auto CFile = EncodingModuleHandle->CFileOpen(EncryptedFileInfo.value(), {SCPCFileMode::Read});
			REQUIRE(CFile->DetectFileEncryption() != CFILE::CFileEncryptionMagic::NotEncrypted);
		}
	}
	SUBCASE("Handle UTF8")
	{
		auto UTF8FileInfo = EncodingModuleHandle->FindFileInfo("utf8.tbl", SCPCFilePathTypeID::Tables);
		if (UTF8FileInfo)
		{
			auto CFile = EncodingModuleHandle->CFileOpen(UTF8FileInfo.value(), {SCPCFileMode::Read});
			SCP_buffer FileBuffer = SCP_buffer(CFile->GetSize());
			CFile->ReadBytes(FileBuffer.Data(), FileBuffer.Size);
			REQUIRE(CFile->DetectFileEncoding(FileBuffer.Data(), FileBuffer.Size) == CFILE::CFileTextEncoding::UTF8);
		}
	}
	SUBCASE("Handle UTF8 BOM")
	{
		auto UTF8FileInfo = EncodingModuleHandle->FindFileInfo("utf8bom.tbl", SCPCFilePathTypeID::Tables);
		if (UTF8FileInfo)
		{
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
			auto CFile = EncodingModuleHandle->CFileOpen(UTF8FileInfo.value(), {SCPCFileMode::Read});
			SCP_buffer FileBuffer = SCP_buffer(CFile->GetSize());
			CFile->ReadBytes(FileBuffer.Data(), FileBuffer.Size);
			REQUIRE(CFile->DetectFileEncoding(FileBuffer.Data(), FileBuffer.Size) ==
					CFILE::CFileTextEncoding::WindowsLatin1);
		}
	}
	SUBCASE("Handle Unknown UTF encodings")
	{
		auto UTF32FileInfo = EncodingModuleHandle->FindFileInfo("utf32be.tbl", SCPCFilePathTypeID::Tables);
		if (UTF32FileInfo)
		{
			auto CFile = EncodingModuleHandle->CFileOpen(UTF32FileInfo.value(), {SCPCFileMode::Read});
			SCP_buffer FileBuffer = SCP_buffer(CFile->GetSize());
			CFile->ReadBytes(FileBuffer.Data(), FileBuffer.Size);
			REQUIRE(CFile->DetectFileEncoding(FileBuffer.Data(), FileBuffer.Size) ==
					CFILE::CFileTextEncoding::UnsupportedUTF);
		}

		auto UTF16FileInfo = EncodingModuleHandle->FindFileInfo("utf16le.tbl", SCPCFilePathTypeID::Tables);
		if (UTF16FileInfo)
		{
			auto CFile = EncodingModuleHandle->CFileOpen(UTF16FileInfo.value(), {SCPCFileMode::Read});
			SCP_buffer FileBuffer = SCP_buffer(CFile->GetSize());
			CFile->ReadBytes(FileBuffer.Data(), FileBuffer.Size);
			REQUIRE(CFile->DetectFileEncoding(FileBuffer.Data(), FileBuffer.Size) ==
					CFILE::CFileTextEncoding::UnsupportedUTF);
		}
	}
	SUBCASE("Handle ASCII as valid UTF-8")
	{
		auto UTF8FileInfo = EncodingModuleHandle->FindFileInfo("ascii.tbl", SCPCFilePathTypeID::Tables);
		if (UTF8FileInfo)
		{
			auto CFile = EncodingModuleHandle->CFileOpen(UTF8FileInfo.value(), {SCPCFileMode::Read});
			SCP_buffer FileBuffer = SCP_buffer(CFile->GetSize());
			CFile->ReadBytes(FileBuffer.Data(), FileBuffer.Size);
			REQUIRE(CFile->DetectFileEncoding(FileBuffer.Data(), FileBuffer.Size) == CFILE::CFileTextEncoding::UTF8);
		}
	}

	SUBCASE("UTF-8 Normalization")
	{
		SCP_vector<SCP_buffer> EncodedBuffers;
		SCP_vector<SCP_string> Filenames = {"utf8", "utf8bom", "latin"};
		std::size_t SmallestEncodedBufferSize = 0;
		//Load and normalize all the test files with different encodings
		for (SCP_string Filename : Filenames)
		{
			auto FileInfo = EncodingModuleHandle->FindFileInfo(Filename + ".tbl", SCPCFilePathTypeID::Tables);
			auto CFile = EncodingModuleHandle->CFileOpen(FileInfo.value(), {SCPCFileMode::Read});
			
			EncodedBuffers.push_back(CFile->LoadAsText());
			if (SmallestEncodedBufferSize == 0)
			{
				SmallestEncodedBufferSize = EncodedBuffers.back().Size;
			}
			else
			{
				SmallestEncodedBufferSize = std::min(SmallestEncodedBufferSize, EncodedBuffers.back().Size);
			}
		}
		// Make sure the bytes in all the buffers are the same (ie normalization generated identical output for each input encoding)
		for (std::size_t BufferIndex = 0; BufferIndex < SmallestEncodedBufferSize; BufferIndex++)
		{
			char CurrentByte = EncodedBuffers.front()[BufferIndex];
			for (SCP_vector<SCP_buffer>::iterator CurrentBuffer = EncodedBuffers.begin() + 1;
				 CurrentBuffer != EncodedBuffers.end(); CurrentBuffer++)
			{
				REQUIRE(CurrentByte == (*CurrentBuffer)[BufferIndex]);
			}
		}
	}
}