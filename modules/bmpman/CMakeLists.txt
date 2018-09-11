add_library( bmpman STATIC
	bmpman/bmpman.cpp
	bmpman/bmpman.h
	bmpman/bm_examples.cpp
	bmpman/bm_internal.h
	
)
target_include_directories(bmpman PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(bmpman PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(bmpman PROPERTIES FOLDER "modules")
