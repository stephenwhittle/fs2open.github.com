#pragma once
#include "FSStdTypes.h"
#include "module/SCPModuleBase.h"
#include <memory>

namespace SCP
{
	class Viewport;
	enum class ViewportState;
	class GraphicsModule : public SCPModule<GraphicsModule>
	{
	public:
		virtual bool StartupModule() override {}
		virtual void ShutdownModule() override {}

		static GraphicsModule ConstructModule()
		{
			return GraphicsModule();
		}
		
		Viewport* AddViewport(std::unique_ptr<Viewport>&& viewport) {};

		void SetMainViewport(Viewport* NewMainView) {};
		Viewport* GetMainViewport() {};

		void CloseAllViewports() {};

		ViewportState GetConfiguredViewportState() {};
	private:
		// Gr_configured_window_state
		SCP::ViewportState ViewportState = SCP::ViewportState::Windowed;

	};

} // namespace SCP
