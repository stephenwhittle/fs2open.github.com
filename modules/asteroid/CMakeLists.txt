add_library( asteroid STATIC
	asteroid/asteroid.cpp
	asteroid/asteroid.h
)
target_include_directories(asteroid PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(asteroid PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(asteroid PROPERTIES FOLDER "modules")
