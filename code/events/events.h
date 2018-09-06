#pragma once

#include "utils/event.h"

namespace events {

extern util::event<void> EngineUpdate;

extern util::event<void> EngineShutdown;

extern util::event<void, int, int> GameLeaveState;

extern util::event<void, int, int> GameEnterState;

extern util::event<void, const char*> GameMissionLoad;

class HUDEvents
{
public:
	// Called each time a wingman is rendered to the HUD
	// WingIndex, WingMemberIndex, IsBright
	util::event<void, int, int, bool> RenderWingman;
	// AfterburnerPercentage
	util::event<void, float> RenderAfterburnerGauge;
	// Minutes,Seconds
	util::event<void, int, int> RenderMissionTime;
	// CurrentSpeed, DesiredSpeed, MaxSpeed;
	util::event<void, float, float, float> RenderThrottleGauge;

	util::event<void, vec3d*, float> RenderRadarBlip;

	util::event<void, bool> RenderAutoTarget;
	util::event<void, bool> RenderAutoSpeed;
	// index, name, ammo, max ammo, linked
	util::event<void, int, const char*, int, int, bool> RenderPrimaryWeapon;
	// index, name, ammo,max ammo,  armed, linked, cooldown
	util::event<void, int, const char*, int, int, bool, bool, int> RenderSecondaryWeapon;
	//#define HUD_TARGET_MONITOR 8
	//#define HUD_TARGET_MONITOR_EXTRA_DATA 10
	//#define HUD_TARGET_SHIELD_ICON 11
	//#define HUD_PLAYER_SHIELD_ICON 12
	//#define HUD_ETS_GAUGE 13
	util::event<void, const char*, int, bool> RenderEscortShip;
	util::event<void, int, const char*, int> RenderObjective;

	util::event<void, float> RenderWeaponEnergyGauge;
	util::event<void, const char*, int> RenderDamageLevel;
	util::event<void, int> RenderCountermeasureGauge;
	util::event<void, int> RenderKillsGauge;
	util::event<void, int> RenderLockWarning;
	/*

	#define HUD_MESSAGE_LINES 27
	#define HUD_MISSILE_WARNING_ARROW 28
	#define HUD_OBJECTIVES_NOTIFY_GAUGE 30
	#define HUD_OFFSCREEN_RANGE 32
	#define HUD_ATTACKING_TARGET_COUNT 34
	#define HUD_TEXT_FLASH 35 // (formerly split up among emp, collision, etc)
	#define HUD_MESSAGE_BOX 36
	#define HUD_SUPPORT_GAUGE 37
	#define HUD_LAG_GAUGE 38
	*/

};

extern HUDEvents ExternalDisplay;
}
