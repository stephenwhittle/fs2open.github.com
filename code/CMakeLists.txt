
INCLUDE(source_groups.cmake)

INCLUDE(voice_rec.cmake)

ADD_LIBRARY(code STATIC ${source_files} ${VOICE_REC_FILES} "core_interface/SCPStrongInteger.h")

target_compile_features(code PUBLIC ${REQUIRED_CXX_FEATURES})

#INCLUDE(default_files.cmake)

if (MSVC)
	target_compile_definitions(code PUBLIC "PDB_DEBUGGING=1")
endif(MSVC)

TARGET_INCLUDE_DIRECTORIES(code PUBLIC ${CODE_HEADERS})
TARGET_INCLUDE_DIRECTORIES(code PUBLIC ${FREESPACE_HEADERS})

TARGET_INCLUDE_DIRECTORIES(code PUBLIC ${GENERATED_SOURCE_DIR})
TARGET_INCLUDE_DIRECTORIES(code PUBLIC ${GENERATED_SOURCE_DIR}/code)

target_link_libraries(code PUBLIC OpenGL)

TARGET_LINK_LIBRARIES(code PUBLIC openal)
TARGET_LINK_LIBRARIES(code PUBLIC ${LUA_LIBS})
TARGET_LINK_LIBRARIES(code PUBLIC ${PNG_LIBS})
TARGET_LINK_LIBRARIES(code PUBLIC ${JPEG_LIBS})

TARGET_LINK_LIBRARIES(code PUBLIC sdl2)

if (FSO_BUILD_WITH_FFMPEG)
	TARGET_LINK_LIBRARIES(code PUBLIC ffmpeg)
endif()

TARGET_LINK_LIBRARIES(code PUBLIC utfcpp)

TARGET_LINK_LIBRARIES(code PUBLIC compiler)

target_link_libraries(code PUBLIC md5)

target_link_libraries(code PUBLIC discord-rpc)

target_link_libraries(code PUBLIC libRocket)

target_link_libraries(code PUBLIC pcp)

add_subdirectory(core_interface)

target_link_libraries(code PUBLIC core)

add_subdirectory(math)

target_link_libraries(code PUBLIC math)

add_subdirectory(filesystem)

target_link_libraries(code PUBLIC filesystem)

add_subdirectory(config)

target_link_libraries(code PUBLIC config)

add_subdirectory(memory)

target_link_libraries(code PUBLIC memory)

add_subdirectory(cfile)

target_link_libraries(code PUBLIC cfile)

add_subdirectory(log)

target_link_libraries(code PUBLIC log)

add_subdirectory(localization)

target_link_libraries(code PUBLIC localization)

add_subdirectory(parse)

target_link_libraries(code PUBLIC parse)

add_subdirectory(utils)

target_link_libraries(code PUBLIC utils)

add_subdirectory(def_files)

target_link_libraries(code PUBLIC def_files)

add_subdirectory(font)

target_link_libraries(code PUBLIC font)

add_subdirectory(color)

add_subdirectory(cmdline)

target_link_libraries(code PUBLIC cmdline)

target_link_libraries(code PUBLIC color)

add_subdirectory(module)

target_link_libraries(code PUBLIC module)

target_link_libraries(code PUBLIC vulkan-hpp)

#add_subdirectory(bmpman)

#target_link_libraries(code PUBLIC bmpman)

add_subdirectory(asteroid)


enable_clang_tidy(code)

IF (FSO_USE_SPEECH)
	find_package(Speech REQUIRED)
	target_link_libraries(code PUBLIC speech)
ENDIF(FSO_USE_SPEECH)

TARGET_LINK_LIBRARIES(code PUBLIC jansson)

# Headers for standalone "gui" on UNIX
IF(UNIX)
	TARGET_LINK_LIBRARIES(code PUBLIC ${MONGOOSE_LIBS})
ENDIF(UNIX)

IF(FSO_BUILD_TOOLS)
	ADD_SUBDIRECTORY(cfilearchiver)

	ADD_SUBDIRECTORY(cfileextractor)
ENDIF(FSO_BUILD_TOOLS)

if (ENABLE_COTIRE)
	set_target_properties(code PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "${CMAKE_CURRENT_SOURCE_DIR}/prefix_header.h")
else()
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		target_compile_options(code PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:-include${CMAKE_CURRENT_SOURCE_DIR}/prefix_header.h>")
	elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		target_compile_options(code PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:/FI ${CMAKE_CURRENT_SOURCE_DIR}/prefix_header.h>")
	elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		target_compile_options(code PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:-include${CMAKE_CURRENT_SOURCE_DIR}/prefix_header.h>")
	elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
		target_compile_options(code PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:-include${CMAKE_CURRENT_SOURCE_DIR}/prefix_header.h>")
	ELSE()
		MESSAGE("Unknown compiler for global includes. This will probably break the build.")
	ENDIF()
endif()


if (FSO_RELEASE_LOGGING)
	target_compile_definitions(code PUBLIC SCP_RELEASE_LOGGING)
endif()

if (FSO_BUILD_WITH_FFMPEG)
	target_compile_definitions(code PUBLIC WITH_FFMPEG)
endif()

include(util)
configure_cotire(code)

# Ensure that Xcode generates debug symbols on macOS for Debug builds
set_target_properties(code PROPERTIES XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Debug] "YES")
set_target_properties(code PROPERTIES XCODE_ATTRIBUTE_COPY_PHASE_STRIP[variant=Debug] "NO")
set_target_properties(code PROPERTIES XCODE_ATTRIBUTE_STRIP_INSTALLED_PRODUCT[variant=Debug] "NO")
set_target_properties(code PROPERTIES XCODE_ATTRIBUTE_GCC_SYMBOLS_PRIVATE_EXTERN[variant=Debug] "NO")


add_subdirectory(code_tests)