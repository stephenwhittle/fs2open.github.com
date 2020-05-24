add_subdirectory(effects)
add_subdirectory(maps)
add_subdirectory(scripts)
add_subdirectory(tables)

#propagate the list up to enclosing scope
set (default_files "${default_files}" PARENT_SCOPE)

