CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
project(main)

add_subdirectory(string)
add_subdirectory(string/example)

add_executable(${PROJECT_NAME} main.cpp)
