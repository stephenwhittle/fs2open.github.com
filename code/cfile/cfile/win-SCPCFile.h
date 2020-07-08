#pragma once
#include <cstdio>
#define CFILE_BLOCK_UNUSED 0
#define CFILE_BLOCK_USED 1
#include <windows.h>

struct CFILE {
	int type = CFILE_BLOCK_UNUSED; // CFILE_BLOCK_UNUSED, CFILE_BLOCK_USED
	int dir_type;                  // directory location
	FILE* fp;                      // File pointer if opening an individual file
	const void* data;              // Pointer for memory-mapped file access.  NULL if not mem-mapped.
	bool mem_mapped; // Flag for memory mapped files (if data is not null and this is false it means that it's an
					 // embedded file)
#ifdef _WIN32
	HANDLE hInFile;  // Handle from CreateFile()
	HANDLE hMapFile; // Handle from CreateFileMapping()
#else //_WIN32
	size_t data_length; // length of data for mmap
#endif

	size_t lib_offset;
	size_t raw_position;
	size_t size; // for packed files

	size_t max_read_len; // max read offset, for special error handling

	const char* source_file;
	int line_num;
};