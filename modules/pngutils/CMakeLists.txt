add_library( pngutils STATIC
	
	pngutils/pngutils.cpp
	pngutils/pngutils.h
)
target_include_directories(pngutils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(pngutils PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(pngutils PROPERTIES FOLDER "modules")
