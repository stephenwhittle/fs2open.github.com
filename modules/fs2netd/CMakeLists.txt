add_library( fs2netd STATIC
	fs2netd/fs2netd_client.cpp
	fs2netd/fs2netd_client.h
	fs2netd/protocol.h
	fs2netd/tcp_client.cpp
	fs2netd/tcp_client.h
	fs2netd/tcp_socket.cpp
	fs2netd/tcp_socket.h
)
target_include_directories(fs2netd PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(fs2netd PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(fs2netd PROPERTIES FOLDER "modules")
