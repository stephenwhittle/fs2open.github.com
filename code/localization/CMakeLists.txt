add_library(localization STATIC localization/localize.h localization/localize.cpp localization/fhash.h localization/fhash.cpp)
target_include_directories(localization PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(localization PUBLIC core filesystem memory config parse cfile font)