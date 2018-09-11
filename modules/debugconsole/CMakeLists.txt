add_library( debugconsole STATIC
	CMakeLists.txt
	debugconsole/console.cpp
	debugconsole/console.h
	debugconsole/consolecmds.cpp
	debugconsole/consoleparse.cpp
	debugconsole/consoleparse.h
)
target_include_directories(debugconsole PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(debugconsole PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(debugconsole PROPERTIES FOLDER "modules")
