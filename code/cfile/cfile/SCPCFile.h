#pragma once
#include <cstdio>
#include <fstream>
#include <mio/mmap.hpp>
#include "SCPCallPermit.h"
#include "SCPFlags.h"
#include "FSAssert.h"
#include "SCPCFilePathType.h"
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
	enum class EDataSource
	{
		NotInitialized,
		MemoryMapped,
		LooseFile,
		PackFile,
		InMemory
	};
	EDataSource CurrentDataSource = EDataSource::NotInitialized;
	//may use ummap if we want unsigned chars
	mio::mmap_source MemoryMappedFile;
public:
	std::fstream UnderlyingFile;


	SCPCFilePathTypeID dir_type;                  // directory location
	//this pointer is read-only also. no write support for memory-mapped files or for in-memory files
	const void* data;              // Pointer for memory-mapped file access.  NULL if not mem-mapped.
	bool mem_mapped = false; // Flag for memory mapped files (if data is not null and this is false it means that it's an
					 // embedded file)
	size_t data_length; // length of data for mmap

	std::uintmax_t lib_offset;
	
	std::uintmax_t size; // for packed files

	

	
	/*
	//also cftemp but may be special for that one*/

	//TODO: do we want to pass in the CFileInfo ID for these?
	//TODO: delete copy constructor, perhaps delete move constructor too
	CFILE(SCPCallPermit<class SCPCFileModule>, SCPPath PackFilePath, std::uintmax_t Offset, std::uintmax_t Size)
		:CurrentDataSource(EDataSource::PackFile),
		lib_offset(Offset),
		size(Size)
	{
		UnderlyingFile.open(PackFilePath.c_str(), std::ios::in | std::ios::binary);
		UnderlyingFile.seekp(Offset);
	}

	CFILE(SCPCallPermit<class SCPCFileModule>, SCPPath FilePath, SCPCFileModeFlags Mode)
		:CurrentDataSource(EDataSource::LooseFile)
	{		
		if (Mode.HasFlag(SCPCFileMode::MemoryMapped))
		{
			CurrentDataSource = EDataSource::MemoryMapped;
			mem_mapped = true;
			MemoryMappedFile = mio::mmap_source(FilePath);
			return;
		}

		std::fstream::openmode DesiredMode;

		if (Mode.HasFlag(SCPCFileMode::Write))
		{
			DesiredMode = std::ios::out;
		}

		DesiredMode |= (std::ios::in | std::ios::binary);

		// open the file using mode settings
		UnderlyingFile.open(FilePath.c_str(), DesiredMode);

	}

	CFILE(SCPCallPermit<class SCPCFileModule>, uintmax_t Size, void* DataPointer)
		:CurrentDataSource(EDataSource::InMemory),
		size(Size),
		data(DataPointer) {};

	~CFILE()
	{
		if (CurrentDataSource == EDataSource::MemoryMapped)
		{
			MemoryMappedFile.unmap();
		}
		else
		{
			UnderlyingFile.close();
		}
	}


	void Flush() {};
	//may belong on the SCPFile type
	void Delete() {};

	SCPCFilePathTypeID GetDirectoryType() { return dir_type; }

	template<typename DestinationType>
	int Read(DestinationType* Destination, size_t ElementSize, size_t ElementCount)
	{
		return 0;
	}

	template <typename DestinationType>
	int ReadBytes(DestinationType* Destination, size_t Count)
	{
		return 0;
	}

	template<typename SourceType>
	int Write(SourceType* Source, size_t ElementCount) {}

	void SeekAbsolute() {};
	void SeekRelative() {};
	void SeekEnd() {};
	void Tell() {};
	void WriteChar() {};
	void WriteString() {};


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