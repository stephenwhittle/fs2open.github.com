add_library( species_defs STATIC
	
	species_defs/species_defs.cpp
	species_defs/species_defs.h
)
target_include_directories(species_defs PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(species_defs PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(species_defs PROPERTIES FOLDER "modules")
