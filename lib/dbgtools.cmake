add_library(dbgtools STATIC)
target_include_directories(dbgtools PUBLIC ${CMAKE_CURRENT_LIST_DIR}/dbgtools/include)
target_sources(dbgtools PRIVATE 
	${CMAKE_CURRENT_LIST_DIR}/dbgtools/src/assert.cpp
	${CMAKE_CURRENT_LIST_DIR}/dbgtools/src/callstack.cpp
	${CMAKE_CURRENT_LIST_DIR}/dbgtools/src/debugger.cpp
	${CMAKE_CURRENT_LIST_DIR}/dbgtools/src/fpe_ctrl.cpp
	${CMAKE_CURRENT_LIST_DIR}/dbgtools/src/hw_breakpoint.cpp)
if (WIN32 AND (NOT MINGW AND( NOT CYGWIN)))
	target_link_libraries(dbgtools dbghelp)
else()
	target_compile_options(dbgtools -rdynamic)
endif ()
