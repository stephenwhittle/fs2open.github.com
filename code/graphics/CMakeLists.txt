add_library(graphics STATIC graphics/SCPGraphicsModule.h graphics/Viewport.h graphics/GraphicsOperation.h  "graphics/OpenGLContext.h")

target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(graphics PUBLIC module optional)