
#ifndef _SDL_GRAPHICS_OPERATIONS
#define _SDL_GRAPHICS_OPERATIONS
#pragma once

#include "osapi/osapi.h"
#include "graphics/GraphicsOperation.h"
#include <memory>

namespace SCP
{
	class Viewport;
	struct OpenGLContextAttributes;
}

class SDLGraphicsOperations: public SCP::GraphicsOperations {
 public:
	SDLGraphicsOperations();
	~SDLGraphicsOperations() override;

	std::unique_ptr<SCP::OpenGLContext> createOpenGLContext(SCP::Viewport* viewport,
														   const SCP::OpenGLContextAttributes& gl_attrs) override;

	void makeOpenGLContextCurrent(SCP::Viewport* view, SCP::OpenGLContext* ctx) override;

	std::unique_ptr<SCP::Viewport> createViewport(const SCP::ViewPortProperties& props) override;
};

#endif // _SDL_GRAPHICS_OPERATIONS
