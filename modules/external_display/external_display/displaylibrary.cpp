
#include "external_display/displaylibrary.h"


namespace external_display
{
#ifdef ARCH_64BIT
	const char* const LIBRARY_NAME = "ExternalDisplay64.dll";
#else
	const char* const LIBRARY_NAME = "ExternalDisplay.dll";
#endif
	
	

	namespace display
	{
		DisplayLibrary::DisplayLibrary() : HookEvents(nullptr)
		{
			
			if (!LoadExternal(LIBRARY_NAME))
				return;

			HookEvents = LoadFunction<HookEventsSignature>("HookEvents");
		}


		void DisplayLibrary::Init(events::HUDEvents* Events) 
		{
			if (HookEvents)
			{
				HookEvents(Events);
			}
		}

        DisplayLibraryProvider::DisplayLibraryProvider() : library(DisplayLibrary())
		{

		}

		DisplayLibraryProvider::~DisplayLibraryProvider()
		{
		}

		SCP_string DisplayLibraryProvider::getName()
		{
			return "ExternalDisplay";
		}
	}
}
