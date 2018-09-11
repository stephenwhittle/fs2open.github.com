add_library( popup STATIC
	
	popup/popup.cpp
	popup/popup.h
	popup/popupdead.cpp
	popup/popupdead.h
)
target_include_directories(popup PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(popup PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(popup PROPERTIES FOLDER "modules")
