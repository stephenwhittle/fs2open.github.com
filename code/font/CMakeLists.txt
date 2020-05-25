add_library(font STATIC font/FSFont.h font/FSFont.cpp)
target_include_directories(font PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(font PUBLIC core)