add_library(core STATIC)

target_sources(core PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/core.cpp
	${CMAKE_CURRENT_LIST_DIR}/format.cpp
	${CMAKE_CURRENT_LIST_DIR}/error.cpp
	${CMAKE_CURRENT_LIST_DIR}/path.cpp
	)

target_sources(core PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/core.h
	${CMAKE_CURRENT_LIST_DIR}/format.h
	${CMAKE_CURRENT_LIST_DIR}/error.h
	${CMAKE_CURRENT_LIST_DIR}/path.h
    ${CMAKE_CURRENT_LIST_DIR}/platform.h
	)
target_include_directories(core PUBLIC
	${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(core PUBLIC SimpleSignal dbgtools)