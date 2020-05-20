/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell
 * or otherwise commercially exploit the source or things you created based on the
 * source.
 *
*/



#ifndef _PSTYPES_H
#define _PSTYPES_H



#include "windows_stub/config.h"
#include "globalincs/scp_defines.h"
#include "globalincs/toolchain.h"
#include "utils/strings.h"

#include <cstdio>    // For NULL, etc
#include <cstdlib>
#include <memory.h>
#include <cstring>
#include <algorithm>
#include <cstdint>

#include "core_interface/FSOutputDeviceBase.h"
#include "core_interface/FSMathTypes.h"
#include "core_interface/FSIntegerTypes.h"
#include "core_interface/FSAssert.h"

// value to represent an uninitialized state in any int or uint
#define UNINITIALIZED 0x7f8e6d9c

#define MAX_PLAYERS	12

#ifdef LOCAL
#undef LOCAL
#endif

#define LOCAL static			// make module local varilable static.

#ifdef _WIN32
#define DIR_SEPARATOR_CHAR '\\'
#define DIR_SEPARATOR_STR  "\\"
#else
#define DIR_SEPARATOR_CHAR '/'
#define DIR_SEPARATOR_STR  "/"
#endif

#ifndef NDEBUG
constexpr bool FSO_DEBUG = true;
#else
constexpr bool FSO_DEBUG = false;
#endif



//Stucture to store clipping codes in a word
typedef struct ccodes {
	ubyte cc_or, cc_and;		//or is low byte, and is high byte
} ccodes;

struct vertex;



/** Compares two uv_pairs */
inline bool operator==(const uv_pair &left, const uv_pair &right)
{
	return (left.u == right.u) && (left.v == right.v);
}

/** Represents a point in 3d screen space. 'w' is 1/z.

Like vec3d but for screens.

Note: this is a struct, not a class, so no member functions. */
typedef struct screen3d
{
	union {
		struct {
			float x,y,w;
		} xyw;
		float a1d[3];
	};
} screen3d;

/** Compares two screen3ds */
inline bool operator==(const screen3d &self, const screen3d &other)
{
	return (self.xyw.x == other.xyw.x
		&& self.xyw.y == other.xyw.y
		&& self.xyw.w == other.xyw.w
	);
}

/** Used to store rotated points for mines. Has flag to indicate if projected.

Note: this is a struct, not a class, so no memeber functions. */
typedef struct vertex {
	vec3d		world;				// world space position
	screen3d	screen;				// screen space position (sw == 1/z)
	uv_pair		texture_position;	// texture position
	ubyte		r, g, b, a;			// color.  Use b for darkening;
	ubyte		codes;				// what sides of view pyramid this point is on/off.  0 = Inside view pyramid.
	ubyte		flags;				// Projection flags.  Indicates whether it is projected or not or if projection overflowed.
	ubyte		pad[2];				// pad structure to be 4 byte aligned.
} vertex;

typedef struct effect_vertex {
	vec3d position;
	uv_pair tex_coord;
	float radius;
	ubyte r, g, b, a;
} effect_vertex;

struct particle_pnt {
	vec3d position;
	float size;
	vec3d up;
};

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

// weapon count list (mainly for pilot files)
typedef struct wep_t {
	int index;
	int count;
} wep_t;

typedef struct coord2d {
	int x,y;
} coord2d;

//#include "osapi/dialogs.h"




extern int Fred_running;  // Is Fred running, or FreeSpace?
extern bool running_unittests;

const size_t INVALID_SIZE = static_cast<size_t>(-1);

//======================================================================================
//======================================================================================
//======================================================================================


#include "math/fix.h"
#include "math/floating.h"

// Some constants for stuff
#define MAX_FILENAME_LEN	32		// Length for filenames, ie "title.pcx"
#define MAX_PATH_LEN		256		// Length for pathnames, ie "c:\bitmaps\title.pcx"

