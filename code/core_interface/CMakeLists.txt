add_library(core INTERFACE)
target_include_directories(core INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(core INTERFACE fmt)
