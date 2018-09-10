add_library(io STATIC 
    io/cursor.cpp
    io/joy_ff-sdl.cpp
    io/joy-sdl.cpp
    io/key.cpp
    io/mouse.cpp
    io/timer.cpp
    io/cursor.h
    io/joy_ff.h
    io/joy.h
    io/key.h
    io/mouse.h
    io/timer.h
)

target_link_libraries( io PUBLIC globalincs utils cmdline core)
target_include_directories(io PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(io PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(io PROPERTIES FOLDER "modules")
