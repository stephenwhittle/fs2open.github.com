add_library(filesystem STATIC filesystem/SCPPath.h filesystem/SCPPath.cpp filesystem/SCPFile.h filesystem/SCPFile.h)

target_include_directories(filesystem PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(filesystem PUBLIC core)

target_link_libraries(filesystem PUBLIC ghc_filesystem)