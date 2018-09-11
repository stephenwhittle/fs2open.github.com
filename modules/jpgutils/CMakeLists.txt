add_library( jpgutils STATIC
	
	jpgutils/jpgutils.cpp
	jpgutils/jpgutils.h
)
target_include_directories(jpgutils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(jpgutils PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(jpgutils PROPERTIES FOLDER "modules")
