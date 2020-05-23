
#pragma once

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

