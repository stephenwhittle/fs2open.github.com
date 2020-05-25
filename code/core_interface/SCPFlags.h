#pragma once

#include "FSIntegerTypes.h"

//def_list
struct flag_def_list {
	const char *name;
	int def;
	ubyte var;
};

template<class T>
struct flag_def_list_new {
    const char* name;			// The parseable representation of this flag
    T def;				// The flag definition for this flag
    bool in_use;		// Whether or not this flag is currently in use or obsolete
    bool is_special;	// Whether this flag requires special processing. See parse_string_flag_list<T, T> for details
};
