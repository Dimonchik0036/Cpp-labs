cmake_minimum_required(VERSION 3.8)

project(hashtable)

# library with common functions for tests and exe

add_library(hashtablelib
        src/hash_table.h
        src/hash_table.tpp
        src/hash_table.cpp
        src/linked_hash_entry.h
        src/linked_hash_entry.tpp)

set_target_properties(hashtablelib PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

#
# project itself
#
add_executable(hashtable
        src/hash_table.h
        src/main.cpp)

add_dependencies(hashtable hashtablelib)

set_target_properties(hashtable PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

target_link_libraries(hashtable hashtablelib)

#
# unit tests
#
add_executable(hashtable_unittests
        tests/main.cpp
        tests/hashtable_unittest.cpp
        src/hash_table.h)

set_target_properties(hashtable_unittests PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

target_include_directories(hashtable_unittests PRIVATE
        "${PROJECT_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/src"
        "${PROJECT_SOURCE_DIR}/tests/gtest/googletest"
        "${PROJECT_SOURCE_DIR}/tests/gtest/googletest/include")

add_dependencies(hashtable hashtablelib)

target_link_libraries(hashtable_unittests hashtablelib)

find_package(Threads REQUIRED)
target_link_libraries(hashtable_unittests ${CMAKE_THREAD_LIBS_INIT})
