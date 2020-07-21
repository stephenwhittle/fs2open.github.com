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

	//may use ummap if we want unsigned chars
	mio::mmap_source MemoryMappedFile;
public:
	std::fstream UnderlyingFile;


	SCPCFilePathTypeID dir_type;                  // directory location
	FILE* fp;                      // File pointer if opening an individual file
	const void* data;              // Pointer for memory-mapped file access.  NULL if not mem-mapped.
	bool mem_mapped = false; // Flag for memory mapped files (if data is not null and this is false it means that it's an
					 // embedded file)
	size_t data_length; // length of data for mmap

	std::uintmax_t lib_offset;
	size_t raw_position;
	std::uintmax_t size; // for packed files

	size_t max_read_len; // max read offset, for special error handling

	const char* source_file;
	int line_num;
	/*//restrict the constructor so only CFileModule can create it
	//then anybody that wants a CFile can do so via CFileModule::OpenCFile(params)
	//passkey idiom or attorney-client
	//placeholder default constructor
	_cfopen(const char* source_file,
			int line,
			const char* filename,
			const char* mode,
			int type / *= CFILE_NORMAL* /,
			int dir_type / *= CF_TYPE_ANY* /,
			bool localize / *= false* /,
			uint32_t location_flags / *= CF_LOCATION_ALL* /,
			SCP_string LanguagePrefix / *= ""* /)
		CFILE* _cfopen_special(const char* source_file,
																	   int line,
																	   const char* file_path,
																	   const char* mode,
																	   const size_t size,
																	   const size_t offset,
																	   const void* data,
																	   int dir_type = CF_TYPE_ANY);



	//also cftemp but may be special for that one*/

	//TODO: do we want to pass in the CFileInfo ID for these?
	//TODO: delete copy constructor, perhaps delete move constructor too
	CFILE(SCPCallPermit<class SCPCFileModule>, SCPPath PackFilePath, std::uintmax_t Offset, std::uintmax_t Size)
		:lib_offset(Offset),
		size(Size)
	{
		UnderlyingFile.open(PackFilePath.c_str(), std::ios::in | std::ios::binary);
		UnderlyingFile.seekp(Offset);
	}

	CFILE(SCPCallPermit<class SCPCFileModule>, SCPPath FilePath, SCPCFileModeFlags Mode)
	{		
		if (Mode.HasFlag(SCPCFileMode::MemoryMapped))
		{
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