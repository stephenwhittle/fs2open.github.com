/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on the 
 * source.
 *
*/

/** @defgroup osapi The OS API
 *  Provides a number of function to interact with the operating system. The API is the same for every platform.
 */

/** @file
 *  @ingroup osapi
 */


#ifndef _OSAPI_H
#define _OSAPI_H

#include "globalincs/pstypes.h"
#include "config/osregistry.h"
#include "osapi/dialogs.h"
#include "globalincs/flagset.h"

#include <functional>
#include <memory>

#include <SDL_events.h>

// --------------------------------------------------------------------------------------------------
// OSAPI DEFINES/VARS
//

// set if running under MsDev - done after os_init(...) has returned
extern int Os_debugger_running;

#ifdef SCP_UNIX
const char* os_get_legacy_user_dir();
#endif

// --------------------------------------------------------------------------------------------------
// OSAPI FUNCTIONS
//

// initialization/shutdown functions -----------------------------------------------

// If app_name is NULL or ommited, then TITLE is used
// for the app name, which is where registry keys are stored.
void os_init(const char * wclass, const char * title, const char * app_name = nullptr);

// set the main window title
void os_set_title( const char * title );

// call at program end
void os_cleanup();

// window management ---------------------------------------------------------------

// Returns 1 if app is not the foreground app.
int os_foreground();

// process management --------------------------------------------------------------

/**
 * @brief Removes all pending events and ignores them
 */
void os_ignore_events();

// call to process windows messages. only does something in non THREADED mode
void os_poll();

// Sleeps for n milliseconds or until app becomes active.
void os_sleep(uint ms);

/**
 * @brief Determines if FSO is running in legacy config mode
 */
bool os_is_legacy_mode();

/**
 * @brief Gets a path to a configuration file
 * A relative path to the wanted file or directory. Directories should be separated with '/'
 * @param subpath The path of the wanted file or directory, may be emtpy to get the config directory
 * 
 * @returns The path to the specified config path
 */
SCP_string os_get_config_path(const SCP_string& subpath = "");

namespace os
{
	/**
	 * @defgroup os_graphics_api Graphics operations abstraction
	 * @see @ref graphics_api_page
	 * @ingroup osapi
	 */

	

	

	

	

	
	
	

	

	/**
	 * @brief Adds a viewport to the osapi list
	 *
	 * The osapi system will automatically free these viewports when it is shut down.
	 *
	 * @param viewport The viewport to add. The osapi system assumes ownership over the pointer
	 * @return The pointer of the viewport
	 *
	 * @ingroup os_graphics_api
	 */
	//Viewport* addViewport(std::unique_ptr<Viewport>&& viewport);

	/**
	 * @brief Sets the main viewport of the application
	 * @param mainView The viewport to set
	 *
	 * @ingroup os_graphics_api
	 */
	//void setMainViewPort(Viewport* mainView);

	/**
	 * @brief Gets the main viewport of the application
	 * @return The main viewport, can be @c nullptr if it hasn't been set yet
	 *
	 * @ingroup os_graphics_api
	 */
	//Viewport* getMainViewport();

	/**
	 * @brief Gets the SDL handle of the main window
	 * @return The SDL handle or @c nullptr if there is no main window or if the main window can't be represented as
	 * an SDL window
	 *
	 * @ingroup os_graphics_api
	 */
	SDL_Window* getSDLMainWindow();

	void closeAllViewports();

	/**
	 * @defgroup eventhandling API for consuming OS events
	 * @ingroup osapi
	 * See \ref eventhandling_page for more information.
	 *  @{
	 */
	namespace events
	{
		/**
		 * @brief The default weight of a listener.
		 * The default event handler use this weight so other event handlers should have a weight that is less than this.
		 */
		const int DEFAULT_LISTENER_WEIGHT = 0;

		/**
		 * @brief An event handler
		 * Gets the generated sdl event and must return if it handled the event or not.
		 */
		typedef std::function<bool(const SDL_Event&)> Listener;

		/**
		 * @brief An identification for a listener
		 */
		typedef size_t ListenerIdentifier;

		/**
		 * @brief Adds a new event handler
		 * @param type The type of events to handle
		 * @param weigth The weight of this handler, used for determining in which order handlers are called.
		 * @param listener The listener that will be called
		 * @return An idfentification which can be used to remove this handler again
		 * @see removeEventListener()
		 */
		ListenerIdentifier addEventListener(SDL_EventType type, int weigth, const Listener& listener);

		/**
		 * @brief Removes the event handler with the given identifier
		 * @param identifier The identifier of the event handler that should be removed
		 * @return @c true if the event handler was removed, @c false otherwise
		 */
		bool removeEventListener(ListenerIdentifier identifier);

		/**
		* @brief Checks if the event belongs to a window
		* This can be used to handle multiple windows correctly and only handle the events
		* that belong to a specific window.
		*
		* @param e The event that should be checked
		* @param window The window the event should belong to
		* @return @c true if the event belongs to the window, @c false otherwise
		*/
		bool isWindowEvent(const SDL_Event& e, SDL_Window* window);
	}
	/** @} */ // end of OsAPI
}

// Documentation pages
/**
 * @page osapi_page The OS API
 * @subpage eventhandling_page
 * @subpage graphics_api_page
 */

/**
 * @page eventhandling_page OS Event handling API
 * The OS-API exposed a generic event handler interface to allow user code to consume SDL events. The handler will
 * be passed the SDL_Event structure and should return @c true if it handled the event or @c false if not. If it handled the event then the processing of this event will be stopped and it will not be delivered to subsequent
 * listeners.
 */

/**
 * @page graphics_api_page OS Graphics API
 * Since multiple different applications need to use the FSO rendering engine it's necessary to provide an abstraction
 * of how an OpenGL context is created. Furthermore, since FSO may want to use multiple viewports it's also necessary to
 * provide an abstraction for that. This is done via the @ref GraphicsOperations class which must be implemented by users
 * of the FSO rendering engine. This class allows to create viewports and OpenGL contexts. First, a viewport will be
 * created which can be a standalone window or a part of a GUI. Then an OpenGL context will be created based on that
 * viewport which will be used for further rendering.
 *
 * @see @ref os_graphics_api
 */

#endif // _OSAPI_H
