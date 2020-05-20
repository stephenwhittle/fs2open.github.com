add_library(math STATIC 
	math/bitarray.h
	math/fix.cpp
	math/fix.h
	math/floating.cpp
	math/floating.h
	math/fvi.cpp
	math/fvi.h
	math/spline.cpp
	math/spline.h
	math/staticrand.cpp
	math/staticrand.h
	math/vecmat.cpp
	math/vecmat.h
	utils/RandomRange.h
)

target_include_directories(math PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(math PRIVATE core)