// contants and defined for byteswapping routines (useful for mac)

#ifdef SCP_SOLARIS // Solaris
#define INTEL_INT(x)	x
#define INTEL_LONG(x)   x
#define INTEL_SHORT(x)	x
#define INTEL_FLOAT(x)	(*x)
#elif BYTE_ORDER == BIG_ENDIAN
// turn off inline asm
#undef USE_INLINE_ASM

#define INTEL_INT(x)	SDL_Swap32(x)
#define INTEL_LONG(x)   SDL_Swap64(x)
#define INTEL_SHORT(x)	SDL_Swap16(x)
#define INTEL_FLOAT(x)	SDL_SwapFloat((*x))

#else // Little Endian -
#define INTEL_INT(x)	x
#define INTEL_LONG(x)   x
#define INTEL_SHORT(x)	x
#define INTEL_FLOAT(x)	(*x)
#endif // BYTE_ORDER





// lod checker for (modular) table parsing
typedef struct lod_checker {
	char filename[MAX_FILENAME_LEN];
	int num_lods;
	int override;
} lod_checker;


// Callback Loading function.
// If you pass a function to this, that function will get called
// around 10x per second, so you can update the screen.
// Pass NULL to turn it off.
// Call this with the name of a function.  That function will
// then get called around 10x per second.  The callback function
// gets passed a 'count' which is how many times game_busy has
// been called since the callback was set.   It gets called
// one last time with count=-1 when you turn off the callback
// by calling game_busy_callback(NULL).   Game_busy_callback
// returns the current count, so you can tell how many times
// game_busy got called.
// If delta_step is above 0, then it will also make sure it
// calls the callback each time count steps 'delta_step' even
// if 1/10th of a second hasn't elapsed.
extern int game_busy_callback( void (*callback)(int count), int delta_step = -1 );

// Call whenever loading to display cursor
extern void game_busy(const char *filename = NULL);

#define NOX(s) s

const char *XSTR(const char *str, int index);



//=========================================================
// Memory management functions
//=========================================================

#include "globalincs/fsmemory.h"

class camid
{
private:
	int sig;
	size_t idx;
public:
	camid();
	camid(size_t n_idx, int n_sig);

	class camera *getCamera();
	size_t getIndex();
	int getSignature();
	bool isValid();
};

#include "globalincs/vmallocator.h"
#include "globalincs/safe_strings.h"

// check to see that a passed sting is valid, ie:
//  - has >0 length
//  - is not "none"
//  - is not "<none>"
inline bool VALID_FNAME(const char* x) {
	return strlen((x)) && stricmp((x), "none") != 0 && stricmp((x), "<none>") != 0;
}
/**
 * @brief Checks if the specified string may be a valid file name
 *
 * @warning This only does a quick check against an empty string and a few known invalid names. It does not check if the
 * file actually exists.
 *
 * @param x The file name to check
 * @return @c true if the name is valid, @c false otherwise
 */
inline bool VALID_FNAME(const SCP_string& x) {
	if (x.empty()) {
		return false;
	}
	if (!stricmp(x.c_str(), "none")) {
		return false;
	}
	if (!stricmp(x.c_str(), "<none>")) {
		return false;
	}
	return true;
}

// Function to generate a stacktrace
SCP_string dump_stacktrace();

// DEBUG compile time catch for dangerous uses of memset/memcpy/memmove
// This is disabled for VS2013 and lower since that doesn't support the necessary features
#if !defined(NDEBUG) && !defined(USING_THIRD_PARTY_LIBS) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
	#if SCP_COMPILER_CXX_AUTO_TYPE && SCP_COMPILER_CXX_STATIC_ASSERT && HAVE_STD_IS_TRIVIALLY_COPYABLE
	// feature support seems to be: gcc   clang   msvc
	// auto                         4.4   2.9     2010
	// std::is_trivial              4.5   ?       2012 (2010 only duplicates std::is_pod)
	// static_assert                4.3   2.9     2010
	#include <type_traits>
	#include <cstring>

	// MEMSET!
	const auto ptr_memset = std::memset;
	#define memset memset_if_trivial_else_error

