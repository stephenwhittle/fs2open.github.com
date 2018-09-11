
#ifndef DISPLAY_LIBRARY_H
#define DISPLAY_LIBRARY_H

#include "external_display/external_display.h"
#include "external_display/external_display_internal.h"
#include "hud_events.h"
#include "external_dll/externalcode.h"

namespace external_display
{
	namespace display
	{


		typedef bool (*HookEventsSignature)(events::HUDEvents* data);
		
		class DisplayLibrary : public SCP_ExternalCode
		{
		private:
			HookEventsSignature HookEvents;

		public:
			DisplayLibrary();

			virtual ~DisplayLibrary() {}
			void Init(events::HUDEvents* Events);
		};

		class DisplayLibraryProvider : public internal::DisplayProvider
		{
		private:
			DisplayLibrary library;

		public:
			DisplayLibraryProvider();
			virtual void Init(events::HUDEvents* Events) override 
			{
				library.Init(Events);
			};

			virtual ~DisplayLibraryProvider();

			static SCP_string getName();
		};
	}
}

#endif // DISPLAY_LIBRARY_H
