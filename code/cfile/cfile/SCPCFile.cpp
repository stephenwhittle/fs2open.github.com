#include "cfile/SCPCFile.h"
#include "SCPEndian.h"




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
	char PreviousByte = 0;

	auto ValidateUTF8 = [&ValidatesAsUTF8, &PreviousByte](uint8_t Byte)
	{
		if (
			ValidatesAsUTF8 &&
			(Byte == 0x00) ||
			(Byte == 0xC0) ||
			(Byte == 0xC1) ||
			((Byte >= 0xF5) && (Byte <= 0xFF))
			)
		{
			ValidatesAsUTF8 = false;
		}
		//Check that tail bytes (0x80-0xBF) are preceded by 0xC2-0xF4 OR a tail byte
		if ((Byte >= 0x80) && (Byte <= 0xBF))
		{
			if (
				!((PreviousByte >= 0x80) && (PreviousByte <= 0xBF)) &&
				!((PreviousByte >= 0xC2) && (PreviousByte <= 0xF4))
				)
			{
				ValidatesAsUTF8 = false;
			}
		}
	};
	auto ValidateWindowsLatin = [&Has8859ControlCharacters](uint8_t Byte)
	{
		if ((Byte >= 0x80) && (Byte <= 0x9F))
		{
			Has8859ControlCharacters = true;
		}
	};
	auto ValidateASCII = [&HasByteAbove7F](uint8_t Byte)
	{
		if (Byte > 0x7F)
		{
			HasByteAbove7F = false;
		}
	};
	//iterate through all the other characters in the file
	for (uint32_t ByteIndex = 0; ByteIndex < Size; ByteIndex++)
	{
		uint8_t Byte = Buffer[ByteIndex];
		ValidateUTF8(Byte);
		//Check that the character is in the valid UTF8 range
		ValidateWindowsLatin(Byte);
		ValidateASCII(Byte);
		PreviousByte = Byte;
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
		return CFileEncryptionMagic::OldSignature;
		break;
	case 0x5c331a55_AsLittleEndian32:
		return CFileEncryptionMagic::NewSignature;
		break;
	case 0xcacacaca_AsLittleEndian32:
		return CFileEncryptionMagic::EightBitSignature;
		break;
	default:
		break;
	}
	SeekAbsolute(OldFilePosition);
	return CFileEncryptionMagic::NotEncrypted;
}

SCP_buffer CFILE::UTF8Normalize()
{
	std::uintmax_t OldPosition = Tell();
	SeekAbsolute(0);
	//add one extra byte of null padding
	SCP_buffer raw_text = SCP_buffer(size + 1);
	ReadBytes(raw_text.Data(), size);
	CFileEncryptionMagic FileEncryption = DetectFileEncryption();
	
	
	uintmax_t PayloadOffset = 0;
	//convert to switch/case later
	//using internal size not buffer size here so we don't look at any null terminator at the end of the buffer
	if (DetectFileEncoding(raw_text.Data(), size) == CFileTextEncoding::UTF8BOM)
	{
		PayloadOffset += 3;
	}
	if (FileEncryption != CFileEncryptionMagic::NotEncrypted)
	{
		PayloadOffset += 4;
	}

	SCP_buffer BufferWithoutBom = raw_text.CopyRange(raw_text.begin() + PayloadOffset, raw_text.end());
	raw_text                    = std::move(BufferWithoutBom);

	// this will need to calculate the offset
	
	if (FileEncryption != CFileEncryptionMagic::NotEncrypted) {
		//unencrypt(raw_text, FileEncryption);
	}


	SeekAbsolute(OldPosition);
	return raw_text;
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
