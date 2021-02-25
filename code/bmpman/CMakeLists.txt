add_library(bmpman STATIC bmpman/bmpman.cpp bmpman/SCPBitmapManager.h bmpman/SCPBitmapTypes.h bmpman/SCPBitmapManager.cpp)
target_include_directories(bmpman PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries (bmpman PUBLIC module core cfile)