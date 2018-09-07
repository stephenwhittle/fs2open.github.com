add_library(globalincs INTERFACE)
target_link_libraries( globalincs INTERFACE windows_stub)
target_include_directories(globalincs INTERFACE ${CMAKE_CURRENT_LIST_DIR})
