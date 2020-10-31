#include "cfile/SCPCFile.h"
#include "SCPEndian.h"
#include "SCPBitOps.h"
#include <iostream>
#include <bitset>
#include "tutf8e.h"



CFILE::CFileTextEncoding CFILE::DetectFileEncoding(const char* Buffer, std::size_t Size)
{
	if (Size < 2)
	{
		return CFileTextEncoding::Unknown;
	}
	uint32_t BOM = *((uint32_t*)Buffer);

	SeekAbsolute(0);
	constexpr uint32_t UTF8BomSequence = 0xefbbbf00_AsBigEndian32;
	constexpr uint32_t UTF16BESequence = 0xfeff0000_AsBigEndian32;
	constexpr uint32_t UTF16LESequence = 0xfffe0000_AsBigEndian32;
	constexpr uint32_t UTF32BESequence = 0x0000feff_AsBigEndian32;
	constexpr uint32_t UTF32LESequence = 0xfffe0000_AsBigEndian32;

	if (
		BOM == UTF32LESequence || //UTF32LE
		BOM == UTF32BESequence //UTF32BE
		)
	{
		return CFileTextEncoding::UnsupportedUTF;
	} 
	else if ((BOM & UTF8BomSequence) == UTF8BomSequence) //00bfbbef
	{
		return CFileTextEncoding::UTF8BOM;
	}
	else if ((BOM & UTF16LESequence) == UTF16LESequence) //UTF16LE
	{
		return CFileTextEncoding::UnsupportedUTF;
	}
	else if ((BOM & UTF16BESequence) == UTF16BESequence) //UTF16BE
	{
		return CFileTextEncoding::UnsupportedUTF;
	}
	bool HasByteAbove7F = false;
	bool Has8859ControlCharacters = false;
	bool ValidatesAsUTF8 = true;
	unsigned char PreviousChar = 0;

	auto ValidateUTF8 = [&ValidatesAsUTF8, &PreviousChar](unsigned char NextChar)
	{
		if (
			ValidatesAsUTF8 &&
			(NextChar == 0x00) ||
			(NextChar == 0xC0) ||
			(NextChar == 0xC1) ||
			((NextChar >= 0xF5) && (NextChar <= 0xFF))
			)
		{
			ValidatesAsUTF8 = false;
		}
		//Check that tail bytes (0x80-0xBF) are preceded by 0xC2-0xF4 OR a tail byte
		if ((NextChar >= 0x80) && (NextChar <= 0xBF))
		{
			if (
				!((PreviousChar >= 0x80) && (PreviousChar <= 0xBF)) &&
				!((PreviousChar >= 0xC2) && (PreviousChar <= 0xF4))
				)
			{
				ValidatesAsUTF8 = false;
			}
		}
	};
	auto ValidateWindowsLatin = [&Has8859ControlCharacters](unsigned char NextChar)
	{
		if ((NextChar >= 0x80) && (NextChar <= 0x9F))
		{
			Has8859ControlCharacters = true;
		}
	};
	auto ValidateASCII = [&HasByteAbove7F](unsigned char NextChar)
	{
		if (NextChar > 0x7F)
		{
			HasByteAbove7F = false;
		}
	};
	//iterate through all the other characters in the file
	for (uint32_t ByteIndex = 0; ByteIndex < Size; ByteIndex++)
	{
		unsigned char Byte = Buffer[ByteIndex];
		ValidateUTF8(Byte);
		//Check that the character is in the valid UTF8 range
		ValidateWindowsLatin(Byte);
		ValidateASCII(Byte);
		PreviousChar = Byte;
	}

	if (ValidatesAsUTF8)
	{
		return CFileTextEncoding::UTF8;
	}
	else if (Has8859ControlCharacters)
	{
		return CFileTextEncoding::WindowsLatin1;
	}
	else if (!HasByteAbove7F)
	{
		return CFileTextEncoding::Ascii;
	}
	else
	{
		return CFileTextEncoding::Unknown;
	}
}


CFILE::CFileEncryptionMagic CFILE::DetectFileEncryption() 
{
	std::uintmax_t OldFilePosition = Tell();
	SeekAbsolute(0);
	uint32_t Magic = 0;
	ReadBytes(&Magic, 4);
	switch (Magic)	
	{
	case 0xdeadbeef_AsLittleEndian32:
		SeekAbsolute(OldFilePosition);
		return CFileEncryptionMagic::OldSignature;
		break;
	case 0x5c331a55_AsLittleEndian32:
		SeekAbsolute(OldFilePosition);
		return CFileEncryptionMagic::NewSignature;
		break;
	case 0xcacacaca_AsLittleEndian32:
		SeekAbsolute(OldFilePosition);
		return CFileEncryptionMagic::EightBitSignature;
		break;
	default:
		break;
	}
	SeekAbsolute(OldFilePosition);
	return CFileEncryptionMagic::NotEncrypted;
}

