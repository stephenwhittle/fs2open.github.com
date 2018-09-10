add_library( gamehelp STATIC
	gamehelp/contexthelp.cpp
	gamehelp/contexthelp.h
	gamehelp/gameplayhelp.cpp
	gamehelp/gameplayhelp.h
)
target_include_directories(gamehelp PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(gamehelp PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(gamehelp PROPERTIES FOLDER "modules")
