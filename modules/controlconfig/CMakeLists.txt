add_library( controlconfig STATIC
	controlconfig/controlsconfig.cpp
	controlconfig/controlsconfig.h
	controlconfig/controlsconfigcommon.cpp
)
target_include_directories(controlconfig PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(controlconfig PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(controlconfig PROPERTIES FOLDER "modules")
