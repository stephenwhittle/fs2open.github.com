add_library( cutscene STATIC
	
	cutscene/cutscenes.cpp
	cutscene/cutscenes.h
	cutscene/Decoder.cpp
	cutscene/Decoder.h
	cutscene/ffmpeg/AudioDecoder.cpp
	cutscene/ffmpeg/AudioDecoder.h
	cutscene/ffmpeg/FFMPEGDecoder.cpp
	cutscene/ffmpeg/FFMPEGDecoder.h
	cutscene/ffmpeg/internal.cpp
	cutscene/ffmpeg/internal.h
	cutscene/ffmpeg/SubtitleDecoder.cpp
	cutscene/ffmpeg/SubtitleDecoder.h
	cutscene/ffmpeg/VideoDecoder.cpp
	cutscene/ffmpeg/VideoDecoder.h
	cutscene/movie.cpp
	cutscene/movie.h
	cutscene/player.cpp
	cutscene/player.h
	cutscene/VideoPresenter.cpp
	cutscene/VideoPresenter.h
)
target_include_directories(cutscene PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(cutscene PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(cutscene PROPERTIES FOLDER "modules")
