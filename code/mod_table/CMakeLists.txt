add_library(mod_table STATIC mod_table/SCPGameSettingsTable.cpp)
target_include_directories(mod_table INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(mod_table PUBLIC parse core)