
#pragma once
#include "FSIntegerTypes.h"

typedef struct vec4 {
	union {
		struct {
			float x,y,z,w;
		} xyzw;
		float a1d[4];
	};
} vec4;

struct bvec4 {
	bool x;
	bool y;
	bool z;
	bool w;
};

// sometimes, you just need some integers
typedef struct ivec3 {
	int x, y, z;
} ivec3;

struct ivec2 {
	int x, y;
};

/** Represents a point in 3d space.

Note: this is a struct, not a class, so no member functions. */
typedef struct vec3d {
	union {
		struct {
			float x,y,z;
		} xyz;
		float a1d[3];
	};
} vec3d;

typedef struct vec2d {
	float x, y;
} vec2d;

typedef struct angles {
	float	p, b, h;
} angles_t;

typedef struct matrix {
	union {
		struct {
			vec3d	rvec, uvec, fvec;
		} vec;
		float a2d[3][3];
		float a1d[9];
	};
} matrix;

typedef struct matrix4 {
	union {
		struct {
			vec4 rvec, uvec, fvec, pos;
		} vec;
		float a2d[4][4];
		float a1d[16];
	};
} matrix4;


typedef struct uv_pair {
	float u,v;
} uv_pair;

typedef struct coord2d {
	int x,y;
} coord2d;

#define TRUE	1
#define FALSE	0


#define PI				3.141592654f
// twice values
const float PI2			= (PI*2.0f);
// half values
const float PI_2		= (PI/2.0f);

/** Represents a point in 3d screen space. 'w' is 1/z.

Like vec3d but for screens.

Note: this is a struct, not a class, so no member functions. */
typedef struct screen3d
{
	union
	{
		struct
		{
			float x, y, w;
		} xyw;
		float a1d[3];
	};
} screen3d;

/** Compares two screen3ds */
inline bool operator==(const screen3d& self, const screen3d& other)
{
	return (self.xyw.x == other.xyw.x && self.xyw.y == other.xyw.y && self.xyw.w == other.xyw.w);
}

/** Used to store rotated points for mines. Has flag to indicate if projected.

Note: this is a struct, not a class, so no memeber functions. */
typedef struct vertex
{
	vec3d world; // world space position
	screen3d screen; // screen space position (sw == 1/z)
	uv_pair texture_position; // texture position
	ubyte r, g, b, a; // color.  Use b for darkening;
	ubyte codes; // what sides of view pyramid this point is on/off.  0 = Inside view pyramid.
	ubyte flags; // Projection flags.  Indicates whether it is projected or not or if projection overflowed.
	ubyte pad[2]; // pad structure to be 4 byte aligned.
} vertex;
