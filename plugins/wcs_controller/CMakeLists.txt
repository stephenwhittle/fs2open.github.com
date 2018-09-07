add_library(ExternalDisplay MODULE main.cpp)
set_target_properties(ExternalDisplay PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS true FOLDER "Plugins")
target_link_libraries(ExternalDisplay bridge)
if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
    set_target_properties(ExternalDisplay PROPERTIES OUTPUT_NAME "ExternalDisplay64")
else()
    set_target_properties(ExternalDisplay PROPERTIES OUTPUT_NAME "ExternalDisplay")
endif("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")

add_target_copy_files($<TARGET_FILE:ExternalDisplay>)

