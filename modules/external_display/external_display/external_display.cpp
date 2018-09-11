#include "external_display/displaylibrary.h"
#include "external_display/external_display.h"
#include "external_display/external_display_internal.h"
#include <memory>

namespace external_display
{
	std::unique_ptr<internal::DisplayProvider> currentProvider = nullptr;
	
	template<class Provider>
	std::unique_ptr<internal::DisplayProvider> initProvider()
	{
		std::unique_ptr<internal::DisplayProvider> provider;
		provider.reset(new Provider());
		mprintf(("    Successfully initialized '%s'\n", Provider::getName().c_str() ));
		return provider;
	}

	bool init()
	{
		mprintf(("Initializing external display...\n"));

		auto Provider = initProvider<display::DisplayLibraryProvider>();
		if (Provider)
		{
			Provider->Init(&events::ExternalDisplay);
			currentProvider = std::move(Provider);
			return true;
		}

		mprintf(("No supported provider found, external display will not be used...\n"));
		return false;
	}


	void shutdown()
	{
		if (currentProvider == nullptr)
		{
			return;
		}

		currentProvider.reset();
	}
}
