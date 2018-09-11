add_library( localization STATIC
	
	localization/fhash.cpp
	localization/fhash.h
	localization/localize.cpp
	localization/localize.h
)
target_include_directories(localization PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(localization PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(localization PROPERTIES FOLDER "modules")
