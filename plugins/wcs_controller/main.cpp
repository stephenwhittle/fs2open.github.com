#include "hud_events.h"
void ShowThrottle(float CurrentSpeed, float DesiredSpeed, float MaxSpeed)
{
	mprintf(("Current %f Desired %f Max %f", CurrentSpeed, DesiredSpeed, MaxSpeed));
}


bool HookEvents(events::HUDEvents* Events)
{
	Events->RenderThrottleGauge.add(ShowThrottle);
    return true;
}
//mprintf(( "Error opening %s\n", filename ));

