add_library(sound INTERFACE)
target_include_directories(sound INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(sound INTERFACE core module)
#FSO_BUILD_WITH_FFMPEG