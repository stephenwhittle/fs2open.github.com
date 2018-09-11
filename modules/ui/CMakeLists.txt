add_library( ui STATIC
	ui/button.cpp
	ui/checkbox.cpp
	ui/gadget.cpp
	ui/icon.cpp
	ui/inputbox.cpp
	ui/keytrap.cpp
	ui/listbox.cpp
	ui/radio.cpp
	ui/scroll.cpp
	ui/slider.cpp
	ui/slider2.cpp
	ui/ui.h
	ui/uidefs.h
	ui/uidraw.cpp
	ui/uimouse.cpp
	ui/window.cpp
)
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(ui PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(ui PROPERTIES FOLDER "modules")
