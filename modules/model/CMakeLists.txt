add_library( model STATIC
	
	model/model.h
	model/modelanim.cpp
	model/modelanim.h
	model/modelcollide.cpp
	model/modelinterp.cpp
	model/modeloctant.cpp
	model/modelread.cpp
	model/modelrender.cpp
	model/modelrender.h
	model/modelsinc.h
	model/model_flags.h
)
target_include_directories(model PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(model PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(model PROPERTIES FOLDER "modules")
