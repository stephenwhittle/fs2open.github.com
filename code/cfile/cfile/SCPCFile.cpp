#include "cfile/SCPCFile.h"
#include "SCPEndian.h"
enum class CFile::CFileEncryptionMagic
{
	NotEncrypted = 0,
	OldSignature = INTEL_INT(0xdeadbeef),
	NewSignature = INTEL_INT(0x5c331a55),
	EightBitSignature = INTEL_INT(0xcacacaca)
};


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

SCP_buffer CFILE::ReadAllContentsIntoBuffer()
{
	SCP_buffer raw_text = SCP_buffer(size + 1);
	ReadBytes(raw_text.Data(), std::min(size, (uintmax_t)10));
	CFileEncryptionMagic FileEncryption = DetectFileEncryption();
	std::uintmax_t OldPosition          = Tell();

	// this will need to calculate the offset
	uintmax_t RealLength = check_encoding_and_skip_bom(mf, filename);

	if (FileEncryption != CFileEncryptionMagic::NotEncrypted) {
		int unscrambled_len;
		SCP_buffer unscrambled_text = SCP_buffer(RealLength + 1);

		ReadBytes(unscrambled_text.Data(), RealLength);

		// unscramble text
		unencrypt(unscrambled_text.Data(), file_len, raw_text.Data(), &unscrambled_len);
		file_len = unscrambled_len;
	} else {
		ReadBytes(raw_text.Data(), RealLength);
	}

	// WMC - Slap a NULL character on here for the odd error where we forgot a #End
	raw_text[RealLength] = '\0';
	SeekAbsolute(OldPosition);
	return raw_text;
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
