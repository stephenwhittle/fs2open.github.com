add_library(cmdline STATIC cmdline/SCPCmdLine.h cmdline/SCPCmdLine.cpp)

target_include_directories(cmdline PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(cmdline PUBLIC core peglib optional)