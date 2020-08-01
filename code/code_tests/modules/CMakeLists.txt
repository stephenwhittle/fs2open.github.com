
set (test_sources 
	cfile/cfile.cpp 
	config/config.cpp 
	cmdline/cmdline.cpp
	filesystem/filesystem.cpp
	log/log.cpp
)

list (TRANSFORM test_sources PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

target_sources(code_tests PUBLIC ${test_sources})