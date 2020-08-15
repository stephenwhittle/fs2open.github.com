add_library(cfile STATIC cfile/SCPCFileModule.h cfile/SCPCFileModule.cpp cfile/SCPCFileDBCustomSQL.cpp cfile/SCPCFile.h cfile/SCPCFile.cpp cfile/cfilelist.cpp cfile/cfilesystem.h cfile/encrypt.h cfile/encrypt.cpp cfile/SCPCFileDatabase.h cfile/SCPCFileDatabase.cpp)
target_include_directories(cfile PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(cfile PUBLIC core filesystem config memory lua51 utils module cmdline def_files mio SQLiteCpp sqlbuilder tutf8e)

#target_add_platform_files(cfile PUBLIC cfile/SCPCFile.h)