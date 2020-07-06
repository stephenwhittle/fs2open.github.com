add_library(cmdline STATIC cmdline/SCPCmdlineModule.h cmdline/SCPCmdlineModule.cpp cmdline/SCPCmdLine.h cmdline/SCPCmdLine.cpp cmdline/SCPCmdLineOption.natvis)

target_include_directories(cmdline PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(cmdline PUBLIC core peglib module optional)