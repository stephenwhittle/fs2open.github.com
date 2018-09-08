add_library(cfile STATIC cfile/cfile.cpp cfile/cfilearchive.cpp cfile/cfilelist.cpp cfile/cfilesystem.cpp )
set_target_properties(cfile PROPERTIES FOLDER "modules")
target_include_directories(cfile PUBLIC ${CMAKE_CURRENT_LIST_DIR})
#TARGET_INCLUDE_DIRECTORIES(utils PRIVATE ${GENERATED_SOURCE_DIR})

target_link_libraries(cfile globalincs core)
#target_include_directories(utils PRIVATE ${CMAKE_CURRENT_LIST_DIR}/boost)