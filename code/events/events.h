#pragma once

#include "utils/event.h"

namespace events {

extern util::event<void> EngineUpdate;

extern util::event<void> EngineShutdown;

extern util::event<void, int, int> GameLeaveState;

extern util::event<void, int, int> GameEnterState;

extern util::event<void, const char*> GameMissionLoad;

extern util::event<void, int, int, bool> RenderWingman;

extern util::event<void, float> RenderAfterburnerGauge;

extern util::event<void, int, int> RenderMissionTime;

extern util::event<void, float, float, float> RenderThrottleGauge;

extern util::event<void, bool> RenderAutoTarget;

extern util::event<void, bool> RenderAutoSpeed;
extern util::event<void, int, const char*, const char*, bool> RenderPrimaryWeapon;
extern util::event<void, int, const char*, const char*, bool, bool, int> RenderSecondaryWeapon;
extern util::event<void, const char*, int, bool> RenderEscortShip;

}
