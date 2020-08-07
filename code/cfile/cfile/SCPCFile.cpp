#include "cfile/SCPCFile.h"
#include "SCPEndian.h"
enum class CFILE::CFileEncryptionMagic
{
	NotEncrypted = 0,
	OldSignature = INTEL_INT(0xdeadbeef),
	NewSignature = INTEL_INT(0x5c331a55),
	EightBitSignature = INTEL_INT(0xcacacaca)
};

enum class CFILE::CFileTextEncoding
{
	UTF8,
	WindowsLatin1,
	Iso88951,
	Ascii,
	UnsupportedUTF,
	Unknown
};
template <std::size_t Size>
CFileTextEncoding CFILE::DetectFileEncoding(const char (&Buffer)[Size])
{
	if (Size < 2)
	{
		return CFileTextEncoding::Unknown;
	}
	uint32_t BOM = 0;
	for (uint32_t ByteIndex = 0; ByteIndex < std::min(Size, 4); ByteIndex++)
	{
		(uint8_t*)(&BOM)[ByteIndex] = Buffer[ByteIndex];
	}
	if (
		INTEL_INT(BOM) == INTEL_INT(0x0000feff)|| //UTF32BE
		INTEL_INT(BOM) == INTEL_INT(0xfffe0000) //UTF32LE
		)
	{
		return CFileTextEncoding::UnsupportedUTF;
	} 
	else if (INTEL_INT(BOM) & INTEL_INT(0xefbbbf00))
	{
		return CFileTextEncoding::UTF8;
	}
	else if (INTEL_INT(BOM) & INTEL_INT(0xfeff0000)) //UTF16BE
	{
		return CFileTextEncoding::UnsupportedUTF;
	}
	else if (INTEL_INT(BOM) & INTEL_INT(0xfffe0000)) //UTF16LE
	{
		return CFileTextEncoding::UnsupportedUTF;
	}
	bool HasByteAbove7F = false;
	bool Has8859ControlCharacters = false;
	bool ValidatesAsUTF8 = true;
	char PreviousByte = 0;

	auto ValidateUTF8 = [&ValidatesAsUTF8, &PreviousByte](char Byte)
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
	auto ValidateWindowsLatin = [&Has8859ControlCharacters](char Byte)
	{
		if ((Byte >= 0x80) && (Byte <= 0x9F))
		{
			&Has8859ControlCharacters = true;
		}
	};
	auto ValidateASCII = [&HasByteAbove7F](char Byte)
	{
		if (Byte > 0x7F)
		{
			HasByteAbove7F = false;
		}
	};
	//iterate through all the other characters in the file
	for (uint32_t ByteIndex = 0; ByteIndex < Size; ByteIndex++)
	{
		auto Byte = Buffer[ByteIndex];
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
	switch (INTEL_INT(Magic))	
	{
	case INTEL_INT(0xdeadbeef):
		return CFileEncryptionMagic::OldSignature;
		break;
	case INTEL_INT(0x5c331a55):
		return CFileEncryptionMagic::NewSignature;
		break;
	case INTEL_INT(0xcacacaca):
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
	SCP_buffer raw_text = SCP_buffer(size + 1);
	ReadBytes(raw_text.Data(), size);
	CFileEncryptionMagic FileEncryption = DetectFileEncryption();
	
	//possibly pass in a boolean for the bom too?
	switch (DetectFileEncoding(raw_text.Data()));
	{
	case CFileTextEncoding::UTF8:
		break;
	case CFileTextEncoding::WindowsLatin1:
		break;
	case CFileTextEncoding::Ascii:
		break;
	case CFileTextEncoding::Unknown:
	default:
		break;
	}
	// this will need to calculate the offset
	uintmax_t RealLength = check_encoding_and_skip_bom(mf, filename);

	if (FileEncryption != CFileEncryptionMagic::NotEncrypted) {
		int unscrambled_len;
		SCP_buffer unscrambled_text = SCP_buffer(RealLength + 1);

		ReadBytes(unscrambled_text.Data(), RealLength);

		// unscramble text
		unencrypt(unscrambled_text.Data(), file_len, raw_text.Data(), &unscrambled_len);
		file_len = unscrambled_len;
	} else 
	{
		ReadBytes(raw_text.Data(), RealLength);
	}

	// WMC - Slap a NULL character on here for the odd error where we forgot a #End
	raw_text[RealLength] = '\0';
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
		
		std::intmax_t DesiredPosition = Tell() + Offset;

		Assertion((DesiredPosition >= lib_offset) && (DesiredPosition <= lib_offset + size), "Packed CFile seek request was out of bounds!");

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
		return UnderlyingFile.tellg() - lib_offset;
		break;
	case EDataSource::MemoryMapped:
		return MemoryMappedFileView.tellg();
		break;
	default:
		break;
	}
}
