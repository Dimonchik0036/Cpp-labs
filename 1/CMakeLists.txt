cmake_minimum_required(VERSION 3.8)
project(1)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES main.cpp tritset.cpp)
set(HEADER_FILES tritset.h)
add_executable(1 ${SOURCE_FILES} ${HEADER_FILES})