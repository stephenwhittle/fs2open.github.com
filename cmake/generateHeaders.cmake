

include(Cxx11)
include(platformChecks)

CONFIGURE_FILE(${CMAKE_CURRENT_LIST_DIR}/project.h.in ${GENERATED_SOURCE_DIR}/project.h)
CONFIGURE_FILE(${CMAKE_CURRENT_LIST_DIR}/fred_rc.h.in ${GENERATED_SOURCE_DIR}/fred_rc.h)


add_library(generated INTERFACE)
target_include_directories(generated INTERFACE ${GENERATED_SOURCE_DIR})
target_sources(generated INTERFACE
    ${GENERATED_SOURCE_DIR}/project.h
    ${GENERATED_SOURCE_DIR}/fred_rc.h
    ${GENERATED_SOURCE_DIR}/scp_compiler_detection.h
    ${GENERATED_SOURCE_DIR}/platformChecks.h) 