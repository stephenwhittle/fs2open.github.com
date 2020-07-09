#pragma once
#include <cstdio>
#include <mio/mmap.hpp>
#include "SCPCallPermit.h"

class CFILE
{
    
private:

	//may use ummap if we want unsigned chars
    mio::mmap_source MemoryMappedFile;
public:
	


	int dir_type;                  // directory location
	FILE* fp;                      // File pointer if opening an individual file
	const void* data;              // Pointer for memory-mapped file access.  NULL if not mem-mapped.
	bool mem_mapped; // Flag for memory mapped files (if data is not null and this is false it means that it's an
					 // embedded file)
	size_t data_length; // length of data for mmap

	size_t lib_offset;
	size_t raw_position;
	size_t size; // for packed files

	size_t max_read_len; // max read offset, for special error handling

	const char* source_file;
	int line_num;
	//restrict the constructor so only CFileModule can create it
	//then anybody that wants a CFile can do so via CFileModule::OpenCFile(params)
	//passkey idiom or attorney-client
	//placeholder default constructor
	_cfopen(const char* source_file,
			int line,
			const char* filename,
			const char* mode,
			int type /*= CFILE_NORMAL*/,
			int dir_type /*= CF_TYPE_ANY*/,
			bool localize /*= false*/,
			uint32_t location_flags /*= CF_LOCATION_ALL*/,
			SCP_string LanguagePrefix /*= ""*/) 
		CFILE* _cfopen_special(const char* source_file,
																	   int line,
																	   const char* file_path,
																	   const char* mode,
																	   const size_t size,
																	   const size_t offset,
																	   const void* data,
																	   int dir_type = CF_TYPE_ANY);
	//also cftemp but may be special for that one
	CFILE(SCPCallPermit<class SCPCFileModule>, )
		: dir_type(0),
		fp(nullptr),
		data(nullptr),
		mem_mapped(false),
		data_length(0),
		lib_offset(0),
		raw_position(0),
		size(0),
		max_read_len(0),
		source_file(nullptr),
		line_num(0)
	{}
};