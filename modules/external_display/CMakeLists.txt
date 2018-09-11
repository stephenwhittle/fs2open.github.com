add_library( external_display STATIC
	
	external_display/displaylibrary.cpp
	external_display/displaylibrary.h
	external_display/external_display.cpp
	external_display/external_display.h
	external_display/external_display_internal.h
)
target_include_directories(external_display PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(external_display PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(external_display PROPERTIES FOLDER "modules")
