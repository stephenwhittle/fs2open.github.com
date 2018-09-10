add_library( events STATIC
	CMakeLists.txt
	events/events.cpp
	events/events.h
)
target_include_directories(events PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(events PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(events PROPERTIES FOLDER "modules")
