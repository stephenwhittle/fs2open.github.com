
include(prebuilt.cmake)

ADD_SUBDIRECTORY(libjpeg)

ADD_SUBDIRECTORY(zlib)

ADD_SUBDIRECTORY(libpng)

ADD_SUBDIRECTORY(lua)

include(OpenAL.cmake)

add_subdirectory(opengl)

ADD_SUBDIRECTORY(libsdl)

ADD_SUBDIRECTORY(jansson)
# These are only required for the web-ui which is non-Windows
IF(UNIX)
	ADD_SUBDIRECTORY(mongoose)
ENDIF(UNIX)

add_subdirectory(md5)

add_subdirectory(utfcpp)

include(freetype.cmake) 

if (FSO_BUILD_WITH_FFMPEG)
	include(FFmpeg.cmake)
endif()

add_subdirectory(discord)

include(libRocket.cmake)

add_subdirectory(libpcp)

add_subdirectory(fmt)

add_subdirectory(filesystem)

add_subdirectory(optional)

add_subdirectory(variant)

add_subdirectory(mio)


#set(SQLITECPP_INTERNAL_SQLITE OFF CACHE STRING "" FORCE)

#include(sqlite3.cmake)

add_subdirectory(SQLiteCpp)

include(peglib.cmake)

add_subdirectory(vulkan-hpp)
