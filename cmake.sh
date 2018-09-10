#!/bin/bash
echo "add_library( ${PWD##*/} STATIC" > CMakeLists.txt

find -type f -printf '\t%P\n' >> CMakeLists.txt

echo ")" >> CMakeLists.txt

echo "target_include_directories(${PWD##*/} PUBLIC \${CMAKE_CURRENT_LIST_DIR})" >> CMakeLists.txt
echo "TARGET_INCLUDE_DIRECTORIES(${PWD##*/} PRIVATE \${GENERATED_SOURCE_DIR})" >> CMakeLists.txt
echo "set_target_properties(${PWD##*/} PROPERTIES FOLDER \"modules\")" >> CMakeLists.txt

