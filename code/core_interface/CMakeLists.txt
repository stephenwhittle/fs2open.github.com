add_library(core INTERFACE)
target_include_directories(core INTERFACE ${CMAKE_CURRENT_LIST_DIR} ${GENERATED_SOURCE_DIR})
target_link_libraries(core INTERFACE fmt platform ghc_filesystem OptickCore)

target_add_platform_files(core INTERFACE SCPGetPrefsPath.h)

option(FS2_ENABLE_PROFILING "Enable the use of Optick profiling" OFF)

if (FS2_ENABLE_PROFILING)
	target_compile_definitions(core INTERFACE -DFS2_ENABLE_PROFILING)
	set(OPTICK_ENABLED ON CACHE STRING "" FORCE)
	else()
	set(OPTICK_ENABLED OFF CACHE STRING "" FORCE)
endif()

option(FS2_GENERATE_XML_DOC "Generate XML documentation" OFF)

if (FS2_GENERATE_XML_DOC)
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		target_compile_options(core INTERFACE "/doc")
	endif()
endif()