// Put into std to be compatible with code that uses std::mem*
namespace std
{
	template<typename T>
	using trivial_check = std::is_trivially_copyable<T>;

	template<typename T>
	void *memset_if_trivial_else_error(T *memset_data, int ch, size_t count)
	{
		static_assert(trivial_check<T>::value, "memset on non-trivial object");
		return ptr_memset(memset_data, ch, count);
	}

	// assume memset on a void* is "safe"
	// only used in cutscene/mveplayer.cpp:mve_video_createbuf()
	inline void *memset_if_trivial_else_error(void *memset_data, int ch, size_t count)
	{
		return ptr_memset(memset_data, ch, count);
	}

	// MEMCPY!
	const auto ptr_memcpy = std::memcpy;
	#define memcpy memcpy_if_trivial_else_error

	template<typename T, typename U>
	void *memcpy_if_trivial_else_error(T *memcpy_dest, U *src, size_t count)
	{
		static_assert(trivial_check<T>::value, "memcpy on non-trivial object T");
		static_assert(trivial_check<U>::value, "memcpy on non-trivial object U");
		return ptr_memcpy(memcpy_dest, src, count);
	}

	// assume memcpy with void* is "safe"
	// used in:
	//   globalincs/systemvars.cpp:insertion_sort()
	//   network/chat_api.cpp:AddChatCommandToQueue()
	//   network/multi_obj.cpp:multi_oo_sort_func()
	//   parse/lua.cpp:ade_get_args() && ade_set_args()
	//
	// probably should setup a static_assert on insertion_sort as well
	template<typename U>
	void *memcpy_if_trivial_else_error(void *memcpy_dest, U *memcpy_src, size_t count)
	{
		static_assert(trivial_check<U>::value, "memcpy on non-trivial object U");
		return ptr_memcpy(memcpy_dest, memcpy_src, count);
	}

	template<typename T>
	void *memcpy_if_trivial_else_error(T *memcpy_dest, void *memcpy_src, size_t count)
	{
		static_assert(trivial_check<T>::value, "memcpy on non-trivial object T");
		return ptr_memcpy(memcpy_dest, memcpy_src, count);
	}
	template<typename T>
	void *memcpy_if_trivial_else_error(T *memcpy_dest, const void *memcpy_src, size_t count)
	{
		static_assert(trivial_check<T>::value, "memcpy on non-trivial object T");
		return ptr_memcpy(memcpy_dest, memcpy_src, count);
	}

	inline void *memcpy_if_trivial_else_error(void *memcpy_dest, void *memcpy_src, size_t count)
	{
		return ptr_memcpy(memcpy_dest, memcpy_src, count);
	}

	// MEMMOVE!
	const auto ptr_memmove = std::memmove;
	#define memmove memmove_if_trivial_else_error

	template<typename T, typename U>
	inline void *memmove_if_trivial_else_error(T *memmove_dest, U *memmove_src, size_t count)
	{
		static_assert(trivial_check<T>::value, "memmove on non-trivial object T");
		static_assert(trivial_check<U>::value, "memmove on non-trivial object U");
		return ptr_memmove(memmove_dest, memmove_src, count);
	}

	// Not really needed but else clang thinks ptr_memmove isn't used
	inline void *memmove_if_trivial_else_error(void *memmove_dest, void *memmove_src, size_t count)
	{
		return ptr_memmove(memmove_dest, memmove_src, count);
	}
}
// Put into global namespace
using std::memcpy_if_trivial_else_error;
using std::memmove_if_trivial_else_error;
using std::memset_if_trivial_else_error;

	#endif // HAVE_CXX11
#endif // NDEBUG

#endif		// PS_TYPES_H
