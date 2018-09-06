
#include "events.h"

namespace events {

util::event<void> EngineUpdate;

util::event<void> EngineShutdown;

util::event<void, int, int> GameLeaveState;

util::event<void, int, int> GameEnterState;

util::event<void, const char*> GameMissionLoad;



//Called each time a wingman is rendered to the HUD
//WingIndex, WingMemberIndex, IsBright
util::event<void, int, int, bool> RenderWingman;
//AfterburnerPercentage
util::event<void, float> RenderAfterburnerGauge;
//Minutes,Seconds
util::event<void, int, int> RenderMissionTime;
//CurrentSpeed, DesiredSpeed, MaxSpeed;
util::event<void, float, float, float> RenderThrottleGauge;

util::event<void, vec3d, float> RenderRadarBlip;

util::event<void, bool> RenderAutoTarget;
util::event<void, bool> RenderAutoSpeed;
//index, name, ammo, linked
util::event<void, int, const char*, const char*, bool> RenderPrimaryWeapon;
//index, name, ammo, armed, linked, cooldown
util::event<void, int, const char*, const char*,bool, bool, int> RenderSecondaryWeapon;
//#define HUD_TARGET_MONITOR 8
//#define HUD_TARGET_MONITOR_EXTRA_DATA 10
//#define HUD_TARGET_SHIELD_ICON 11
//#define HUD_PLAYER_SHIELD_ICON 12
//#define HUD_ETS_GAUGE 13
util::event<void, const char*, int, bool> RenderEscortShip;
#define HUD_ESCORT_VIEW 17
#define HUD_DIRECTIVES_VIEW 18
#define HUD_THREAT_GAUGE 19
#define HUD_AFTERBURNER_ENERGY 20
#define HUD_WEAPONS_ENERGY 21
#define HUD_WEAPON_LINKING_GAUGE 22
#define HUD_TARGET_MINI_ICON 23
#define HUD_TALKING_HEAD 25
#define HUD_DAMAGE_GAUGE 26
#define HUD_MESSAGE_LINES 27
#define HUD_MISSILE_WARNING_ARROW 28
#define HUD_CMEASURE_GAUGE 29
#define HUD_OBJECTIVES_NOTIFY_GAUGE 30
#define HUD_WINGMEN_STATUS 31
#define HUD_OFFSCREEN_RANGE 32
#define HUD_KILLS_GAUGE 33
#define HUD_ATTACKING_TARGET_COUNT 34
#define HUD_TEXT_FLASH 35 // (formerly split up among emp, collision, etc)
#define HUD_MESSAGE_BOX 36
#define HUD_SUPPORT_GAUGE 37
#define HUD_LAG_GAUGE 38
}
