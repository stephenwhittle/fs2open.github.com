#pragma once
#include <memory>

namespace SCP
{
	class Viewport;
	struct ViewPortProperties;
	class OpenGLContext;
	struct OpenGLContextAttributes;

	/**
		* @brief Abstraction for various graphics operations
		*
		* This is used for providing platform specific functionality for various graphics operations.
		*
		* @ingroup os_graphics_api
		*/
	class GraphicsOperations
	{
	public:
		virtual ~GraphicsOperations() {}

		/**
			* @brief Creates an OpenGL contex
			*
			* Uses the specified attributes and creates an OpenGL context. The width and height
			* values may be used for creating the main window.
			*
			* @warning The viewport must be configured to support OpenGL!
			*
			* @param viewport The viewport to create the context for.
			*
			* @return A pointer to the OpenGL context or @c nullptr if the creation has failed
			*/
		virtual std::unique_ptr<SCP::OpenGLContext> createOpenGLContext(Viewport* viewport,
																	const SCP::OpenGLContextAttributes& gl_attrs) = 0;

		/**
			* @brief Makes an OpenGL context current
			*
			* @warning The viewport must be configured to support OpenGL!
			*
			* @param view The viewport to make the context current on
			* @param ctx The OpenGL context to make current, may be @c nullptr
			*/
		virtual void makeOpenGLContextCurrent(SCP::Viewport* view,SCP::OpenGLContext* ctx) = 0;

		/**
			* @brief Creates a new viewport
			*
			* @note Implementations may choose to dissallow viewport creation after a certain number of viewports are
			* created. E.g. FRED may not want to create more than one viewport.
			*
			* @param props The desired properties of the new viewport.
			* @return The created viewport, may be @c nullptr if the viewport can't be created
			*/
		virtual std::unique_ptr<Viewport> createViewport(const ViewPortProperties& props) = 0;
	};

}