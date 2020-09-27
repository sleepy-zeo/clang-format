CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
project(main)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -Wall -O0 -Wno-unused-variable -pthread")

add_subdirectory(string)
add_subdirectory(string/example)

add_executable(${PROJECT_NAME} main.cpp)
