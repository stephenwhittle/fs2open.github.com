add_library( ship STATIC
	
	ship/afterburner.cpp
	ship/afterburner.h
	ship/awacs.cpp
	ship/awacs.h
	ship/shield.cpp
	ship/ship.cpp
	ship/ship.h
	ship/shipcontrails.cpp
	ship/shipcontrails.h
	ship/shipfx.cpp
	ship/shipfx.h
	ship/shiphit.cpp
	ship/shiphit.h
	ship/ship_flags.h
	ship/subsysdamage.h
)
target_include_directories(ship PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(ship PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(ship PROPERTIES FOLDER "modules")
