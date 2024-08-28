cmake_minimum_required(VERSION 3.10)
project(time_test)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})