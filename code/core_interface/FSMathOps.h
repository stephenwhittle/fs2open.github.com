#pragma once

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif


// Caps V between MN and MX.
template <class T> void CAP( T& v, T mn, T mx )
{
	if ( v < mn ) {
		v = mn;
	} else if ( v > mx ) {
		v = mx;
	}
}

// faster version of CAP()
#define CLAMP(x, min, max) do { if ( (x) < (min) ) (x) = (min); else if ((x) > (max)) (x) = (max); } while(false)