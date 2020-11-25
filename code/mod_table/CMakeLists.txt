add_library(mod_table STATIC mod_table/SCPGameSettingsTable.cpp mod_table/SCPGameSettingsModule.cpp)
target_include_directories(mod_table PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(mod_table PUBLIC parse core cfile)