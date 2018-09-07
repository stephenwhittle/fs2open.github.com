add_library(windows_stub STATIC windows_stub/stubs.cpp windows_stub/config.h)
set_target_properties(windows_stub PROPERTIES FOLDER "windows_stub")
target_include_directories(windows_stub PUBLIC ${CMAKE_CURRENT_LIST_DIR})

TARGET_LINK_LIBRARIES(windows_stub PUBLIC sdl2)