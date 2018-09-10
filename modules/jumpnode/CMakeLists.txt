add_library( jumpnode STATIC
	jumpnode/jumpnode.cpp
	jumpnode/jumpnode.h
)
target_include_directories(jumpnode PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(jumpnode PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(jumpnode PROPERTIES FOLDER "modules")
