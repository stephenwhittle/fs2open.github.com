add_library( exceptionhandler STATIC
	
	exceptionhandler/exceptionhandler.cpp
	exceptionhandler/exceptionhandler.h
)
target_include_directories(exceptionhandler PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(exceptionhandler PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(exceptionhandler PROPERTIES FOLDER "modules")
