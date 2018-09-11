add_library( cmeasure STATIC
	
	cmeasure/cmeasure.cpp
	cmeasure/cmeasure.h
)
target_include_directories(cmeasure PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(cmeasure PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(cmeasure PROPERTIES FOLDER "modules")
