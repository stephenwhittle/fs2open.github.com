add_library( starfield STATIC
	
	starfield/nebula.cpp
	starfield/nebula.h
	starfield/starfield.cpp
	starfield/starfield.h
	starfield/supernova.cpp
	starfield/supernova.h
)
target_include_directories(starfield PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(starfield PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(starfield PROPERTIES FOLDER "modules")
