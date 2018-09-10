add_library( autopilot STATIC
	autopilot/autopilot.cpp
	autopilot/autopilot.h

)
target_include_directories(autopilot PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(autopilot PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(autopilot PROPERTIES FOLDER "modules")
