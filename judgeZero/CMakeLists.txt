Cmake_minimum_required(VERSION 3.5)
project(judgeZero)

include_directories(.)

file(GLOB_RECURSE SRC_FILES ${project_source_dir}/src *.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

