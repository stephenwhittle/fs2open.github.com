#include <render/SplineRenderer.h>
#include <graphics/2d.h>
#include <render/3d.h>
#include <math/spline.h>
#include <globalincs/alphacolors.h>
    // render a bezier
void SplineRenderer::bez_render(bez_spline spline, int divs, color* c)
{
	float inc;
	int idx;
	vertex a, b;
	vec3d pt;

	// bleh
	if(divs <= 0){
		return;
	}
	inc = 1.0f / (float)divs;

	// draw in red
	gr_set_color_fast(c);

	// draw that many divisions
	spline.bez_get_point(&pt, 0.0f);
	g3_rotate_vertex(&a, &pt);
	for(idx=1; idx<=divs; idx++){
		// second point
		spline.bez_get_point(&pt, (float)idx * inc);
		g3_rotate_vertex(&b, &pt);

		// draw the line
		g3_draw_line(&a, &b);

		// store b
		a = b;
	}

	// draw the control points
	gr_set_color_fast(&Color_bright_green);
	for(idx=0; idx<spline.num_pts; idx++){
		g3_draw_sphere_ez(&(spline.pts[idx]), 0.75f);
	}
}


// render a bezier
void SplineRenderer::herm_render(herm_spline spline, int divs, color *clc)
{
	int idx;
	int s_idx;
	float inc = 1.0f / (float)divs;

	vertex a, b, c;
	vec3d pt, d_pt;

	// draw in red
	gr_set_color_fast(clc);

	// render each section
	for(idx=0; idx<spline.num_pts-1; idx++){
		// render this piece
		spline.herm_get_point(&pt, 0.0f, idx);		
		g3_rotate_vertex(&a, &pt);
		
		// draw the deriv
		spline.herm_get_deriv(&d_pt, 0.0f, idx);
		vm_vec_add2(&d_pt, &pt);
		g3_rotate_vertex(&c, &d_pt);
		g3_draw_line(&a, &c);

		for(s_idx=1; s_idx<divs * 2; s_idx++){
			// second point
			spline.herm_get_point(&pt, (float)s_idx * inc, idx);			
			
			// 2nd point on the line
			g3_rotate_vertex(&b, &pt);

			// draw the line
			g3_draw_line(&a, &b);

			// draw the deriv line
			spline.herm_get_deriv(&d_pt, (float)s_idx * inc, idx);			
			vm_vec_add2(&d_pt, &pt);
			g3_rotate_vertex(&c, &d_pt);
			g3_draw_line(&b, &c);

			// store b
			a = b;
		}		
	}	

	// draw the control points
	gr_set_color_fast(&Color_bright_green);
	for(idx=0; idx<spline.num_pts; idx++){
		g3_draw_sphere_ez(&(spline.pts[idx]), 0.75f);
	}
}
