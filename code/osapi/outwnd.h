/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on the 
 * source.
 *
*/

#ifndef _OUTWND_H
#define _OUTWND_H

#include "globalincs/pstypes.h"

void load_filter_info();
void outwnd_init();
void outwnd_close();
void outwnd_printf(const char *id, SCP_FORMAT_STRING const char *format, ...) SCP_FORMAT_STRING_ARGS(2, 3);
void outwnd_printf2(SCP_FORMAT_STRING const char *format, ...) SCP_FORMAT_STRING_ARGS(1, 2);
void outwnd_print(const char* id = nullptr, const char* temp = nullptr);
void outwnd_debug_window_init();
void outwnd_debug_window_do_frame(float frametime);
void outwnd_debug_window_deinit();

extern bool Log_debug_output_to_file;

#endif	// _OUTWND_H
