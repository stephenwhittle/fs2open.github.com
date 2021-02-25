add_library(graphics STATIC graphics/SCPGraphicsModule.h graphics/Viewport.h graphics/GraphicsOperation.h  graphics/2d.h graphics/2d.cpp graphics/OpenGLContext.h)

target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(graphics PUBLIC module optional math cfile color)