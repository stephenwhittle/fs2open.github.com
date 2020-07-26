add_library(filesystem STATIC filesystem/SCPDirectoryIterator.h filesystem/SCPDirectoryIterator.cpp filesystem/SCPFilesystemModule.h filesystem/SCPFilesystemModule.cpp filesystem/SCPPath.h filesystem/SCPPath.cpp filesystem/SCPFile.h filesystem/SCPFile.cpp filesystem/SCPWorkingDirectory.h)

target_include_directories(filesystem PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(filesystem PUBLIC core platform)

target_link_libraries(filesystem PUBLIC ghc_filesystem cmdline module mpark_variant)