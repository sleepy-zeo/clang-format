CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
project(sm)

aux_source_directory(. SRCS)
add_library(${PROJECT_NAME} SHARED ${SRCS})
target_include_directories(${PROJECT_NAME} INTERFACE .)