SCP_buffer CFILE::LoadAsText()
{
	std::uintmax_t OldPosition = Tell();
	SeekAbsolute(0);
	//add one extra byte of null padding
	SCP_buffer FileContents = SCP_buffer(size + 1);
	ReadBytes(FileContents.Data(), size);
	CFileEncryptionMagic FileEncryption = DetectFileEncryption();
	
	
	uintmax_t PayloadOffset = 0;
	//can probably perform the decryption here - I don't think its possible to have encrypted non-ASCII files
	//so we don't have to worry about how an encrypted header and a BOM interact
	if (FileEncryption != CFileEncryptionMagic::NotEncrypted)
	{
		PayloadOffset += 4;
	}

	// using internal size not buffer size here so we don't look at any null terminator at the end of the buffer
	CFileTextEncoding FileEncoding = DetectFileEncoding(FileContents.Data(), size);

	//If we either can't detect the encoding, or we don't support it, or the buffer is already UTF8, no need for further action
	if ((FileEncoding == CFileTextEncoding::Unknown) || 
		(FileEncoding == CFileTextEncoding::UnsupportedUTF) || 
		(FileEncoding == CFileTextEncoding::UTF8))
	{
		SeekAbsolute(OldPosition);
		return FileContents;
	}

	//convert to switch/case later if we want to handle the other types
	if (FileEncoding == CFileTextEncoding::UTF8BOM)
	{
		PayloadOffset += 3;
	}


	SCP_buffer BufferWithoutBom = FileContents.CopyRange(FileContents.begin() + PayloadOffset, FileContents.end());
	FileContents = std::move(BufferWithoutBom);

	if (FileEncoding == CFileTextEncoding::WindowsLatin1)
	{
		const char InvalidCharacter = ' ';
		std::size_t OutputBufferSize = 0;
		tutf8e_encoder_buffer_length(tutf8e_encoder_windows_1252, FileContents.Data(), &InvalidCharacter, FileContents.Size, &OutputBufferSize);
	
		SCP_buffer NormalizedOutput = SCP_buffer(OutputBufferSize);

		tutf8e_encoder_buffer_encode(tutf8e_encoder_windows_1252, FileContents.Data(), FileContents.Size, &InvalidCharacter, NormalizedOutput.Data(), &OutputBufferSize);
		FileContents = std::move(NormalizedOutput);
	}
	
	if (FileEncryption != CFileEncryptionMagic::NotEncrypted) {
		DecryptBuffer(FileContents, FileEncryption);
	}


	SeekAbsolute(OldPosition);
	return FileContents;
}

std::uintmax_t CFILE::GetSize() 
{
	//size should be valid for all file types
	//leaving this function here in case that changes in future
	//and size has to be dynamically calculated
	return size;
}

void CFILE::SeekAbsolute(uintmax_t Position) 
{
	switch (CurrentDataSource)
	{
	case EDataSource::InMemory:
		InMemoryFileView.seekg(Position);
		break;
	case EDataSource::LooseFile:
		UnderlyingFile.seekg(Position);
		break;
	case EDataSource::PackFile:
		UnderlyingFile.seekg(Position - lib_offset);
		break;
	case EDataSource::MemoryMapped:
		MemoryMappedFileView.seekg(Position);
		break;
	default:
		break;
	}
}

void CFILE::SeekRelative(std::intmax_t Offset)
{
	switch (CurrentDataSource)
	{
	case EDataSource::InMemory:
		InMemoryFileView.seekg(Offset, std::ios_base::cur);
		break;
	case EDataSource::LooseFile:
		UnderlyingFile.seekg(Offset, std::ios_base::cur);
		break;
	case EDataSource::PackFile:
		{
			std::intmax_t DesiredPosition = Tell() + Offset;

			Assertion((DesiredPosition >= lib_offset) && (DesiredPosition <= lib_offset + size), "Packed CFile seek request was out of bounds!");
		}
		UnderlyingFile.seekg(Offset, std::ios_base::cur);
		break;
	case EDataSource::MemoryMapped:
		MemoryMappedFileView.seekg(Offset, std::ios_base::cur);
		break;
	default:
		break;
	}
}

std::uintmax_t CFILE::Tell()
{
	switch (CurrentDataSource)
	{	
	case EDataSource::InMemory:
		return InMemoryFileView.tellg();
		break;
	case EDataSource::LooseFile:
		return UnderlyingFile.tellg();
		break;
	case EDataSource::PackFile:
		return (std::uintmax_t)UnderlyingFile.tellg() - lib_offset;
		break;
	case EDataSource::MemoryMapped:
		return MemoryMappedFileView.tellg();
		break;
	default:
		throw;
		break;
	}
}


