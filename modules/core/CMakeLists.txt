add_library(core STATIC memory/memory.cpp)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(core PUBLIC 
    "$<$<COMPILE_FEATURES:cxx_variadic_macros>:HAS_VARIADIC_MACROS=1>"
    "$<$<NOT:$<COMPILE_FEATURES:cxx_variadic_macros>>:HAS_VARIADIC_MACROS=0>"
)
set_target_properties(core PROPERTIES FOLDER "modules")
target_link_libraries(core osapi globalincs)