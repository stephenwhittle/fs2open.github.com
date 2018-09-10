add_library( fireball STATIC
	fireball/fireballs.cpp
	fireball/fireballs.h
	fireball/warpineffect.cpp
)
target_include_directories(fireball PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(fireball PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(fireball PROPERTIES FOLDER "modules")
