add_library(globalincs STATIC globalincs/version.cpp globalincs/systemvars.cpp)
target_link_libraries( globalincs PUBLIC windows_stub math)
target_include_directories(globalincs PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(globalincs PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(globalincs PROPERTIES FOLDER "modules")
if (MSVC)
target_compile_definitions(globalincs PUBLIC SCP_FORMAT_STRING=_Printf_format_string_)
else()
target_compile_definitions(globalincs PUBLIC SCP_FORMAT_STRING=)
endif()


# IF (WIN32)
# 	add_file_folder("GlobalIncs"
# 		${file_root_globalincs}
# 		globalincs/mspdb_callstack.cpp
# 		globalincs/mspdb_callstack.h
# 		globalincs/windebug.cpp
# 	)
# ENDIF(WIN32)
