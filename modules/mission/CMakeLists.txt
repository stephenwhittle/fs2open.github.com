add_library( mission STATIC
	
	mission/missionbriefcommon.cpp
	mission/missionbriefcommon.h
	mission/missioncampaign.cpp
	mission/missioncampaign.h
	mission/missiongoals.cpp
	mission/missiongoals.h
	mission/missiongrid.cpp
	mission/missiongrid.h
	mission/missionhotkey.cpp
	mission/missionhotkey.h
	mission/missionload.cpp
	mission/missionload.h
	mission/missionlog.cpp
	mission/missionlog.h
	mission/missionmessage.cpp
	mission/missionmessage.h
	mission/missionparse.cpp
	mission/missionparse.h
	mission/missiontraining.cpp
	mission/missiontraining.h
	mission/mission_flags.h
)
target_include_directories(mission PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(mission PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(mission PROPERTIES FOLDER "modules")
