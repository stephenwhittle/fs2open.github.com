add_library( radar STATIC
	
	radar/radar.cpp
	radar/radar.h
	radar/radardradis.cpp
	radar/radardradis.h
	radar/radarorb.cpp
	radar/radarorb.h
	radar/radarsetup.cpp
	radar/radarsetup.h
)
target_include_directories(radar PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(radar PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(radar PROPERTIES FOLDER "modules")
