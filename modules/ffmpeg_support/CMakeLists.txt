add_library(ffmpeg_support STATIC 
ffmpeg/FFmpeg.cpp
ffmpeg/FFmpegContext.cpp
)
set_target_properties(ffmpeg_support PROPERTIES FOLDER "modules")
target_include_directories(ffmpeg_support PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(ffmpeg_support PRIVATE ${GENERATED_SOURCE_DIR})
target_link_libraries(ffmpeg_support PRIVATE cfile globalincs )
#MSVC does not define WIN32, it uses _WIN32 instead
if (WIN32)
target_compile_definitions(ffmpeg_support PUBLIC WIN32)
endif()