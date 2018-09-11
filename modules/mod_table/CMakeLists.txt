add_library( mod_table STATIC
	
	mod_table/mod_table.cpp
	mod_table/mod_table.h
)
target_include_directories(mod_table PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(mod_table PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(mod_table PROPERTIES FOLDER "modules")
