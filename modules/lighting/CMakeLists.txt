add_library( lighting STATIC
	
	lighting/lighting.cpp
	lighting/lighting.h
)
target_include_directories(lighting PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(lighting PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(lighting PROPERTIES FOLDER "modules")
