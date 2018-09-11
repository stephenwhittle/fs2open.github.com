add_library( observer STATIC
	
	observer/observer.cpp
	observer/observer.h
)
target_include_directories(observer PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(observer PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(observer PROPERTIES FOLDER "modules")
