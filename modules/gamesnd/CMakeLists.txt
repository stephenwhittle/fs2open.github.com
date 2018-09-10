add_library( gamesnd STATIC
	gamesnd/eventmusic.cpp
	gamesnd/eventmusic.h
	gamesnd/gamesnd.cpp
	gamesnd/gamesnd.h
)
target_include_directories(gamesnd PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(gamesnd PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(gamesnd PROPERTIES FOLDER "modules")
