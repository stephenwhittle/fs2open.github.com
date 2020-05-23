add_library(log STATIC log/generic_log.h log/generic_log.cpp)

target_include_directories(log PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(log PUBLIC core fmt cfile)
