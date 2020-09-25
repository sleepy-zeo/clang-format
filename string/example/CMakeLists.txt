cmake_minimum_required(VERSION 3.8)
project(test_string)

aux_source_directory(. srcs)

add_executable(${PROJECT_NAME} ${srcs})
target_link_libraries(${PROJECT_NAME} PRIVATE sz_string)