add_library( debris STATIC
	
	debris/debris.cpp
	debris/debris.h
)
target_include_directories(debris PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(debris PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(debris PROPERTIES FOLDER "modules")
