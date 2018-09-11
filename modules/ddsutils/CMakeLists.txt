add_library( ddsutils STATIC
	
	ddsutils/ddsutils.cpp
	ddsutils/ddsutils.h
)
target_include_directories(ddsutils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(ddsutils PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(ddsutils PROPERTIES FOLDER "modules")
