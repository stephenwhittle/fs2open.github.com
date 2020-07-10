/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on the 
 * source.
 *
*/ 



#ifndef _CFILESYSTEM_H
#define _CFILESYSTEM_H

#include "cfile/cfile.h"

// Builds a list of all the files
void cf_build_secondary_filelist( const char *cdrom_path );
void cf_free_secondary_filelist();

// Internal stuff
typedef struct cf_pathtype {
	int			index;					// To verify that the CF_TYPE define is correctly indexed into this array
	const char*	path;					// Path relative to FreeSpace root, has ending backslash.
	const char*	extensions;				// Extensions used in this pathtype, separated by spaces
	int			parent_index;			// Index of this directory's parent.  Used for creating directories when writing.
} cf_pathtype;

// During cfile_init, verify that Pathtypes[n].index == n for each item
extern cf_pathtype Pathtypes[CF_MAX_PATH_TYPES];

bool cf_check_location_flags(uint32_t check_flags, uint32_t desired_flags);

// Returns the default storage path for files given a 
// particular pathtype.   In other words, the path to 
// the unpacked, non-cd'd, stored on hard drive path.
// If filename isn't null it will also tack the filename
// on the end, creating a completely valid filename.
// Input:   pathtype  - CF_TYPE_??
//			path_max  - Maximum characters in the path
//          filename  - optional, if set, tacks the filename onto end of path.
// Output:  path      - Fully qualified pathname.
//Returns 0 if result would be too long (invalid result)
int cf_create_default_path_string(char* path, uint path_max, int pathtype, const char* filename = nullptr,
                                  bool localize = false, uint32_t location_flags = CF_LOCATION_ALL, SCP_string LanguagePrefix = "");
int cf_create_default_path_string(SCP_string& path, int pathtype, const char* filename = nullptr, bool localize = false,
                                  uint32_t location_flags = CF_LOCATION_ALL, SCP_string LanguagePrefix = "");

typedef struct SCPCFileInfo {
	uint32_t uid;
	char name_ext[32];  // Filename and extension
	int root_index;     // Where in Roots this is located
	int pathtype_index; // Where in Paths this is located
	time_t write_time;  // When it was last written
	int size;           // How big it is in bytes
	int pack_offset;    // For pack files, where it is at.   0 if not in a pack file.  This can be used to tell if in a
						// pack file.
	char* real_name;    // For real files, the full path
	const void* data;   // For in-memory files, the data pointer
} SCPCFileInfo;

class SCPRootInfo {
	
public:
	enum class RootType
	{
		Path,
		PackFile,
		InMemory
	};
	SCPRootInfo(SCPPath RootPath, RootType Type, uint32_t LocationFlags) 
		:uid(0),
		Path(RootPath),
		Type(Type),
		location_flags(LocationFlags)
		{};
	uint32_t uid;
	SCPPath Path; // Contains something like c:\projects\freespace or
					// c:\projects\freespace\freespace.vp
	RootType Type;   // CF_ROOTTYPE_PATH  = Path, CF_ROOTTYPE_PACK =Pack file, CF_ROOTTYPE_MEMORY=In memory
	uint32_t location_flags;
};


#endif	//_CFILESYSTEM_H
