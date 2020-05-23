add_library(config STATIC config/SCPConfig.h config/SCPConfig.cpp config/osregistry.h config/osregistry.cpp)

target_include_directories(config PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(config PRIVATE memory sdl2)
target_link_libraries(config PUBLIC filesystem)