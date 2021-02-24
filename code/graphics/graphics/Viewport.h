#pragma once
#include "graphics/OpenGLContext.h"

namespace SCP
{
	enum class ViewportState
	{
		Windowed = 0,
		Borderless = 1,
		Fullscreen = 2
	};
	/**
	 * @brief Flags for viewport creation
	 * @ingroup os_graphics_api
	 */
	enum class ViewPortFlags
	{
		Fullscreen = 0,
		Borderless,
		Resizeable,

		NUM_VALUES
	};

	/**
	 * @brief Pixel format of a viewport
	 * @ingroup os_graphics_api
	 */
	struct ViewportPixelFormat
	{
		uint32_t red_size;
		uint32_t green_size;
		uint32_t blue_size;
		uint32_t alpha_size;

		uint32_t depth_size;
		uint32_t stencil_size;

		uint32_t multi_samples; //!< The amount of multi-sampling, use 0 for no multi-sampling
	};


	/**
	 * @brief A viewport supporting graphics operations
	 *
	 * A viewport is something that supports rendering operations. Typically this is a window but here it's more
	 * abstract.
	 *
	 * @ingroup os_graphics_api
	 */
	class Viewport
	{
	public:
		virtual ~Viewport() {}

		/**
		 * @brief Returns a SDL_Window handle for this viewport
		 *
		 * @note The returned handle is owned by the viewport and may not be destroyed by the caller.
		 *
		 * @return The window handle or @c nullptr if the viewport can't be represented as an SDL_Window
		 */
		virtual SDL_Window* toSDLWindow() = 0;

		/**
		 * @brief Gets the size of this viewport
		 *
		 * @note This is the actual window size. On HiDPI systems the size of the renderable area might be bigger if
		 * the window is created with support for that.
		 *
		 * @return A (width, height) pair
		 */
		virtual std::pair<uint32_t, uint32_t> getSize() = 0;

		/**
		 * @brief Swaps the buffers of this window
		 */
		virtual void swapBuffers() = 0;

		/**
		 * @brief Sets the window state of the viewport
		 *
		 * @note Implementation may ignore invocations of this function
		 *
		 * @param state The desired state
		 */
		virtual void setState(ViewportState state) = 0;

		/**
		 * @brief Minimizes the viewport
		 *
		 * @note Implementation may ignore invocations of this function
		 */
		virtual void minimize() = 0;

		/**
		 * @brief Restores/Maximizes the viewport
		 *
		 * @note Implementation may ignore invocations of this function
		 */
		virtual void restore() = 0;
	};

	/**
	 * @brief Properties of a viewport that should be created
	 * @ingroup os_graphics_api
	 */
	struct ViewPortProperties
	{
		bool enable_opengl = false; //!< Set to true if the viewport should support OpenGL rendering
		OpenGLContextAttributes gl_attributes;

		bool enable_vulkan = false; //!< Set to true if the viewport should support Vulkan rendering

		ViewportPixelFormat pixel_format;

		SCP_string title;

		uint32_t width;
		uint32_t height;

		flagset<ViewPortFlags> flags;

		uint32_t display;
	};

} // namespace SCP