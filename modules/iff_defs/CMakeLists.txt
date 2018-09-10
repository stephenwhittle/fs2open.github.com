add_library( iff_defs STATIC
	iff_defs/iff_defs.cpp
	iff_defs/iff_defs.h
)
target_include_directories(iff_defs PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(iff_defs PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(iff_defs PROPERTIES FOLDER "modules")
