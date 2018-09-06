#ifndef EXTERNAL_DISPLAY_INTERNALS_H
#define EXTERNAL_DISPLAY_INTERNALS_H

#include "globalincs/pstypes.h"

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
		};
	}
}

#endif // EXTERNAL_DISPLAY_INTERNALS_H
