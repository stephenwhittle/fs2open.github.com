/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on the 
 * source.
 *
*/ 



#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <sstream>
#include <algorithm>

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#include <windows.h>
#include <winbase.h>		/* needed for memory mapping of file functions */
#endif

#ifdef SCP_UNIX
#include <glob.h>
#include <sys/types.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#endif

#include "cfile/cfilesystem.h"
#include "cfile/SCPCFile.h"
#include "cfile/SCPCFileModule.h"
#include "SCPCmdOptions.h"
#include "FSAssert.h"
#include "FSMathOps.h"
#include "SCPApplication.h"
#include "def_files/def_files.h"
//#include "localization/localize.h"
#include "config/SCPConfig.h"
#include "filesystem/SCPPath.h"
#include "filesystem/SCPFile.h"
#include "filesystem/SCPDirectoryIterator.h"
#include "SCPEndian.h"
#include "memory/memory.h"
#include "memory/utils.h"
#include "module/SCPModuleManager.h"
#include "cmdline/SCPCmdlineModule.h"
#include "cmdline/SCPCmdLine.h"
enum CfileRootType {
	CF_ROOTTYPE_PATH = 0,
	CF_ROOTTYPE_PACK = 1,
	CF_ROOTTYPE_MEMORY = 2,
};

//  Created by:
//    specifying hard drive tree
//    searching for pack files on hard drive		// Found by searching all known paths
//    specifying cd-rom tree
//    searching for pack files on CD-rom tree
/*


// convenient type for sorting (see cf_build_pack_list())
typedef struct cf_root_sort { 
	char				path[CF_MAX_PATHNAME_LENGTH];
	int				roottype;
	int				cf_type;
} cf_root_sort;

#define CF_NUM_ROOTS_PER_BLOCK   32
#define CF_MAX_ROOT_BLOCKS			256				// Can store 32*256 = 8192 Roots
#define CF_MAX_ROOTS					(CF_NUM_ROOTS_PER_BLOCK * CF_MAX_ROOT_BLOCKS)

typedef struct cf_root_block {
	SCPRootInfo			roots[CF_NUM_ROOTS_PER_BLOCK];
} cf_root_block;

static int Num_roots = 0;
static cf_root_block  *Root_blocks[CF_MAX_ROOT_BLOCKS];

static int Num_path_roots = 0;



#define CF_NUM_FILES_PER_BLOCK   512
#define CF_MAX_FILE_BLOCKS			128						// Can store 512*128 = 65536 files

typedef struct cf_file_block {
	SCPCFileInfo						files[CF_NUM_FILES_PER_BLOCK];
} cf_file_block;

static uint Num_files = 0;
static cf_file_block  *File_blocks[CF_MAX_FILE_BLOCKS];

// Return a pointer to to file 'index'.
SCPCFileInfo *cf_get_file(int index)
{
	int block = index / CF_NUM_FILES_PER_BLOCK;
	int offset = index % CF_NUM_FILES_PER_BLOCK;

	return &File_blocks[block]->files[offset];
}
*/


extern int cfile_inited;
/*

// Create a new root and return a pointer to it.  The structure is assumed unitialized.
SCPRootInfo *cf_get_root(int n)
{
	int block = n / CF_NUM_ROOTS_PER_BLOCK;
	int offset = n % CF_NUM_ROOTS_PER_BLOCK;

	if (!cfile_inited)
		return NULL;

	return &Root_blocks[block]->roots[offset];
}

*/

/*
// packfile sort function
bool cf_packfile_sort_func(const cf_root_sort &r1, const cf_root_sort &r2)
{
	// if the 2 directory types are the same, do a string compare
	if (r1.cf_type == r2.cf_type) {
		return (stricmp(r1.path, r2.path) < 0);
	}

	// otherwise return them in order of CF_TYPE_* precedence
	return (r1.cf_type < r2.cf_type);
}
*/


/**
 * Searches for a file.
 *
 * @note Follows all rules and precedence and searches CD's and pack files.
 *
 * @param filespec      Filename & extension
 * @param pathtype      See CF_TYPE_ defines in CFILE.H
 * @param localize      Undertake localization
 * @param location_flags Specifies where to search for the specified flag
 *
 * @return A structure which describes the found file
 */
