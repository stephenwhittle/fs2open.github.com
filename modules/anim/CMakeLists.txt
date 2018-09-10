add_library( anim STATIC
	anim/animplay.cpp
	anim/animplay.h
	anim/packunpack.cpp
	anim/packunpack.h
)
target_include_directories(anim PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(anim PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(anim PROPERTIES FOLDER "modules")
