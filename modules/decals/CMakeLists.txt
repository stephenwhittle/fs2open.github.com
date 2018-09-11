add_library( decals STATIC
	
	decals/decals.cpp
	decals/decals.h
)
target_include_directories(decals PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(decals PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(decals PROPERTIES FOLDER "modules")