SCPCFileInfo cf_find_file_location(const SCPPath filespec, SCPCFilePathTypeID PathType, bool localize /*= false*/, uint32_t location_flags /*= CF_LOCATION_ALL*/, SCP_string LanguagePrefix /*= ""*/)
{
	Assert(!filespec.empty());

	// see if we have something other than just a filename
	// our current rules say that any file that specifies a direct
	// path will try to be opened on that path.  If that open
	// fails, then we will open the file based on the extension
	// of the file

	// NOTE: full path should also include localization, if so desired
	if (filespec.has_parent_path())
	{
		SCPFile LocatedFile(filespec);
		if (LocatedFile.Exists())
		{	
			CFileLocation res(filespec, LocatedFile.FileSize());
			return res;
		}

		return CFileLocation();		// If they give a full path, fail if not found.
	}

	// Search the hard drive for files first.
	//if the user specifies a path, search that first
	//else search them in order from Root to the end of PathTypes;
	
	//return the result with the information

	//search for the exact path first
	if (filespec.has_parent_path())
	{
		//save these as stored queries ready for binding perhaps
		FileFinder FindFileWithExactPath();
		FindFileWithExactPath().where(sql::column("FullPath") == filespec.string());

		//SELECT * FROM files where FullPath = filespec.string()
		FileQueryBuilder QueryBuilder;
		QueryBuilder.FullPath({ FileQueryBuilder::ConditionType::Equal, filespec.string() });
		for (auto FileResult : CFileDatabase().Files(QueryBuilder))
		{
			//if we have a result we just want the first one that matches
			return FileResult;
		}
	}
	// if (localize) {
	// create localized filespec
	//SCPPath LocalizedPath = SCPPath(filespec);
	//LocalizedPath         = LocalizedPath.remove_filename() / LanguagePrefix / LocalizedPath.filename();
	//SELECT * FROM files WHERE Filename == filespec.string() and PathType == PathType and DIR_FILTER(LocationFlags, location_flags) == 1

	FileQueryBuilder QueryBuilder;
	QueryBuilder.Filename({ FileQueryBuilder::ConditionType::Equal, filespec.string() });
	if (PathType !=  SCPCFilePathTypeID::Any)
	{
		FileFinder().FilenameIs(filespec.string()).PathTypeIs(PathType).LocationMatches(location_flags);

		QueryBuilder.PathType({ FileQueryBuilder::ConditionType::Equal, PathType });
	}
	for (auto FileResult : CFileDatabase().Files(QueryBuilder)) 
	{
		//check location flags here
		// if we have a result we just want the first one that matches
		return FileResult;
	}
	//then search for the file name in the specific folder
	//Filename(filespec.string), PathType(pathtype)
	//and if a location_flags set was specified then filter those results
	//then if localized version not found
	//do the same with the un localized name
	return CFileLocation();
}

// -- from parselo.cpp --
extern char *stristr(char *str, const char *substr);

/**
 * Searches for a file.
 *
 * @note Follows all rules and precedence and searches CD's and pack files. Searches all locations in order for first filename using filter list.
 * @note This function is exponentially slow, so don't use it unless truely needed
 *
 * @param filename      Filename & extension
 * @param ext_num       Number of extensions to look for
 * @param ext_list      Extension filter list
 * @param pathtype      See CF_TYPE_ defines in CFILE.H
 * @param max_out       Maximum string length that should be stuffed into pack_filename
 * @param localize      Undertake localization
 *
 * @return A structure containing information about the found file
 */
