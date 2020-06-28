add_library(color STATIC color/alphacolors.h color/alphacolors.cpp color/SCPColorTable.h color/SCPColorTable.cpp color/SCPColorTableDescriptor.h)

target_include_directories(color PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(color PRIVATE def_files parse cfile)
target_link_libraries(color PUBLIC core_interface mpark_variant)