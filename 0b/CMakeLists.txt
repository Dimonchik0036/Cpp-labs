cmake_minimum_required(VERSION 3.8)
project(0b)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES main.cpp strings.cpp)
set(HEADER_FILES strings.h)
add_executable(0 ${SOURCE_FILES} ${HEADER_FILES})