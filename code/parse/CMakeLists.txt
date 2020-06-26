add_library(parse STATIC parse/parselo.h parse/parselo.cpp parse/SCPParser.h parse/SCPParser.cpp parse/SCPTable.h parse/SCPTable.cpp parse/SCPTableFormatDescriptor.h parse/SCPTableFormatDescriptor.cpp)
target_include_directories(parse PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(parse PUBLIC core filesystem memory config utils cfile def_files peglib optional)