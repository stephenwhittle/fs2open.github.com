add_library(sqlite_orm INTERFACE)
target_include_directories(sqlite_orm INTERFACE ${CMAKE_CURRENT_LIST_DIR}/sqlite_orm/include)
target_link_libraries(sqlite_orm INTERFACE sqlite)