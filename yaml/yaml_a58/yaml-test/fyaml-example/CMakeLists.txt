cmake_minimum_required(VERSION 3.10)
message("${PROJECT_SOURCE_DIR}")
message("${CMAKE_SOURCE_DIR}/fyaml")
project(fyaml-example)

set(PROJECT_SOURCE_DIR ./src)

include_directories(
    ${CMAKE_SOURCE_DIR}/fyaml
    ${CMAKE_SOURCE_DIR}/fyaml-cpp
    ${CMAKE_SOURCE_DIR}/src
    )

    

link_directories(
    ${CMAKE_SOURCE_DIR}/lib
    )

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR} *.c *.cpp)
# file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR} *.c *.cpp) 
# file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.c
# ${CMAKE_SOURCE_DIR}/src/*.cpp


#list(FILTER SRC_FILES EXCLUDE REGEX "MvTestcasePcDebug.cpp")

message(${SRC_FILES})

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}
    fyaml
    )

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/install/bin)


