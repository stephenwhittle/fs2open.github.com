add_library( weapon STATIC
	weapon/beam.cpp
	weapon/beam.h
	weapon/corkscrew.cpp
	weapon/corkscrew.h
	weapon/emp.cpp
	weapon/emp.h
	weapon/flak.cpp
	weapon/flak.h
	weapon/muzzleflash.cpp
	weapon/muzzleflash.h
	weapon/shockwave.cpp
	weapon/shockwave.h
	weapon/swarm.cpp
	weapon/swarm.h
	weapon/trails.cpp
	weapon/trails.h
	weapon/weapon.h
	weapon/weapons.cpp
	weapon/weapon_flags.h
)
target_include_directories(weapon PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(weapon PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(weapon PROPERTIES FOLDER "modules")
