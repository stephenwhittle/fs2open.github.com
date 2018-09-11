add_library( object STATIC
	
	object/collidedebrisship.cpp
	object/collidedebrisweapon.cpp
	object/collideshipship.cpp
	object/collideshipweapon.cpp
	object/collideweaponweapon.cpp
	object/deadobjectdock.cpp
	object/deadobjectdock.h
	object/objcollide.cpp
	object/objcollide.h
	object/object.cpp
	object/object.h
	object/objectdock.cpp
	object/objectdock.h
	object/objectshield.cpp
	object/objectshield.h
	object/objectsnd.cpp
	object/objectsnd.h
	object/objectsort.cpp
	object/object_flags.h
	object/parseobjectdock.cpp
	object/parseobjectdock.h
	object/waypoint.cpp
	object/waypoint.h
)
target_include_directories(object PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(object PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(object PROPERTIES FOLDER "modules")