void CFILE::DecryptBuffer(SCP_buffer& scrambled_text, CFILE::CFileEncryptionMagic EncryptionType)
{
	if (EncryptionType == CFILE::CFileEncryptionMagic::NewSignature)
	{
		//UnencryptNew(scrambled_text);
		return;
	}

	int byte_offset = 0;
	char maybe_last = 0;

	

	//unnecessary check as we already look at the encryption earlier
	// Only decrypt files that start with unique signature
	/*memcpy(&encrypt_id, scrambled_text, 4);

	if ( (encrypt_id != Encrypt_signature) && (encrypt_id !=  Encrypt_signature_8bit) ) {
		memcpy(text, scrambled_text, scrambled_len);
		*text_len = scrambled_len;
		return;
	}	*/

	//Skip the first 32-bit word
	/*scrambled_text += 4;
	scrambled_len -= 4;*/

	// First decrypt stage: undo XOR operation
	uintmax_t ByteIndex = 0;
	for (auto& CurrentByte : scrambled_text)
	{
		CurrentByte ^= ByteIndex;
		ByteIndex++;
	}

/*
	for ( i =0; i < scrambled_len; i++ ) {
		scrambled_text[i] ^= i;
	}
*/
	if (EncryptionType == CFILE::CFileEncryptionMagic::EightBitSignature)
	{
		return;
	}
	/*if (encrypt_id == Encrypt_signature_8bit) {
		memcpy(text, scrambled_text, scrambled_len);
		*text_len = scrambled_len;
		return;
	}*/
	
	auto Position = 0;
	
	auto DecryptedSize = (((scrambled_text.Size ) * 8))/ 7;
	SCP_buffer DecryptedBuffer(DecryptedSize);
	for (int i = 0; i < DecryptedSize; i++)
	{
		if (Position >= scrambled_text.Size) {
			break;
		}
		uint16_t CombinedValue = 0;
		uint8_t BitOffset = i % 8;
		if (BitOffset == 0)
		{
			//std::cout << i << ":" << std::bitset<8>(MaskHighBits<std::uint8_t>( 0, 7 - BitOffset)) <<" -- " << i+1 << ":" << std::bitset<8>(MaskLowBits<std::uint8_t>( 0, BitOffset)) << " -- Mask" << std::endl;
			//std::cout << i << ":" << std::bitset<8>(scrambled_text[Position]) <<" -- " << i+1 << ":" << std::bitset<8>(scrambled_text[Position+1]) << " -- Raw" << std::endl;
			//std::cout << i << ":" << std::bitset<8>(scrambled_text[Position] & MaskHighBits<std::uint8_t>( 0, 7 - BitOffset)) <<" -- " << i+1 << ":" << std::bitset<8>(scrambled_text[Position+1] & MaskLowBits<std::uint8_t>( 0, BitOffset)) << std::endl;

			//CombinedValue = ((scrambled_text[Position] & MaskHighBits<std::uint8_t>(0, 7 - BitOffset)) << 8) | (scrambled_text[Position + 1] & MaskLowBits<std::uint8_t>(0, BitOffset));
			//CombinedValue = CombinedValue >> 9;

			CombinedValue = ((scrambled_text[Position] & MaskHighBits<std::uint8_t>(0, 7 - BitOffset)));
			CombinedValue = CombinedValue >> 1;
		}
		else
		{
			//std::cout << i << ":" << std::bitset<8>(MaskLowBits<std::uint8_t>( 0, BitOffset)) <<" -- " << i+1 << ":" << std::bitset<8>(MaskHighBits<std::uint8_t>( 0, 7 - BitOffset)) << " -- Mask" <<std::endl;
			//std::cout << i << ":" << std::bitset<8>(scrambled_text[Position]) <<" -- " << i+1 << ":" << std::bitset<8>(scrambled_text[Position+1]) << " -- Raw" << std::endl;
			//std::cout << i << ":" << std::bitset<8>(scrambled_text[Position] & MaskLowBits<std::uint8_t>( 0, BitOffset)) <<" -- " << i+1 << ":" << std::bitset<8>(scrambled_text[Position+1] & MaskHighBits<std::uint8_t>( 0, 7 - BitOffset)) << std::endl;

			CombinedValue = ((scrambled_text[Position] & MaskLowBits<std::uint8_t>(0, BitOffset)) << 8) | (scrambled_text[Position + 1] & MaskHighBits<std::uint8_t>(0, 7 - BitOffset));
			CombinedValue = CombinedValue >> (BitOffset + 1);
			Position++;
		}
		//std::cout << (unsigned char)CombinedValue;
		DecryptedBuffer[i] = ((unsigned char)CombinedValue);
	}
	scrambled_text = std::move(DecryptedBuffer);
}