add_library( nebula STATIC
	
	nebula/neb.cpp
	nebula/neb.h
	nebula/neblightning.cpp
	nebula/neblightning.h
)
target_include_directories(nebula PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(nebula PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(nebula PROPERTIES FOLDER "modules")
