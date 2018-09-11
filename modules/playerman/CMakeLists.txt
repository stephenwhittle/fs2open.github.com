add_library( playerman STATIC
	
	playerman/managepilot.cpp
	playerman/managepilot.h
	playerman/player.h
	playerman/playercontrol.cpp
)
target_include_directories(playerman PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(playerman PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(playerman PROPERTIES FOLDER "modules")
