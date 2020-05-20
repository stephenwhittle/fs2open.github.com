#pragma once

#include "FSIntegerTypes.h"

// NEVER REFERENCE THESE VALUES OUTSIDE OF THE GRAPHICS LIBRARY!!!
// If you need to get the rgb values of a "color" struct call
// gr_get_colors after calling gr_set_colors_fast.
typedef struct color {
	uint		screen_sig;
	int		is_alphacolor;
	int		alphacolor;
	int		magic;
	ubyte		red;
	ubyte		green;
	ubyte		blue;
	ubyte		alpha;
	ubyte		ac_type;							// The type of alphacolor.  See AC_TYPE_??? defines
	ubyte		raw8;
} color;

// Used by the team coloring code
typedef struct team_color {
	struct {
		float r, g, b;
	} base;
	struct {
		float r, g, b;
	} stripe;
} team_color;
