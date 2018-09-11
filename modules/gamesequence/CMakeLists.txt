add_library( gamesequence STATIC
	
	gamesequence/gamesequence.cpp
	gamesequence/gamesequence.h
)
target_include_directories(gamesequence PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(gamesequence PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(gamesequence PROPERTIES FOLDER "modules")
