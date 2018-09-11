add_library( render STATIC
	
	render/3d.h
	render/3dclipper.cpp
	render/3ddraw.cpp
	render/3dinternal.h
	render/3dlaser.cpp
	render/3dmath.cpp
	render/3dsetup.cpp
	render/batching.cpp
	render/batching.h
)
target_include_directories(render PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(render PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(render PROPERTIES FOLDER "modules")
