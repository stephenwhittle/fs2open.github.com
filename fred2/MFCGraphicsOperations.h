
#ifndef _MVC_GRAPHICS_OPERATIONS
#define _MVC_GRAPHICS_OPERATIONS
#pragma once

#include "globalincs/pstypes.h"
#include "osapi/osapi.h"

#include <glad/glad_wgl.h>

class MFCViewport : public SCP::Viewport
{
	HWND _windowHandle = nullptr;
	HDC _device_context = nullptr;
public:
	explicit MFCViewport(HWND hwnd, HDC dc);

	~MFCViewport() override;

	SDL_Window* toSDLWindow() override;

	std::pair<uint32_t, uint32_t> getSize() override;

	void swapBuffers() override;

	void setState(SCP::ViewportState state) override;

	void minimize() override;

	void restore() override;

	HDC getHDC();
};

class MFCOpenGLContext : public SCP::OpenGLContext
{
	// HACK: Since OpenGL apparently likes global state we also have to make this global...
	static void* _oglDllHandle;
	static size_t _oglDllReferenceCount;

	HGLRC _render_context = nullptr;
public:
	explicit MFCOpenGLContext(HGLRC hglrc);

	~MFCOpenGLContext() override;

	static void* wglLoader(const char* name);

	os::OpenGLLoadProc getLoaderFunction() override;

	bool setSwapInterval(int status) override;

	HGLRC getHandle();
};

class MFCGraphicsOperations : public os::GraphicsOperations
{
	HWND _windowHandle = nullptr;
public:

	explicit MFCGraphicsOperations(HWND hwnd);

	~MFCGraphicsOperations() override;

	std::unique_ptr<SCP::Viewport> createViewport(const SCP::ViewportProperties& props) override;

	std::unique_ptr<SCP::OpenGLContext> createOpenGLContext(SCP::Viewport* port, const SCP::OpenGLContextAttributes& ctx) override;

	void makeOpenGLContextCurrent(SCP::Viewport* view, SCP::OpenGLContext* ctx) override;
};

#endif // _MVC_GRAPHICS_OPERATIONS
