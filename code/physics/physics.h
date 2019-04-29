/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on the 
 * source.
 *
*/



#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "core/pstypes.h"  // for vec3d, matrix, uint
#include <stddef.h>        // for NULL


#define	PF_ACCELERATES			(1 << 1)
#define	PF_USE_VEL				(1 << 2)		//	Use velocity present in physics_info struct, don't call physics_sim_vel.
#define	PF_AFTERBURNER_ON		(1 << 3)		//	Afterburner currently engaged.
#define	PF_SLIDE_ENABLED		(1 << 4)		// Allow descent style sliding
#define	PF_REDUCED_DAMP		(1 << 5)		// Allows reduced damping on z (for death, shockwave) (CAN be reset in physics)
#define	PF_IN_SHOCKWAVE		(1 << 6)		// Indicates whether object has recently been hit by shockwave (used to enable shake)
#define	PF_DEAD_DAMP			(1 << 7)		// Makes forward damping same as sideways (NOT reset in physics)
#define	PF_AFTERBURNER_WAIT	(1 << 8)		// true when afterburner cannot be used.  replaces variable used in afterburner code
#define	PF_CONST_VEL			(1 << 9)		// Use velocity in phys_info struct.  Optimize weapons in phys_sim 
#define	PF_WARP_IN				(1 << 10)	//	Use when ship is warping in
#define	PF_SPECIAL_WARP_IN	(1 << 11)	//	Use when ship is warping in and we want to slow the ship faster than normal game physics
#define	PF_WARP_OUT				(1 << 12)	//	Use when ship is warping out
#define	PF_SPECIAL_WARP_OUT	(1 << 13)	//	Use when ship is warping out and we want to slow the ship faster than normal game physics
#define PF_BOOSTER_ON		(1 << 14)
#define PF_GLIDING			(1 << 15)
#define PF_FORCE_GLIDE		(1 << 16)




extern int physics_paused;				//	Set means don't do physics, except for player.

// To use the "Descent-ship" physics:
//   controls_read_all(&ci, FrameSecs );
//   physics_read_flying_controls( &ViewerOrient, &ViewerPhysics, FrameSecs, &ci );
//   physics_sim(&ViewerPos, &ViewerOrient, &ViewerPhysics, FrameSecs );		
extern void physics_init( physics_info * pi );
extern void physics_read_flying_controls( matrix * orient, physics_info * pi, control_info * ci, float sim_time, vec3d *wash_rot=NULL);
extern void physics_sim(vec3d *position, matrix * orient, physics_info * pi, float sim_time );
extern void physics_sim_editor(vec3d *position, matrix * orient, physics_info * pi, float sim_time);

extern void physics_sim_vel(vec3d * position, physics_info * pi, float sim_time, matrix * orient);
extern void physics_sim_rot(matrix * orient, physics_info * pi, float sim_time );
extern void physics_apply_whack(vec3d *force, vec3d *pos, physics_info *pi, matrix *orient, float mass);
extern void physics_apply_shock(vec3d *direction_vec, float pressure, physics_info *pi, matrix *orient, vec3d *min, vec3d *max, float radius);
extern void physics_collide_whack(vec3d *impulse, vec3d *delta_rotvel, physics_info *pi, matrix *orient, bool is_landing);
int check_rotvel_limit( physics_info *pi );


// If physics_set_viewer is called with the viewer's physics_info, then
// this variable tracks the viewer's bank.  This is used for g3_draw_rotated_bitmap.
extern float Physics_viewer_bank;

// If you would like Physics_viewer_bank to be tracked (Which is needed
// for rotating 3d bitmaps) call this and pass it a pointer to the
// viewer's physics info.
#define PHYSICS_VIEWER_FRONT				0
#define PHYSICS_VIEWER_LEFT				1
#define PHYSICS_VIEWER_RIGHT				2
#define PHYSICS_VIEWER_REAR				3
#define PHYSICS_VIEWER_UP					4
void physics_set_viewer( physics_info * p, int dir );

//WMC - apply_physics
void apply_physics( float damping, float desired_vel, float initial_vel, float t, float * new_vel, float * delta_pos );

//WMC - camera code type stuff
//Acceleration, constant Velocity, Deceleration (to another speed, so maybe not deceleration)
class avd_movement
{
private:
	//Current
	float Pc;		//Current position
	float Vc;		//Current velocity
	
	//Initial
	int TSi;		//Initial timestamp <-- note TIMESTAMP
	float Pi;		//Initial position
	float Vi;		//Initial velocity

	//Given
	float Pf;		//Final position
	float Tf;		//Final duration
	float Tai;		//Starting acceleration duration
	float Taf;		//Ending acceleration duration
	float Vf;		//Final velocity

	//Calculated
	float Vm;		//Middle velocity
	float Ai;		//Starting acceleration
	float Af;		//Ending acceleration
public:
	avd_movement();
	void clear();

	void get(float Time, float *Position, float *Velocity);
	void get(float *Position, float *Velocity);

	void set(float position);
	void setAVD(float final_position, float total_movement_time, float starting_accleration_time, float ending_acceleration_time, float final_velocity);
	void setVD(float total_movement_time, float ending_acceleration_time, float final_velocity);

	void update(float Time);
	void update();
};

/*
#ifdef __cplusplus
}
#endif
*/

#endif	// PHYSICS_H
