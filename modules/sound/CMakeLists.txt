add_library(sound STATIC 
    sound/audiostr.cpp
    sound/ds.cpp
    sound/ds3d.cpp
    sound/dscap.cpp
    sound/fsspeech.cpp
    sound/openal.cpp
    sound/rtvoice.cpp
    sound/sound.cpp
    sound/speech.cpp
    sound/voicerec.cpp
)

target_link_libraries( sound PUBLIC globalincs utils openal cmdline core ffmpeg_support)
target_include_directories(sound PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(sound PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(sound PROPERTIES FOLDER "modules")
if (MSVC)
target_compile_definitions(sound PUBLIC SCP_FORMAT_STRING=_Printf_format_string_)
else()
target_compile_definitions(sound PUBLIC SCP_FORMAT_STRING=)
endif()
