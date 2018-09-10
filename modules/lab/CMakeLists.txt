add_library( lab STATIC
	lab/lab.cpp
	lab/lab.h
	lab/wmcgui.cpp
	lab/wmcgui.h
)
target_include_directories(lab PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(lab PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(lab PROPERTIES FOLDER "modules")
