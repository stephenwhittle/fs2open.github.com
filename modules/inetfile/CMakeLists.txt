add_library( inetfile STATIC
	inetfile/cftp.cpp
	inetfile/cftp.h
	inetfile/chttpget.cpp
	inetfile/chttpget.h
	inetfile/inetgetfile.cpp
	inetfile/inetgetfile.h
)
target_include_directories(inetfile PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(inetfile PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(inetfile PROPERTIES FOLDER "modules")
