#ifndef EXTERNAL_DISPLAY_INTERNALS_H
#define EXTERNAL_DISPLAY_INTERNALS_H

#include "hud_events.h"
#include "external_display/external_display.h"

namespace external_display
{
	namespace internal
	{

		class DisplayProvider
		{
		public:
			virtual ~DisplayProvider()
			{
			}
			virtual void Init(events::HUDEvents* Events)
			{
			}
		};
	}
}

#endif // EXTERNAL_DISPLAY_INTERNALS_H
