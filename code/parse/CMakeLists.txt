add_library(parse STATIC parse/parselo.h parse/parselo.cpp)
target_include_directories(parse PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(parse PUBLIC core filesystem memory config utils localization)