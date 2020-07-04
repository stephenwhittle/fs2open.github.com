add_library(module STATIC module/SCPModuleBase.h )

target_include_directories(module PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(module PUBLIC core optional)