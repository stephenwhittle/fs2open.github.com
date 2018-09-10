add_library(parse STATIC 
    parse/sexp.h
    parse/sexp.cpp
    parse/parselo.h
    parse/parselo.cpp
    parse/generic_log.h
    parse/generic_log.cpp
    parse/encrypt.h
    parse/encrypt.cpp
    parse/sexp/sexp_lookup.h
    parse/sexp/sexp_lookup.cpp
    parse/sexp/LuaSEXP.h
    parse/sexp/LuaSEXP.cpp
    parse/sexp/DynamicSEXP.h
    parse/sexp/DynamicSEXP.cpp
)

target_link_libraries( parse PUBLIC globalincs utils cmdline core)
target_include_directories(parse PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(parse PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(parse PROPERTIES FOLDER "modules")
