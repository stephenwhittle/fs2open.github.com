add_library( pcxutils STATIC
	
	pcxutils/pcxutils.cpp
	pcxutils/pcxutils.h
)
target_include_directories(pcxutils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(pcxutils PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(pcxutils PROPERTIES FOLDER "modules")
