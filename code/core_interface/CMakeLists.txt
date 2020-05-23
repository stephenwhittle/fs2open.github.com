add_library(core INTERFACE)
target_include_directories(core INTERFACE ${CMAKE_CURRENT_LIST_DIR} ${GENERATED_SOURCE_DIR})
target_link_libraries(core INTERFACE fmt platform)

source_group(code\\core FILES FSOutputDeviceBase.h FSMathTypes.h FSColorTypes.h FSAssert.h FSIntegerTypes.h FSMathOps.h FSStdTypes.h SCPCmdOptions.h SCPLimits.h)