add_library(globalincs STATIC)
target_link_libraries( globalincs PRIVATE windows_stub)
target_include_directories(globalincs PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(globalincs PRIVATE ${GENERATED_SOURCE_DIR})

add_subdirectory(globalincs)