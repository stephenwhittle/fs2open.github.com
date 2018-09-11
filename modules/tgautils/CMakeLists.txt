add_library( tgautils STATIC
	tgautils/tgautils.cpp
	tgautils/tgautils.h
)
target_include_directories(tgautils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(tgautils PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(tgautils PROPERTIES FOLDER "modules")
