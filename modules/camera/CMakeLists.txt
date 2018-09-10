add_library( camera STATIC
	camera/camera.cpp
	camera/camera.h
)
target_include_directories(camera PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(camera PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(camera PROPERTIES FOLDER "modules")
