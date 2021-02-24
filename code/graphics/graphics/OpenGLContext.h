#pragma once
namespace SCP
{
	/**
	 * @brief A function pointer for loading an OpenGL function
	 * @ingroup os_graphics_api
	 */
	using OpenGLLoadProc = void* (*) (const char* name);

	/**
	 * @brief Flags for OpenGL context creation
	 * @ingroup os_graphics_api
	 */
	enum class OpenGLContextFlags
	{
		Debug = 0,
		ForwardCompatible,

		NUM_VALUES
	};

	/**
	 * @brief The required context profile
	 * @ingroup os_graphics_api
	 */
	enum class OpenGLProfile
	{
		Core,
		Compatibility
	};

	/**
	 * @brief Attributes for OpenGL context creation
	 * @ingroup os_graphics_api
	 */
	struct OpenGLContextAttributes
	{
		uint32_t major_version; //!< The major version of the created context
		uint32_t minor_version; //!< The minor version of the created context

		flagset<OpenGLContextFlags> flags; //!< The OpenGL context flags
		OpenGLProfile profile; //!< The desired OpenGL profile
	};

	/**
	 * @brief An OpenGL context
	 * Can be deleted which will properly free the resources of the underlying OpenGL context
	 * @ingroup os_graphics_api
	 */
	class OpenGLContext
	{
	public:
		virtual ~OpenGLContext() {}

		/**
		 * @brief Gets an OpenGL loader function
		 */
		virtual OpenGLLoadProc getLoaderFunction() = 0;

		/**
		 * @brief Sets the swap interval
		 */
		virtual bool setSwapInterval(int status) = 0;
	};

} // namespace SCP