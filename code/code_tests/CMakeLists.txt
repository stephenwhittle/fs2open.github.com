add_executable(code_tests codetests.cpp stub.cpp)
target_link_libraries(code_tests PUBLIC core cfile cmdline)