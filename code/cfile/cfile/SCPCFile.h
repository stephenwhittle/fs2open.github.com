#pragma once
#include <cstdio>
#include <fstream>
#include <mio/mmap.hpp>
#include "SCPCallPermit.h"
#include "SCPFlags.h"
#include "SCPEndian.h"
#include "FSAssert.h"
#include "FSStdTypes.h"
#include "SCPCFilePathType.h"
#include "SCPMembuf.h"
#include "filesystem/SCPPath.h"
#include "filesystem/SCPFile.h"
enum class SCPCFileMode
{
	Append = 1,
	Binary = 2,
	Read = 4,
	Write = 8,
	EraseOnOpen = 16,
	MemoryMapped = 32
};

using SCPCFileModeFlags = SCPFlags<SCPCFileMode>;

enum class SCPCFileLocation
{
	GameRootDirectory = 1,
	UserDirectory = 2,
	MemoryRoot = 4,
	TopLevelDirectory = 8,
	PrimaryMod = 16,
	SecondaryMods = 32
};

using SCPCFileLocationFlags = SCPFlags<SCPCFileLocation>;


//Definition for 'all flags set'
constexpr SCPCFileLocationFlags SCPCFileLocationALL = 
SCPCFileLocationFlags
{ 
	SCPCFileLocation::GameRootDirectory,
	SCPCFileLocation::UserDirectory,
	SCPCFileLocation::MemoryRoot,
	SCPCFileLocation::TopLevelDirectory,
	SCPCFileLocation::PrimaryMod,
	SCPCFileLocation::SecondaryMods
};




class CFILE
{

private:
	
	//Takes in the buffer as a reference to avoid copy overhead
	

	enum class EDataSource
	{
		NotInitialized,
		MemoryMapped,
		LooseFile,
		PackFile,
		InMemory
	};
	bool Writeable = false;
	EDataSource CurrentDataSource = EDataSource::NotInitialized;
	//may use ummap if we want unsigned chars
	mio::mmap_source MemoryMappedFile;
	SCPMembuf MemoryMappedFileBuffer;
	std::istream MemoryMappedFileView;
public:
	std::fstream UnderlyingFile;
	SCPMembuf InMemoryFileBuffer;
	std::istream InMemoryFileView;

	SCPCFilePathTypeID dir_type;                  // directory location
	//this pointer is read-only also. no write support for memory-mapped files or for in-memory files
	const void* data;              // Pointer for memory-mapped file access.  NULL if not mem-mapped.
	bool mem_mapped = false; // Flag for memory mapped files (if data is not null and this is false it means that it's an
					 // embedded file)
	std::uintmax_t lib_offset;
	
	std::uintmax_t size; // for packed files
	enum class CFileEncryptionMagic : uint32_t
	{
		NotEncrypted,
		OldSignature,
		NewSignature,
		EightBitSignature
	};

	enum class CFileTextEncoding
	{
		UTF8,
		UTF8BOM,
		WindowsLatin1,
		Iso88951,
		Ascii,
		UnsupportedUTF,
		Unknown
	};
	CFileTextEncoding DetectFileEncoding(const char* Buffer, std::size_t Size);
	CFileEncryptionMagic DetectFileEncryption();
	
	
	/*
	//also cftemp but may be special for that one*/

	//TODO: do we want to pass in the CFileInfo ID for these?
	//TODO: delete copy constructor, perhaps delete move constructor too
	CFILE(const SCPCallPermit<class SCPCFileModule>&, SCPPath PackFilePath, std::uintmax_t Offset, std::uintmax_t Size)
		:CurrentDataSource(EDataSource::PackFile),
		MemoryMappedFileView(&MemoryMappedFileBuffer),
		InMemoryFileView(&InMemoryFileBuffer),
		lib_offset(Offset),
		size(Size)
	{
		UnderlyingFile.open(PackFilePath.c_str(), std::ios::in | std::ios::binary);
		UnderlyingFile.seekp(Offset);
	}

	CFILE(const SCPCallPermit<class SCPCFileModule>&, SCPPath FilePath, SCPCFileModeFlags Mode)
		:CurrentDataSource(EDataSource::LooseFile),
		MemoryMappedFileView(&MemoryMappedFileBuffer),
		InMemoryFileView(&InMemoryFileBuffer)
	{		
		if (Mode.HasFlag(SCPCFileMode::MemoryMapped))
		{
			CurrentDataSource = EDataSource::MemoryMapped;
			mem_mapped = true;
			MemoryMappedFile = mio::mmap_source(FilePath.string());
			size = MemoryMappedFile.size();
			MemoryMappedFileBuffer = SCPMembuf(MemoryMappedFile.begin(), MemoryMappedFile.end());
			return;
		}

		std::fstream::openmode DesiredMode = 0;

		if (Mode.HasFlag(SCPCFileMode::Write))
		{
			DesiredMode = std::ios::out | std::ios::app;
			Writeable = true;
			SCPPath::CreateDirectories(FilePath);
		}
		else
		{
			Assertion(SCPFile::Exists(FilePath), "Cannot open a non-existing file for read-only access!");
		}

		DesiredMode |= (std::ios::in | std::ios::binary);
		size = SCPFile::FileSize(FilePath);

		// open the file using mode settings
		UnderlyingFile.open(FilePath.c_str(), DesiredMode);
		Assert(UnderlyingFile.good());
	}

