add_library(cfile STATIC cfile/cfile.h cfile/cfile.cpp cfile/cfilearchive.h cfile/cfilearchive.cpp cfile/cfilelist.cpp cfile/cfilesystem.h cfile/cfilesystem.cpp)
target_include_directories(cfile PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(cfile PUBLIC core filesystem config memory lua51 utils def_files)