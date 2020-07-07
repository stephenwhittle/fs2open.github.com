add_library(core INTERFACE)
target_include_directories(core INTERFACE ${CMAKE_CURRENT_LIST_DIR} ${GENERATED_SOURCE_DIR})
target_link_libraries(core INTERFACE fmt platform ghc_filesystem)

target_add_platform_files(core INTERFACE SCPGetPrefsPath.h)