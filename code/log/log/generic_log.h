/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on the 
 * source.
 *
*/



#ifndef _FREESPACE_LOGFILE_HEADER_FILE
#define _FREESPACE_LOGFILE_HEADER_FILE

// ----------------------------------------------------------------------------------------------------
// LOGFILE DEFINES/VARS
//
#define LOGFILE_MULTI_LOG		0
#define LOGFILE_EVENT_LOG		1

#include "fmt/printf.h"
// ----------------------------------------------------------------------------------------------------
// MULTI LOGFILE FUNCTIONS
//

// initialize the multi logfile
bool logfile_init(int logfile_type);

// close down the multi logfile
void logfile_close(int logfile_type);

bool logfile_valid(int LogType);

template <typename... Args>
void log_printf(int LogType, const char* Format, const Args&... args)
{
	if (Format == NULL) {
		return;
	}
	if (!logfile_valid(LogType))
	{
		return;
	}

	log_string(LogType, fmt::sprintf(Message, Args).c_str());
}

void log_string(int LogType, const char* Message, int Time = 0);

#endif
