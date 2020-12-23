add_library(species STATIC)
target_sources(species PRIVATE ${CMAKE_CURRENT_LIST_DIR}/species/SCPSpeciesDefaultsTable.cpp)
target_include_directories(species PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(species PUBLIC core module cfile parse color)
