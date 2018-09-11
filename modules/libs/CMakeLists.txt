add_library( libs STATIC
	
	discord/discord.cpp
	discord/discord.h
	renderdoc/renderdoc.cpp
	renderdoc/renderdoc.h
	renderdoc/renderdoc_app.h
)
target_include_directories(libs PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(libs PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(libs PROPERTIES FOLDER "modules")
