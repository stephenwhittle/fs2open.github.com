add_library(math STATIC)

target_sources(math PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/fix.cpp
	${CMAKE_CURRENT_LIST_DIR}/floating.cpp
	${CMAKE_CURRENT_LIST_DIR}/fvi.cpp
	${CMAKE_CURRENT_LIST_DIR}/spline.cpp
	${CMAKE_CURRENT_LIST_DIR}/staticrand.cpp
	${CMAKE_CURRENT_LIST_DIR}/vecmat.cpp
	)

target_sources(math PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/math/fix.h
	${CMAKE_CURRENT_LIST_DIR}/math/floating.h
	${CMAKE_CURRENT_LIST_DIR}/math/fvi.h
	${CMAKE_CURRENT_LIST_DIR}/math/spline.h
	${CMAKE_CURRENT_LIST_DIR}/math/staticrand.h
	${CMAKE_CURRENT_LIST_DIR}/math/bitarray.h
	${CMAKE_CURRENT_LIST_DIR}/math/RandomRange.h
	${CMAKE_CURRENT_LIST_DIR}/math/vecmat.h
	)
target_include_directories(math PUBLIC
	${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(math PUBLIC core)