CFileLocationExt cf_find_file_location_ext( const char* filename, const int ext_num, const char** ext_list, int pathtype, bool localize /*= false*/, SCP_string LanguagePrefix /*= ""*/)
{
	int cur_ext, i;
    uint ui;
	int cfs_slow_search = 0;
	char longname[MAX_PATH_LEN];
	char filespec[MAX_FILENAME_LEN];
	char *p = NULL;
	
	Assert( (filename != NULL) && (strlen(filename) < MAX_FILENAME_LEN) );
	Assert( (ext_list != NULL) && (ext_num > 1) );	// if we are searching for just one ext
													// then this is the wrong function to use


	// if we have a full path already then fail.  this function if for searching via filter only!
#ifdef SCP_UNIX
	if ( strpbrk(filename, "/") ) {			// do we have a full path already?
#else
	if ( strpbrk(filename,"/\\:")  ) {		// do we have a full path already?
#endif
		Int3();
		return CFileLocationExt();
	}

	// Search the hard drive for files first.
	uint num_search_dirs = 0;
	int search_order[CF_MAX_PATH_TYPES];

	if ( CF_TYPE_SPECIFIED(pathtype) )	{
		search_order[num_search_dirs++] = pathtype;
	} else {
		for (i = CF_TYPE_ROOT; i < CF_MAX_PATH_TYPES; i++)
			search_order[num_search_dirs++] = i;
	}

	memset( longname, 0, sizeof(longname) );
	memset( filespec, 0, sizeof(filespec) );

	// strip any existing extension
	strncpy(filespec, filename, MAX_FILENAME_LEN-1);

	for (ui = 0; ui < num_search_dirs; ui++) {
		// always hit the disk if we are looking in only one path
		if (num_search_dirs == 1) {
			cfs_slow_search = 1;
		}
		// otherwise hit based on a directory type
		else {
			switch (search_order[ui])
			{
				case CF_TYPE_ROOT:
				case CF_TYPE_DATA:
				case CF_TYPE_SINGLE_PLAYERS:
				case CF_TYPE_MULTI_PLAYERS:
				case CF_TYPE_MULTI_CACHE:
				case CF_TYPE_MISSIONS:
				case CF_TYPE_CACHE:
					cfs_slow_search = 1;
					break;
			}
		}

		if ( !cfs_slow_search )
			continue;

		for (cur_ext = 0; cur_ext < ext_num; cur_ext++) {
			// strip any extension and add the one we want to check for
			// (NOTE: to be fully retail compatible, we need to support multiple periods for something like *_1.5.wav,
			//        which means that we need to strip a length of >2 only, assuming that all valid ext are at least 2 chars)
			p = strrchr(filespec, '.');
			if ( p && (strlen(p) > 2) )
				(*p) = 0;

			strcat_s( filespec, ext_list[cur_ext] );
 
			GetDefaultFilePath( longname, sizeof(longname)-1, search_order[ui], filespec, localize, (uint32_t)CF_LOCATION_ALL, LanguagePrefix );

#if defined _WIN32
			_finddata_t findstruct;

			intptr_t findhandle = _findfirst(longname, &findstruct);
			if (findhandle != -1) {
				CFileLocationExt res(cur_ext);
				res.found = true;
				res.size = static_cast<size_t>(findstruct.size);

				_findclose(findhandle);

				res.offset = 0;
				res.full_name = longname;

				return res;
			}
#endif
			{
				FILE *fp = fopen(longname, "rb" );

				if (fp) {
					CFileLocationExt res(cur_ext);
					res.found = true;
					res.size = static_cast<size_t>(filelength( fileno(fp) ));

					fclose(fp);

					res.offset = 0;
					res.full_name = longname;

					return res;
				}
			}
		}
	}

	// Search the pak files and CD-ROM.

	// first off, make sure that we don't have an extension
	// (NOTE: to be fully retail compatible, we need to support multiple periods for something like *_1.5.wav,
	//        which means that we need to strip a length of >2 only, assuming that all valid ext are at least 2 chars)
	p = strrchr(filespec, '.');
	if ( p && (strlen(p) > 2) )
		(*p) = 0;

	// go ahead and get our length, which is used to test with later
	size_t filespec_len = strlen(filespec);

	// get total legnth, with extension, which is iused to test with later
	// (FIXME: this assumes that everything in ext_list[] is the same length!)
	size_t filespec_len_big = filespec_len + strlen(ext_list[0]);

	SCP_vector< SCPCFileInfo* > file_list_index;
	int last_root_index = -1;
	int last_path_index = -1;

	file_list_index.reserve( MIN(ext_num * 4, (int)Num_files) );

	// next, run though and pick out base matches
	for (ui = 0; ui < Num_files; ui++) {
		SCPCFileInfo *f = cf_get_file(ui);

		// ... only search paths that we're supposed to
		if ( (num_search_dirs == 1) && (pathtype != f->pathtype_index) )
			continue;

		// ... check that our names are the same length (accounting for the missing extension on our own name)
		if ( strlen(f->name_ext) != filespec_len_big )
			continue;

		// ... check that we match the base filename
		if ( strnicmp(f->name_ext, filespec, filespec_len) != 0 )
			continue;

		// ... make sure that it's one of our supported types
		bool found_one = false;
		for (cur_ext = 0; cur_ext < ext_num; cur_ext++) {
			if ( stristr(f->name_ext, ext_list[cur_ext]) ) {
				found_one = true;
				break;
			}
		}

		if ( !found_one )
			continue;

		// ... we check based on location, so if location changes after the first find then bail
		if (last_root_index == -1) {
			last_root_index = f->root_index;
			last_path_index = f->pathtype_index;
		} else {
			if (f->root_index != last_root_index)
				break;

			if (f->pathtype_index != last_path_index)
				break;
		}

		// ok, we have a good base match, so add it to our cache
		file_list_index.push_back( f );
	}

	// now try and find our preferred match
	for (cur_ext = 0; cur_ext < ext_num; cur_ext++) {
		for (SCP_vector<SCPCFileInfo*>::iterator fli = file_list_index.begin(); fli != file_list_index.end(); ++fli) {
			SCPCFileInfo *f = *fli;
	
			strcat_s( filespec, ext_list[cur_ext] );

			if (localize) {
				// create localized filespec
				
				SCPPath LocalizedPath = SCPPath(filespec);
				LocalizedPath         = LocalizedPath.remove_filename() / LanguagePrefix / LocalizedPath.filename();

				if ( !stricmp(LocalizedPath.c_str(), f->name_ext) ) {
					CFileLocationExt res(cur_ext);
					res.found = true;
					res.size = static_cast<size_t>(f->size);
					res.offset = (size_t)f->pack_offset;
					res.data_ptr = f->data;

					if (f->data != nullptr) {
						// This is an in-memory file so we just copy the pathtype name + file name
						res.full_name = Pathtypes[f->pathtype_index].path;
						res.full_name += DIR_SEPARATOR_STR;
						res.full_name += f->name_ext;
					} else if (f->pack_offset < 1) {
						// This is a real file, return the actual file path
						res.full_name = f->real_name;
					} else {
						// File is in a pack file
						SCPRootInfo *r = cf_get_root(f->root_index);

						res.full_name = r->path;
					}

					// found it, so cleanup and return
					file_list_index.clear();

					return res;
				}
			}

			// file either not localized or localized version not found
			if ( !stricmp(filespec, f->name_ext) ) {
				CFileLocationExt res(cur_ext);
				res.found = true;
				res.size = static_cast<size_t>(f->size);
				res.offset = (size_t)f->pack_offset;
				res.data_ptr = f->data;

				if (f->data != nullptr) {
					// This is an in-memory file so we just copy the pathtype name + file name
					res.full_name = Pathtypes[f->pathtype_index].path;
					res.full_name += DIR_SEPARATOR_STR;
					res.full_name += f->name_ext;
				} else if (f->pack_offset < 1) {
					// This is a real file, return the actual file path
					res.full_name = f->real_name;
				} else {
					// File is in a pack file
					SCPRootInfo *r = cf_get_root(f->root_index);

					res.full_name = r->path;
				}

				// found it, so cleanup and return
				file_list_index.clear();

				return res;
			}

			// ok, we're still here, so strip off the extension again in order to
			// prepare for the next run
			p = strrchr(filespec, '.');
			if ( p && (strlen(p) > 2) )
				(*p) = 0;
		}
	}

	return CFileLocationExt();
}


// Returns true if filename matches filespec, else zero if not
int cf_matches_spec(const char *filespec, const char *filename)
{
	const char *src_ext;
	const char *dst_ext;

	src_ext = strrchr(filespec, '*');
	if(!src_ext)
	{
		src_ext = strrchr(filespec, '.');
		if (!src_ext)
			return 1;
	}
	else
	{
		src_ext++;
	}

	if(strlen(filespec) > strlen(filename))
	{
		return 0;
	}

	dst_ext = filename + strlen(filename) - ((filespec + strlen(filespec)) - src_ext);
	if (!dst_ext)
		return 1;
	
	if(src_ext == filespec)
	{
		return !stricmp(dst_ext, src_ext);
	}
	else
	{
		return (!stricmp(dst_ext, src_ext) && !strnicmp(dst_ext, src_ext, src_ext - filespec));
	}
}

int (*Get_file_list_filter)(const char *filename) = NULL;
const char *Get_file_list_child = NULL;
int Skip_packfile_search = 0;
bool Skip_memory_files = false;

static bool verify_file_list_child()
{
	if (Get_file_list_child == NULL) {
		return false;
	}

	// empty or too long
	size_t len = strlen(Get_file_list_child);
	if ( !len || (len > MAX_FILENAME_LEN) ) {
		return false;
	}

	// can not being with directory separator
	if (Get_file_list_child[0] == DIR_SEPARATOR_CHAR) {
		return false;
	}

	// no ':' or spaces
	if ( strchr(Get_file_list_child, ':') || strchr(Get_file_list_child, ' ') ) {
		return false;
	}

	return true;
}

static int cf_file_already_in_list( SCP_vector<SCP_string> &list, const char *filename )
{
	char name_no_extension[MAX_PATH_LEN];
	size_t i, size = list.size();

	if (size == 0) {
		return 0;
	}

	strcpy_s(name_no_extension, filename );
	char *p = strrchr( name_no_extension, '.' );
	if ( p ) *p = 0;

	for (i = 0; i < size; i++) {
		if ( !stricmp(list[i].c_str(), name_no_extension ) ) {
			// Match found!
			return 1;
		}
	}

	// Not found
	return 0;
}

// An alternative cf_get_file_list*(), true dynamic list version.
// This one has a 'type', which is a CF_TYPE_* value.  Because this specifies the directory
// location, 'filter' only needs to be the filter itself, with no path information.
// See above descriptions of cf_get_file_list() for more information about how it all works.
// Note that filesystem listing is always sorted by name *before* sorting by the
// provided sort order. This isn't strictly needed on NTFS, which always provides the list
// sorted by name. But on mac/linux it's always needed.
int cf_get_file_list(SCP_vector<SCP_string>& list, int pathtype, const char* filter, int sort,
                     SCP_vector<file_list_info>* info, uint32_t location_flags)
{
	char *ptr;
	uint i;
	int own_flag = 0;
	size_t l;
	SCP_vector<file_list_info> my_info;
	file_list_info tinfo;

	if ( !info && (sort == CF_SORT_TIME) ) {
		info = &my_info;
		own_flag = 1;
	}

	char filespec[MAX_PATH_LEN];

	bool check_duplicates = !list.empty();

	if ( check_duplicates && (sort != CF_SORT_NONE) ) {
		Int3();
		sort = CF_SORT_NONE;
	}

	if (Get_file_list_child && !verify_file_list_child() ) {
		Get_file_list_child = NULL;
	}

#if defined _WIN32
	GetDefaultFilePath(filespec, sizeof(filespec) - 1, pathtype, (char*)Get_file_list_child, false,
	                              location_flags);
	strcat_s(filespec, DIR_SEPARATOR_STR);
	strcat_s(filespec, filter);

	_finddata_t find;
	intptr_t find_handle;

	find_handle = _findfirst( filespec, &find );
	if (find_handle != -1) {
		do {
            if (strcmp(strrchr(filter, '.'), strrchr(find.name,'.')) != 0)
                continue;

			if (!(find.attrib & _A_SUBDIR)) {
				if ( !Get_file_list_filter || (*Get_file_list_filter)(find.name) ) {
					if ( check_duplicates && cf_file_already_in_list(list, find.name) ) {
						continue;
					}

					ptr = strrchr(find.name, '.');
					if (ptr)
						l = (size_t)(ptr - find.name);
					else
						l = strlen(find.name);

					list.push_back( SCP_string(find.name, l) );

					if (info) {
						tinfo.write_time = find.time_write;
						info->push_back( tinfo );
					}
				}
			}

		} while (!_findnext(find_handle, &find));

		_findclose( find_handle );
	}

#elif defined SCP_UNIX
	GetDefaultFilePath(filespec, sizeof(filespec) - 1, pathtype, (char*)Get_file_list_child, false,
	                              location_flags);

	DIR *dirp;
	struct dirent *dir;

	dirp = opendir (filespec);
	if ( dirp ) {
		while ((dir = readdir (dirp)) != NULL) {

			if (fnmatch(filter, dir->d_name, 0) != 0)
				continue;

			char fn[MAX_PATH];
			if (snprintf(fn, MAX_PATH, "%s/%s", filespec, dir->d_name) >= MAX_PATH) {
				// Make sure the string is null terminated
				fn[MAX_PATH-1] = 0;
			}

			struct stat buf;
			if (stat(fn, &buf) == -1) {
				continue;
			}

			if (!S_ISREG(buf.st_mode)) {
				continue;
			}

			if ( !Get_file_list_filter || (*Get_file_list_filter)(dir->d_name) ) {
				if ( check_duplicates && cf_file_already_in_list(list, dir->d_name) ) {
					continue;
				}

				ptr = strrchr(dir->d_name, '.');
				if (ptr)
					l = (size_t)(ptr - dir->d_name);
				else
					l = strlen(dir->d_name);

				list.push_back( SCP_string(dir->d_name, l) );

				if (info) {
					tinfo.write_time = buf.st_mtime;
					info->push_back( tinfo );
				}
			}
		}

		closedir(dirp);
	}
#endif

	// tcrayford: sort the filesystem listing by name by default to ensure
	// a stable order across filesystems
	cf_sort_filenames( list, CF_SORT_NAME, info );

	bool skip_packfiles = false;
	if ( (pathtype == CF_TYPE_PLAYERS) || (pathtype == CF_TYPE_SINGLE_PLAYERS) || (pathtype == CF_TYPE_MULTI_PLAYERS) ) {
		skip_packfiles = true;
	} else if (Get_file_list_child != NULL) {
		skip_packfiles = true;
	}

	// Search all the packfiles and CD.
	if ( !skip_packfiles )	{
		for (i=0; i<Num_files; i++ )	{
			SCPCFileInfo * f = cf_get_file(i);

			// only search paths we're supposed to...
			if ( (pathtype != CF_TYPE_ANY) && (pathtype != f->pathtype_index)  )	{
				continue;
			}

			if (location_flags != CF_LOCATION_ALL) {
				// If a location flag was specified we need to check if the root of this file satisfies the request
				auto root = cf_get_root(f->root_index);

				if (!CheckLocationFlags(root->location_flags, location_flags)) {
					// Root does not satisfy location flags
					continue;
				}
			}

			if ( !cf_matches_spec( filter,f->name_ext))	{
				continue;
			}

			if ( cf_file_already_in_list(list, f->name_ext))	{
				continue;
			}

			if (Skip_packfile_search && f->pack_offset != 0) {
				// If the packfile skip flag is set we skip files in VPs but still search in directories
				continue;
			}

			if (Skip_memory_files && f->data != nullptr) {
				// If we want to skip memory files and this is a memory file then ignore it
				continue;
			}

			if ( !Get_file_list_filter || (*Get_file_list_filter)(f->name_ext) ) {
				//mprintf(( "Found '%s' in root %d path %d\n", f->name_ext, f->root_index, f->pathtype_index ));

				ptr = strrchr(f->name_ext, '.');
				if (ptr)
					l = (size_t)(ptr - f->name_ext);
				else
					l = strlen(f->name_ext);

				list.push_back( SCP_string(f->name_ext, l) );

				if (info) {
					tinfo.write_time = f->write_time;
					info->push_back( tinfo );
				}
			}

		}
	}

	if (sort != CF_SORT_NONE)	{
		cf_sort_filenames( list, sort, info );
	}

	if (own_flag)	{
		my_info.clear();
	}

	Get_file_list_filter = NULL;
	Get_file_list_child = NULL;

	return (int)list.size();
}

int cf_file_already_in_list( int num_files, char **list, const char *filename )
{
	int i;

	char name_no_extension[MAX_PATH_LEN];

	strcpy_s(name_no_extension, filename );
	char *p = strrchr( name_no_extension, '.' );
	if ( p ) *p = 0;

	for (i=0; i<num_files; i++ )	{
		if ( !stricmp(list[i], name_no_extension ) )	{
			// Match found!
			return 1;
		}
	}
	// Not found
	return 0;
}

// An alternative cf_get_file_list(), dynamic list version.
// This one has a 'type', which is a CF_TYPE_* value.  Because this specifies the directory
// location, 'filter' only needs to be the filter itself, with no path information.
// See above descriptions of cf_get_file_list() for more information about how it all works.
int cf_get_file_list(int max, char** list, int pathtype, const char* filter, int sort, file_list_info* info,
                     uint32_t location_flags)
{
	char *ptr;
	uint i;
	int num_files = 0, own_flag = 0;
	size_t l;

	if (max < 1) {
		Get_file_list_filter = NULL;

		return 0;
	}

	Assert(list);

	if (!info && (sort == CF_SORT_TIME)) {
		info = (file_list_info *) vm_malloc(sizeof(file_list_info) * max);
		own_flag = 1;
	}

	char filespec[MAX_PATH_LEN];

#if defined _WIN32
	GetDefaultFilePath(filespec, sizeof(filespec) - 1, pathtype, filter, false, location_flags);

	_finddata_t find;
	intptr_t find_handle;

	find_handle = _findfirst( filespec, &find );
	if (find_handle != -1) {
		do {
			if (num_files >= max)
				break;

            if (strcmp(strrchr(filter, '.'), strrchr(find.name,'.')) != 0)
                continue;

			if ( strlen(find.name) >= MAX_FILENAME_LEN )
				continue;

			if (!(find.attrib & _A_SUBDIR)) {
				if ( !Get_file_list_filter || (*Get_file_list_filter)(find.name) ) {
					ptr = strrchr(find.name, '.');
					if (ptr)
						l = ptr - find.name;
					else
						l = strlen(find.name);

					list[num_files] = (char *)vm_malloc(l + 1);
					strncpy(list[num_files], find.name, l);
					list[num_files][l] = 0;
					if (info)
						info[num_files].write_time = find.time_write;

					num_files++;
				}
			}

		} while (!_findnext(find_handle, &find));

		_findclose( find_handle );
	}

#elif defined SCP_UNIX
	GetDefaultFilePath(filespec, sizeof(filespec) - 1, pathtype, nullptr, false, location_flags);

	DIR *dirp;
	struct dirent *dir;

	dirp = opendir (filespec);
	if ( dirp ) {
		while ((dir = readdir (dirp)) != NULL)
		{
			if (num_files >= max)
				break;

			if ( strlen(dir->d_name) >= MAX_FILENAME_LEN ) {
				continue;
			}

			if (fnmatch(filter, dir->d_name, 0) != 0)
				continue;

			char fn[MAX_PATH];
			if (snprintf(fn, MAX_PATH, "%s/%s", filespec, dir->d_name) >= MAX_PATH) {
				// Make sure the string is null terminated
				fn[MAX_PATH-1] = 0;
			}

			struct stat buf;
			if (stat(fn, &buf) == -1) {
				continue;
			}

			if (!S_ISREG(buf.st_mode)) {
				continue;
			}

			if ( !Get_file_list_filter || (*Get_file_list_filter)(dir->d_name) ) {
				ptr = strrchr(dir->d_name, '.');
				if (ptr)
					l = ptr - dir->d_name;
				else
					l = strlen(dir->d_name);

				list[num_files] = (char *)vm_malloc(l + 1);
				strncpy(list[num_files], dir->d_name, l);
				list[num_files][l] = 0;
				if (info)
					info[num_files].write_time = buf.st_mtime;

				num_files++;
			}
		}

		closedir(dirp);
	}
#endif

	bool skip_packfiles = false;
	if ((pathtype == CF_TYPE_PLAYERS) || (pathtype == CF_TYPE_SINGLE_PLAYERS) || (pathtype == CF_TYPE_MULTI_PLAYERS)) {
		skip_packfiles = true;
	}
	else if (Get_file_list_child != NULL) {
		skip_packfiles = true;
	}

	// Search all the packfiles and CD.
	if ( !skip_packfiles)	{
		for (i=0; i<Num_files; i++ )	{
			SCPCFileInfo * f = cf_get_file(i);

			// only search paths we're supposed to...
			if ( (pathtype != CF_TYPE_ANY) && (pathtype != f->pathtype_index)  )	{
				continue;
			}

			if (location_flags != CF_LOCATION_ALL) {
				// If a location flag was specified we need to check if the root of this file satisfies the request
				auto root = cf_get_root(f->root_index);

				if (!CheckLocationFlags(root->location_flags, location_flags)) {
					// Root does not satisfy location flags
					continue;
				}
			}

			if (num_files >= max)
				break;
			
			if ( !cf_matches_spec( filter,f->name_ext))	{
				continue;
			}

			if ( cf_file_already_in_list(num_files,list,f->name_ext))	{
				continue;
			}

			if (Skip_packfile_search && f->pack_offset != 0) {
				// If the packfile skip flag is set we skip files in VPs but still search in directories
				continue;
			}

			if (Skip_memory_files && f->data != nullptr) {
				// If we want to skip memory files and this is a memory file then ignore it
				continue;
			}

			if ( !Get_file_list_filter || (*Get_file_list_filter)(f->name_ext) ) {

				//mprintf(( "Found '%s' in root %d path %d\n", f->name_ext, f->root_index, f->pathtype_index ));

					ptr = strrchr(f->name_ext, '.');
					if (ptr)
						l = ptr - f->name_ext;
					else
						l = strlen(f->name_ext);

					list[num_files] = (char *)vm_malloc(l + 1);
					strncpy(list[num_files], f->name_ext, l);
					list[num_files][l] = 0;

				if (info)	{
					info[num_files].write_time = f->write_time;
				}

				num_files++;
			}

		}
	}


	if (sort != CF_SORT_NONE)	{
		cf_sort_filenames( num_files, list, sort, info );
	}

	if (own_flag)	{
		vm_free(info);
	}

	Get_file_list_filter = NULL;

	return num_files;
}

int cf_file_already_in_list_preallocated( int num_files, char arr[][MAX_FILENAME_LEN], const char *filename )
{
	int i;

	char name_no_extension[MAX_PATH_LEN];

	strcpy_s(name_no_extension, filename );
	char *p = strrchr( name_no_extension, '.' );
	if ( p ) *p = 0;

	for (i=0; i<num_files; i++ )	{
		if ( !stricmp(arr[i], name_no_extension ) )	{
			// Match found!
			return 1;
		}
	}
	// Not found
	return 0;
}

// An alternative cf_get_file_list(), fixed array version.
// This one has a 'type', which is a CF_TYPE_* value.  Because this specifies the directory
// location, 'filter' only needs to be the filter itself, with no path information.
// See above descriptions of cf_get_file_list() for more information about how it all works.
int cf_get_file_list_preallocated(int max, char arr[][MAX_FILENAME_LEN], char** list, int pathtype, const char* filter,
                                  int sort, file_list_info* info, uint32_t location_flags)
{
	int num_files = 0, own_flag = 0;

	if (max < 1) {
		Get_file_list_filter = NULL;

		return 0;
	}

	if (list) {
		for (int i=0; i<max; i++)	{
			list[i] = arr[i];
		}
	} else {
		sort = CF_SORT_NONE;  // sorting of array directly not supported.  Sorting done on list only
	}

	if (!info && (sort == CF_SORT_TIME)) {
		info = (file_list_info *) vm_malloc(sizeof(file_list_info) * max);

		if ( info )
			own_flag = 1;
	}

	char filespec[MAX_PATH_LEN];

	// Search the default directories
#if defined _WIN32
	GetDefaultFilePath(filespec, sizeof(filespec) - 1, pathtype, filter, false, location_flags);

	intptr_t find_handle;
	_finddata_t find;
	
	find_handle = _findfirst( filespec, &find );
	if (find_handle != -1) {
		do {
			if (num_files >= max)			
				break;

			if (!(find.attrib & _A_SUBDIR)) {
            
                if (strcmp(strstr(filter, "."), strstr(find.name,".")) != 0)
                    continue;

				if ( strlen(find.name) >= MAX_FILENAME_LEN )
					continue;

				if ( !Get_file_list_filter || (*Get_file_list_filter)(find.name) ) {

					strncpy(arr[num_files], find.name, MAX_FILENAME_LEN - 1 );
					char *ptr = strrchr(arr[num_files], '.');
					if ( ptr ) {
						*ptr = 0;
					}

					if (info)	{
						info[num_files].write_time = find.time_write;
					}

					num_files++;
				}
			}

		} while (!_findnext(find_handle, &find));

		_findclose( find_handle );
	}

#elif defined SCP_UNIX
	GetDefaultFilePath(filespec, sizeof(filespec) - 1, pathtype, nullptr, false, location_flags);

	DIR *dirp;
	struct dirent *dir;

	dirp = opendir (filespec);
	if ( dirp ) {
		while ((dir = readdir (dirp)) != NULL)
		{
			if (num_files >= max)
				break;

			if (fnmatch(filter, dir->d_name, 0) != 0)
				continue;

			char fn[MAX_PATH];
			if (snprintf(fn, MAX_PATH, "%s/%s", filespec, dir->d_name) >= MAX_PATH) {
				// Make sure the string is null terminated
				fn[MAX_PATH-1] = 0;
			}

			struct stat buf;
			if (stat(fn, &buf) == -1) {
				continue;
			}

			if (!S_ISREG(buf.st_mode)) {
				continue;
			}

			if ( strlen(dir->d_name) >= MAX_FILENAME_LEN ) {
				continue;
			}

			if ( !Get_file_list_filter || (*Get_file_list_filter)(dir->d_name) ) {

				strcpy_s(arr[num_files], dir->d_name );
				char *ptr = strrchr(arr[num_files], '.');
				if ( ptr ) {
					*ptr = 0;
				}

				if (info)	{
					info[num_files].write_time = buf.st_mtime;
				}

				num_files++;
			}
		}
		closedir(dirp);
	}
#endif

	bool skip_packfiles = false;
	if ((pathtype == CF_TYPE_PLAYERS) || (pathtype == CF_TYPE_SINGLE_PLAYERS) || (pathtype == CF_TYPE_MULTI_PLAYERS)) {
		skip_packfiles = true;
	}
	else if (Get_file_list_child != NULL) {
		skip_packfiles = true;
	}

	// Search all the packfiles and CD.
	if (!skip_packfiles) {
		for (uint i=0; i<Num_files; i++ )	{
			SCPCFileInfo * f = cf_get_file(i);

			// only search paths we're supposed to...
			if ( (pathtype != CF_TYPE_ANY) && (pathtype != f->pathtype_index)  )	{
				continue;
			}

			if (location_flags != CF_LOCATION_ALL) {
				// If a location flag was specified we need to check if the root of this file satisfies the request
				auto root = cf_get_root(f->root_index);

				if (!CheckLocationFlags(root->location_flags, location_flags)) {
					// Root does not satisfy location flags
					continue;
				}
			}

			if (num_files >= max)
						
				break;

			if ( !cf_matches_spec( filter,f->name_ext))	{
				continue;
			}

			if ( cf_file_already_in_list_preallocated( num_files, arr, f->name_ext ))	{
				continue;
			}

			if (Skip_packfile_search && f->pack_offset != 0) {
				// If the packfile skip flag is set we skip files in VPs but still search in directories
				continue;
			}

			if (Skip_memory_files && f->data != nullptr) {
				// If we want to skip memory files and this is a memory file then ignore it
				continue;
			}

			if ( !Get_file_list_filter || (*Get_file_list_filter)(f->name_ext) ) {

				//mprintf(( "Found '%s' in root %d path %d\n", f->name_ext, f->root_index, f->pathtype_index ));

				strncpy(arr[num_files], f->name_ext, MAX_FILENAME_LEN - 1 );
				char *ptr = strrchr(arr[num_files], '.');
				if ( ptr ) {
					*ptr = 0;
				}

				if (info)	{
					info[num_files].write_time = f->write_time;
				}

				num_files++;
			}

		}
	}

	if (sort != CF_SORT_NONE) {
		Assert(list);
		cf_sort_filenames( num_files, list, sort, info );
	}

	if (own_flag)	{
		vm_free(info);
	}

	Get_file_list_filter = NULL;

	return num_files;
}

// Returns the default storage path for files given a 
// particular pathtype.   In other words, the path to 
// the unpacked, non-cd'd, stored on hard drive path.
// If filename isn't null it will also tack the filename
// on the end, creating a completely valid filename.
// Input:   pathtype  - CF_TYPE_??
//			path_max  - Maximum characters in the path
//          filename  - optional, if set, tacks the filename onto end of path.
// Output:  path      - Fully qualified pathname.
//Returns 0 if the result would be too long (invalid result)
//int cf_create_default_path_string(char* path, uint path_max, int pathtype, const char* filename, bool localize,
//                                  uint32_t location_flags, SCP_string LanguagePrefix)


// Returns the default storage path for files given a 
// particular pathtype.   In other words, the path to 
// the unpacked, non-cd'd, stored on hard drive path.
// If filename isn't null it will also tack the filename
// on the end, creating a completely valid filename.
// Input:   pathtype  - CF_TYPE_??
//          filename  - optional, if set, tacks the filename onto end of path.
// Output:  path      - Fully qualified pathname.
//Returns 0 if the result would be too long (invalid result)

void cfile_spew_pack_file_crcs()
{
	int i;
	char datetime[45];
	uint chksum = 0;
	time_t my_time;
	
	FILE *out = fopen(GConfig->GetConfigPath("vp_crcs.txt").c_str(), "w");

	if (out == NULL) {
		Int3();
		return;
	}

	my_time = time(NULL);

	memset( datetime, 0, sizeof(datetime) );
	snprintf(datetime, sizeof(datetime)-1, "%s", ctime(&my_time));
	// ctime() adds a newline char, so we have to strip it off
	datetime[strlen(datetime)-1] = '\0';

	fprintf(out, "Pack file CRC log (%s) ... \n", datetime);
	fprintf(out, "-------------------------------------------------------------------------------\n");

	for (i = 0; i < Num_roots; i++) {
		SCPRootInfo *cur_root = cf_get_root(i);

		if (cur_root->roottype != CF_ROOTTYPE_PACK)
			continue;

		chksum = 0;
		cf_chksum_pack(cur_root->path, &chksum, true);

		fprintf(out, "  %s  --  0x%x\n", cur_root->path, chksum);
	}

	fprintf(out, "-------------------------------------------------------------------------------\n");

	fclose(out);
}


