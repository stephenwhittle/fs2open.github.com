add_library( stats STATIC

	stats/medals.cpp
	stats/medals.h
	stats/scoring.cpp
	stats/scoring.h
	stats/stats.cpp
	stats/stats.h
)
target_include_directories(stats PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(stats PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(stats PROPERTIES FOLDER "modules")