	CFILE(const SCPCallPermit<class SCPCFileModule>&, uintmax_t Size, void* DataPointer)
		:CurrentDataSource(EDataSource::InMemory),
		MemoryMappedFileView(&MemoryMappedFileBuffer),
		InMemoryFileBuffer((const SCPMembuf::byte*) DataPointer, Size),
		InMemoryFileView(&InMemoryFileBuffer),
		size(Size),
		data(DataPointer) {};

	~CFILE()
	{
		if (CurrentDataSource == EDataSource::MemoryMapped)
		{
			MemoryMappedFile.unmap();
		}
		else if (CurrentDataSource == EDataSource::LooseFile || CurrentDataSource == EDataSource::PackFile)
		{
			UnderlyingFile.close();
		}
	}


	void Flush() {};
	//may belong on the SCPFile type
	void Delete() {};

	SCPCFilePathTypeID GetDirectoryType() { return dir_type; }
	//todo: refactor this into a free function that operates on SCP_buffers and some kind of 
	//non-owning view
	SCP_buffer UTF8Normalize();
	std::uintmax_t GetSize();
	template<typename DestinationType>
	int Read(DestinationType* Destination, size_t ElementSize, size_t ElementCount)
	{
		switch (CurrentDataSource) {

		case CFILE::EDataSource::MemoryMapped:
			MemoryMappedFileView.read((char*)Destination, ElementSize * ElementCount);
			MemoryMappedFileView.clear();
			return MemoryMappedFileView.gcount();
			break;
		case CFILE::EDataSource::LooseFile:
		case CFILE::EDataSource::PackFile:
			UnderlyingFile.read((char*)Destination, ElementSize * ElementCount);
			UnderlyingFile.clear();
			return UnderlyingFile.gcount();
			break;
		case CFILE::EDataSource::InMemory:
			InMemoryFileView.read((char*)Destination, ElementSize * ElementCount);
			InMemoryFileView.clear();
			return InMemoryFileView.gcount();
			break;
		case CFILE::EDataSource::NotInitialized:
		default:
			break;
		}
		return 0;
	}

	template <typename DestinationType>std::uintmax_t ReadBytes(DestinationType* Destination, size_t Count)
	{
		switch (CurrentDataSource)
		{
		
		case CFILE::EDataSource::MemoryMapped:
			MemoryMappedFileView.read((char*)Destination, Count);
			MemoryMappedFileView.clear();
			return MemoryMappedFileView.gcount();
			break;
		case CFILE::EDataSource::LooseFile:
		case CFILE::EDataSource::PackFile:
			UnderlyingFile.read((char*)Destination, Count);
			UnderlyingFile.clear();
			return UnderlyingFile.gcount();
			break;
		case CFILE::EDataSource::InMemory:
			InMemoryFileView.read((char*)Destination, Count);
			InMemoryFileView.clear();
			return InMemoryFileView.gcount();
			break;
		case CFILE::EDataSource::NotInitialized:
		default:
			break;
		}
		return 0;
	}

	template<typename SourceType>
	int WriteBytes(SourceType* Source, size_t Count) 
	{
		Assert(Writeable);
	}
	
	template<typename SourceType>
	int Write(SourceType* Source, std::size_t ElementSize, std::size_t ElementCount)
	{
		Assert(Writeable);

	}

	void SeekAbsolute(uintmax_t Position);
	void SeekRelative(std::intmax_t Offset);
	//void SeekEnd() {};
	std::uintmax_t Tell();
	void WriteChar() {};
	bool WriteString(SCP_string StrToWrite) 
	{
		Assert(Writeable);
		if (CurrentDataSource == EDataSource::LooseFile)
		{
			UnderlyingFile << StrToWrite;
			UnderlyingFile.flush();
			return true;
		}
		else
		{
			return false;
		}
	};


};



/*
// This allows to use std::unique_ptr with CFILE
namespace std {
template <>
struct default_delete<CFILE> {
	void operator()(CFILE* ptr)
	{
		if (cf_is_valid(ptr)) {
			cfclose(ptr);
		}
	}
};
} // namespace std
*/