add_library( physics STATIC
	
	physics/physics.cpp
	physics/physics.h
)
target_include_directories(physics PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(physics PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(physics PROPERTIES FOLDER "modules")
