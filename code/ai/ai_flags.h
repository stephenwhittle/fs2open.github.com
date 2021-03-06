#ifndef AI_FLAGS_H
#define AI_FLAGS_H

#include "globalincs/flagset.h"

namespace AI {
	FLAG_LIST(AI_Flags) {
		Formation_wing,				//	Fly in formation as part of wing.
		Awaiting_repair,			//	Awaiting a repair ship.
		Being_repaired,				//	Currently docked with repair ship.
		Repairing,					//	Repairing a ship (or going to repair a ship)
		Seek_lock,					//	set if should focus on gaining aspect lock, not hitting with lasers
		Formation_object,			//	Fly in formation off a specific object.
		Temporary_ignore,			//	Means current ignore_objnum is only temporary, not an order from the player.
		Use_exit_path,				//  Used by path code, to flag path as an exit path
		Use_static_path,			//  Used by path code, use fixed path, don't try to recreate
		Target_collision,			//	Collided with aip->target_objnum last frame.  Avoid that ship for half a second or so.
		Unload_secondaries,			//	Fire secondaries as fast as possible!
		On_subsys_path,				//  Current path leads to a subsystem
		Avoid_shockwave_ship,		//	Avoid an existing shockwave from a ship.
		Avoid_shockwave_weapon,		//	Avoid an expected shockwave from a weapon.  shockwave_object field contains object index.
		Avoid_shockwave_started,	//	Already started avoiding shockwave, don't keep deciding whether to avoid.
		Attack_slowly,				//	Move slowly while attacking.
		Repair_obstructed,			//	Ship wants to be repaired, but path is obstructed.
		Kamikaze,					//	Crash into target
		No_dynamic,					//	Not allowed to get dynamic goals
		Avoiding_small_ship,		//	Avoiding a player ship.
		Avoiding_big_ship,			//	Avoiding a large ship.
		Big_ship_collide_recover_1,	//	Collided into a big ship.  Recovering by flying away.
		Big_ship_collide_recover_2,	//	Collided into a big ship.  Fly towards big ship sphere perimeter.
		Stealth_pursuit,			//  AI is trying to fight stealth ship
		Unload_primaries,			//	Fire primaries as fast as possible!
		Trying_unsuccessfully_to_warp,	// Trying to warp, but can't warp at the moment
		Free_afterburner_use,		// Use afterburners while following waypoints or flying towards objects

		NUM_VALUES
	};

	FLAG_LIST(Goal_Flags) {
		Docker_index_valid,	// when set, index field for docker is valid
		Dockee_index_valid,	// when set, index field for dockee is valid
		Goal_on_hold,		// when set, this goal cannot currently be satisfied, although it could be in the future
		Subsys_needs_fixup,	// when set, the subsystem index (for a destroy subsystem goal) is invalid and must be gotten from the subsys name stored in docker.name field!!
		Goal_override,		// paired with AIG_TYPE_DYNAMIC to mean this goal overrides any other goal
		Purge,				// purge this goal next time we process
		Goals_purged,		// this goal has already caused other goals to get purged
		Depart_sound_played,// Goober5000 - replacement for AL's hack ;)
		Target_own_team,

		NUM_VALUES
	};

	FLAG_LIST(Maneuver_Override_Flags) {
		Full,		//	Full sexp control
		Roll,		//	Sexp forced roll maneuver
		Pitch,		//	Sexp forced pitch change
		Heading,	//	Sexp forced heading change
		Full_lat,	//  full control over up/side/forward movement
		Up,			//	vertical movement
		Sideways,	//	horizontal movement
		Forward,	//	forward movement

		NUM_VALUES
	};

	FLAG_LIST(Profile_Flags) {
        Advanced_turret_fov_edge_checks,
        Ai_aims_from_ship_center,
        Ai_can_slow_down_attacking_big_ships,
        Ai_guards_specific_ship_in_wing,
        All_ships_manage_shields,
        Allow_multi_event_scoring,
        Allow_primary_link_at_start,
        Allow_rapid_secondary_dumbfire,
        Allow_turrets_target_weapons_freely,
        Allow_vertical_dodge,
        Aspect_invulnerability_fix,
        Aspect_lock_countermeasure,
        Assist_scoring_scales_with_damage,
        Beams_damage_weapons,
        Big_ships_can_attack_beam_turrets_on_untargeted_ships,
        Disable_linked_fire_penalty,
        Disable_weapon_damage_scaling,
        Dont_insert_random_turret_fire_delay,
        Fix_ai_class_bug,
        Fix_ai_path_order_bug,
        Fix_heat_seeker_stealth_bug,
        Fix_linked_primary_bug,
        Force_beam_turret_fov,
        Glide_decay_requires_thrust,
        Hack_improve_non_homing_swarm_turret_fire_accuracy,
        Huge_turret_weapons_ignore_bombs,
        Include_beams_in_stat_calcs,
        Kill_scoring_scales_with_damage,
        Multi_allow_empty_primaries,
        Multi_allow_empty_secondaries,
        Navigation_subsys_governs_warp,
        No_min_dock_speed_cap,
        No_special_player_avoid,
        No_warp_camera,
        Perform_fewer_scream_checks,
        Player_weapon_scale_fix,
        Prevent_targeting_bombs_beyond_range,
        Require_turret_to_have_target_in_fov,
        Shockwaves_damage_small_ship_subsystems,
        Smart_afterburner_management,
        Smart_primary_weapon_selection,
        Smart_secondary_weapon_selection,
        Smart_shield_management,
        Smart_subsystem_targeting_for_turrets,
        Strict_turred_tagged_only_targeting,
        Turrets_ignore_target_radius,
        Use_actual_primary_range,
        Use_additive_weapon_velocity,
        Use_newtonian_dampening,
        Use_only_single_fov_for_turrets,

		NUM_VALUES
	};
}

#endif