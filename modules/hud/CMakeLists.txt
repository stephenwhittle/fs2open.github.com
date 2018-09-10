add_library( hud STATIC
	hud/hud.cpp
	hud/hud.h
	hud/hudartillery.cpp
	hud/hudartillery.h
	hud/hudbrackets.cpp
	hud/hudbrackets.h
	hud/hudconfig.cpp
	hud/hudconfig.h
	hud/hudescort.cpp
	hud/hudescort.h
	hud/hudets.cpp
	hud/hudets.h
	hud/hudgauges.h
	hud/hudlock.cpp
	hud/hudlock.h
	hud/hudmessage.cpp
	hud/hudmessage.h
	hud/hudnavigation.cpp
	hud/hudnavigation.h
	hud/hudobserver.cpp
	hud/hudobserver.h
	hud/hudparse.cpp
	hud/hudparse.h
	hud/hudreticle.cpp
	hud/hudreticle.h
	hud/hudshield.cpp
	hud/hudshield.h
	hud/hudsquadmsg.cpp
	hud/hudsquadmsg.h
	hud/hudtarget.cpp
	hud/hudtarget.h
	hud/hudtargetbox.cpp
	hud/hudtargetbox.h
	hud/hudwingmanstatus.cpp
	hud/hudwingmanstatus.h
)
target_include_directories(hud PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(hud PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(hud PROPERTIES FOLDER "modules")
