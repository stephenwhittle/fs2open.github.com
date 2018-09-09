add_library(osapi STATIC 
osapi/DebugWindow.cpp osapi/DebugWindow.h 
osapi/dialogs.cpp osapi/dialogs.h 
osapi/osapi.cpp osapi/osapi.h 
osapi/osregistry.cpp osapi/osregistry.h 
osapi/outwnd.cpp osapi/outwnd.h
)
set_target_properties(osapi PROPERTIES FOLDER "modules")
target_include_directories(osapi PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(osapi PRIVATE cmdline globalincs core ${LUA_LIBS})
#MSVC does not define WIN32, it uses _WIN32 instead
if (WIN32)
target_compile_definitions(osapi PUBLIC WIN32)
endif()