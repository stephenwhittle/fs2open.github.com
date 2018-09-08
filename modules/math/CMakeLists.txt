add_library(math STATIC math/fix.cpp math/fvi.cpp math/spline.cpp math/staticrand.cpp math/vecmat.cpp math/fix.h math/bitarray.h math/fvi.h math/spline.h math/staticrand.h math/vecmat.h math/RandomRange.h)
set_target_properties(math PROPERTIES FOLDER "modules")
target_include_directories(math PUBLIC ${CMAKE_CURRENT_LIST_DIR})
#TARGET_INCLUDE_DIRECTORIES(utils PRIVATE ${GENERATED_SOURCE_DIR})

target_link_libraries(math PRIVATE globalincs core)
#target_include_directories(utils PRIVATE ${CMAKE_CURRENT_LIST_DIR}/boost)