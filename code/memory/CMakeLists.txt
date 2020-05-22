add_library(memory STATIC memory/memory.h memory/memory.cpp memory/utils.h)

target_include_directories(memory PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(memory PUBLIC core)
