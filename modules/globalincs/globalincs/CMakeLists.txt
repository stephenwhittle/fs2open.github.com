target_sources(globalincs PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/alphacolors.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/mspdb_callstack.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/safe_strings.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/systemvars.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/version.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/windebug.cpp
    ${CMAKE_CURRENT_LIST_DIR}/memory/memory.cpp
)
target_include_directories(globalincs PRIVATE ${CMAKE_CURRENT_LIST_DIR}/toolchain)
target_include_directories(globalincs PRIVATE ${CMAKE_CURRENT_LIST_DIR}/memory)