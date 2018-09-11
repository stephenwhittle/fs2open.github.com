add_library( pilotfile STATIC
	
	pilotfile/BinaryFileHandler.cpp
	pilotfile/BinaryFileHandler.h
	pilotfile/csg.cpp
	pilotfile/csg_convert.cpp
	pilotfile/FileHandler.h
	pilotfile/JSONFileHandler.cpp
	pilotfile/JSONFileHandler.h
	pilotfile/pilotfile.cpp
	pilotfile/pilotfile.h
	pilotfile/pilotfile_convert.cpp
	pilotfile/pilotfile_convert.h
	pilotfile/plr.cpp
	pilotfile/plr_convert.cpp
)
target_include_directories(pilotfile PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(pilotfile PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(pilotfile PROPERTIES FOLDER "modules")
