add_library(core STATIC)

target_sources(core PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/core.cpp
	${CMAKE_CURRENT_LIST_DIR}/format.cpp
	${CMAKE_CURRENT_LIST_DIR}/error.cpp
	${CMAKE_CURRENT_LIST_DIR}/path.cpp
	${CMAKE_CURRENT_LIST_DIR}/toolchain/clang.h
	${CMAKE_CURRENT_LIST_DIR}/toolchain/doxygen.h
	${CMAKE_CURRENT_LIST_DIR}/toolchain/gcc.h
	${CMAKE_CURRENT_LIST_DIR}/toolchain/mingw.h
	${CMAKE_CURRENT_LIST_DIR}/toolchain/msvc.h
	)

target_sources(core PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/core/core.h
	${CMAKE_CURRENT_LIST_DIR}/core/format.h
	${CMAKE_CURRENT_LIST_DIR}/core/error.h
	${CMAKE_CURRENT_LIST_DIR}/core/path.h
    ${CMAKE_CURRENT_LIST_DIR}/core/platform.h
    ${CMAKE_CURRENT_LIST_DIR}/core/pstypes.h
    ${CMAKE_CURRENT_LIST_DIR}/core/toolchain.h
	)
target_include_directories(core PUBLIC
	${CMAKE_CURRENT_LIST_DIR})

target_compile_options( core PUBLIC -utf-8)
target_link_libraries(core PUBLIC simplesignal dbgtools ghc_filesystem)
