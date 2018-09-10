add_library( headtracking STATIC
	headtracking/freetrack.cpp
	headtracking/freetrack.h
	headtracking/headtracking.cpp
	headtracking/headtracking.h
	headtracking/headtracking_internal.h
	headtracking/trackir.cpp
	headtracking/trackir.h
	headtracking/trackirpublic.cpp
	headtracking/trackirpublic.h
)
target_include_directories(headtracking PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(headtracking PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(headtracking PROPERTIES FOLDER "modules")
