add_library(asteroid INTERFACE)
target_include_directories(asteroid INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(asteroid INTERFACE core parse)
