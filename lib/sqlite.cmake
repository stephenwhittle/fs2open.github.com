add_library(sqlite STATIC sqlite-amalgamation/sqlite3.c sqlite-amalgamation/sqlite3.h )
target_include_directories(sqlite PUBLIC ${CMAKE_CURRENT_LIST_DIR}/sqlite-amalgamation)