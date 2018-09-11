add_library( tracing STATIC
	tracing/categories.cpp
	tracing/categories.h
	tracing/FrameProfiler.cpp
	tracing/FrameProfiler.h
	tracing/MainFrameTimer.cpp
	tracing/MainFrameTimer.h
	tracing/Monitor.cpp
	tracing/Monitor.h
	tracing/scopes.cpp
	tracing/scopes.h
	tracing/ThreadedEventProcessor.h
	tracing/TraceEventWriter.cpp
	tracing/TraceEventWriter.h
	tracing/tracing.cpp
	tracing/tracing.h
)
target_include_directories(tracing PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(tracing PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(tracing PROPERTIES FOLDER "modules")
