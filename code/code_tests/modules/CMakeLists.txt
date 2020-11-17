
set (test_sources 
	cfile/cfile.cpp
	cfile/encoding_tests.cpp 
	cfile/encryption_tests.cpp
	config/config.cpp 
	cmdline/cmdline.cpp
	filesystem/filesystem.cpp
	log/log.cpp
	color/color.cpp
	asteroid/asteroid.cpp
)

list (TRANSFORM test_sources PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

target_sources(code_tests PUBLIC ${test_sources})