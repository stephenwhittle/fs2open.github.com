#include <math/spline.h>

class SplineRenderer {
	// render a bezier
	void herm_render(herm_spline spline, int divs, color* c);

	// render a bezier
	void bez_render(bez_spline spline, int divs, color* c);
};