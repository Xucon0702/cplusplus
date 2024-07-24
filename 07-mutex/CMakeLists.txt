Cmake_minimum_required(VERSION 3.5)

project(mutex_test)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)


message("SRC_FILES:" ${SRC_FILES})

add_executable(mutex_test ${SRC_FILES})