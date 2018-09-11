add_library( missionui STATIC
	
	missionui/chatbox.cpp
	missionui/chatbox.h
	missionui/fictionviewer.cpp
	missionui/fictionviewer.h
	missionui/missionbrief.cpp
	missionui/missionbrief.h
	missionui/missioncmdbrief.cpp
	missionui/missioncmdbrief.h
	missionui/missiondebrief.cpp
	missionui/missiondebrief.h
	missionui/missionloopbrief.cpp
	missionui/missionloopbrief.h
	missionui/missionpause.cpp
	missionui/missionpause.h
	missionui/missionscreencommon.cpp
	missionui/missionscreencommon.h
	missionui/missionshipchoice.cpp
	missionui/missionshipchoice.h
	missionui/missionweaponchoice.cpp
	missionui/missionweaponchoice.h
	missionui/redalert.cpp
	missionui/redalert.h
)
target_include_directories(missionui PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(missionui PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(missionui PROPERTIES FOLDER "modules")
