add_library(config STATIC config/SCPConfigModule.h config/SCPConfig.h config/SCPConfig.cpp config/osregistry.h config/osregistry.cpp "config/ConfigProfile.h")

target_include_directories(config PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(config PRIVATE memory sdl2)
target_link_libraries(config INTERFACE leksysini)
target_link_libraries(config PUBLIC filesystem cmdline module)