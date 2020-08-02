add_library(color STATIC 
color/SCPColorTable.h 
color/SCPColorTable.cpp 
color/SCPColorTableDescriptor.h 
color/SCPColorModule.cpp 
color/SCPColorModule.h
color/SCPColor.h
color/SCPColor.cpp
)

target_include_directories(color PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(color PRIVATE def_files cfile)
target_link_libraries(color PUBLIC parse)