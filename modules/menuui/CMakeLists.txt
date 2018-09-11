add_library( menuui STATIC
	
	menuui/barracks.cpp
	menuui/barracks.h
	menuui/credits.cpp
	menuui/credits.h
	menuui/fishtank.cpp
	menuui/fishtank.h
	menuui/mainhallmenu.cpp
	menuui/mainhallmenu.h
	menuui/mainhalltemp.cpp
	menuui/mainhalltemp.h
	menuui/optionsmenu.cpp
	menuui/optionsmenu.h
	menuui/optionsmenumulti.cpp
	menuui/optionsmenumulti.h
	menuui/playermenu.cpp
	menuui/playermenu.h
	menuui/readyroom.cpp
	menuui/readyroom.h
	menuui/snazzyui.cpp
	menuui/snazzyui.h
	menuui/techmenu.cpp
	menuui/techmenu.h
	menuui/trainingmenu.cpp
	menuui/trainingmenu.h
)
target_include_directories(menuui PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(menuui PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(menuui PROPERTIES FOLDER "modules")
