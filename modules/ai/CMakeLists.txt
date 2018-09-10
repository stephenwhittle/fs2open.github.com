add_library( ai STATIC
	ai/ai.cpp
	ai/ai.h
	ai/aibig.cpp
	ai/aibig.h
	ai/aicode.cpp
	ai/aigoals.cpp
	ai/aigoals.h
	ai/aiinternal.h
	ai/aiturret.cpp
	ai/ai_flags.h
	ai/ai_profiles.cpp
	ai/ai_profiles.h
)
target_include_directories(ai PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(ai PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(ai PROPERTIES FOLDER "modules")
target_link_libraries(ai PUBLIC globalincs)