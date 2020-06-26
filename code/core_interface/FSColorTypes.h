#pragma once

#include "FSIntegerTypes.h"
#include "FSMathOps.h"

#define AC_TYPE_NONE 0  // Not an alphacolor
#define AC_TYPE_HUD 1   // Doesn't change hue depending on background.  Used for HUD stuff.
#define AC_TYPE_BLEND 2 // Changes hue depending on background.  Used for stars, etc.



// NEVER REFERENCE THESE VALUES OUTSIDE OF THE GRAPHICS LIBRARY!!!
// If you need to get the rgb values of a "color" struct call
// gr_get_colors after calling gr_set_colors_fast.
class color{
public:
	//uint		screen_sig;
	int		is_alphacolor;
	int		alphacolor;
	int		magic;
	ubyte		red;
	ubyte		green;
	ubyte		blue;
	ubyte		alpha;
	ubyte		ac_type;							// The type of alphacolor.  See AC_TYPE_??? defines
	ubyte		raw8;
	color(int Red, int Green, int Blue, int Alpha, int type = AC_TYPE_NONE)
	{
		CAP(Red, 0, 255);
		CAP(Green, 0, 255);
		CAP(Blue, 0, 255);
		CAP(Alpha, 0, 255);

		//c->screen_sig      = gr_screen.signature;
		red             = (ubyte)Red;
		green           = (ubyte)Green;
		blue            = (ubyte)Blue;
		alpha           = 255;
		magic           = 0xAC01;
		raw8            = 0;
		alpha         = (ubyte)Alpha;
		ac_type       = (ubyte)type;
		alphacolor    = -1;
		is_alphacolor = 1;
	}
	color()
		:color(0, 0, 0, 0)
	{
		
	}
};

// Used by the team coloring code
typedef struct team_color {
	struct {
		float r, g, b;
	} base;
	struct {
		float r, g, b;
	} stripe;
} team_color;
