#pragma once

#include <osapi/osapi.h>
#include "mission/Editor.h"
#include "FredView.h"

#define WIN32_LEAN_AND_MEAN
#include <QtGui/QOpenGLContext>

namespace fso {
namespace fred {

class QtOpenGLContext: public SCP::OpenGLContext {
	std::unique_ptr<QOpenGLContext> _context;
 public:
	QtOpenGLContext(std::unique_ptr<QOpenGLContext>&& context);
	~QtOpenGLContext() override;

	os::OpenGLLoadProc getLoaderFunction() override;
	bool setSwapInterval(int status) override;

	void makeCurrent(QSurface* surface);
};

class QtViewport: public SCP::Viewport {
	std::unique_ptr<FredView> _viewportWindow;
	SCP::ViewportProperties _viewProps;
 public:
	QtViewport(std::unique_ptr<FredView>&& window, const SCP::ViewportProperties& viewProps);
	~QtViewport() override;

	SDL_Window* toSDLWindow() override;
	std::pair<uint32_t, uint32_t> getSize() override;
	void swapBuffers() override;
	void setState(SCP::ViewportState state) override;
	void minimize() override;
	void restore() override;

	const SCP::ViewportProperties& getViewProperties() const;
	FredView* getWindow();
};

class QtGraphicsOperations: public os::GraphicsOperations {
	Editor* _editor = nullptr;

	QtOpenGLContext* _lastContext = nullptr;
 public:
	QtGraphicsOperations(Editor* editor);
	~QtGraphicsOperations() override;

	std::unique_ptr<SCP::OpenGLContext>
	createOpenGLContext(SCP::Viewport* viewport, const SCP::OpenGLContextAttributes& gl_attrs) override;

	void makeOpenGLContextCurrent(SCP::Viewport* view, SCP::OpenGLContext* ctx) override;

	std::unique_ptr<SCP::Viewport> createViewport(const SCP::ViewportProperties& props) override;
};

}
}
