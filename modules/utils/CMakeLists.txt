add_library(utils STATIC utils/encoding.cpp utils/HeapAllocator.cpp utils/unicode.cpp )
add_subdirectory(utils)
set_target_properties(utils PROPERTIES FOLDER "modules")
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
#target_include_directories(utils PRIVATE ${PROJECT_SOURCE_DIR}/code)
TARGET_INCLUDE_DIRECTORIES(utils PRIVATE ${GENERATED_SOURCE_DIR})

target_link_libraries(utils globalincs utfcpp cfile)
#target_include_directories(utils PRIVATE ${CMAKE_CURRENT_LIST_DIR}/boost)