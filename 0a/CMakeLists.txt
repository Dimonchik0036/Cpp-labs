cmake_minimum_required(VERSION 3.8)
project(0a)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES main.cpp module1.cpp module2.cpp module3.cpp)
set(HEADER_FILES module1.h module2.h module3.h)
add_executable(0 ${SOURCE_FILES} ${HEADER_FILES})