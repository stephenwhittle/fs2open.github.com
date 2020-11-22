add_library(mod_table INTERFACE)
target_include_directories(mod_table INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(mod_table INTERFACE parse core)