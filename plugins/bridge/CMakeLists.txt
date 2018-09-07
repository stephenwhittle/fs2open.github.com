add_library(bridge STATIC hud_events.cpp hud_events.h)
set_target_properties(bridge PROPERTIES FOLDER "Plugins")
target_link_libraries(bridge PUBLIC code)
target_include_directories(bridge PUBLIC ${CMAKE_CURRENT_LIST_DIR})