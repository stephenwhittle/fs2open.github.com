/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on the 
 * source.
 *
*/ 



#ifndef _CFILEARCHIVE_H
#define _CFILEARCHIVE_H

/*
#ifndef _CFILE_INTERNAL 
#error This file should only be included internally in CFILE!!
#endif*/

// The following Cfile_block data is private to cfile.cpp
// DO NOT MOVE the Cfile_block* information to cfile.h / do not extern this data
//



// Called once to setup the low-level reading code.
void cf_init_lowlevel_read_code( CFILE * cfile, size_t lib_offset, size_t size, size_t pos );

#endif
