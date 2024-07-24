cmake_minimum_required(VERSION 3.22)

project(testLib)

include_directories(
    ./
    ./../main
)

    
file(GLOB_RECURSE SRC_FILE ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

message("CMAKE_CURRENT_LIST_DIR:" ${CMAKE_CURRENT_LIST_DIR})
message("SRC_FILE:"${SRC_FILE})

# add_executable(${peoject_name} ${SRC_FILE})

message("project_name:" ${project_name})

add_library(${PROJECT_NAME} SHARED ${SRC_FILE})
