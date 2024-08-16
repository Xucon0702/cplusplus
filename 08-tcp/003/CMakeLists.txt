Cmake_minimum_required(VERSION 3.5)

project(tcp_service)

add_definitions("-Wall -g")

set(SERVER_PRJ_NAME tcp_service)
set(CLIENT_PRJ_NAME tcp_client)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB CLIENT_SRC_FILES ${PROJECT_SOURCE_DIR}/test/client/*.cpp)
file(GLOB SERVER_SRC_FILES ${PROJECT_SOURCE_DIR}/test/server/*.cpp)

file(GLOB SRC_COMMON_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# message("SRC_FILES:" ${SRC_FILES})

add_executable(${SERVER_PRJ_NAME} ${SRC_COMMON_FILES} ${SERVER_SRC_FILES})
add_executable(${CLIENT_PRJ_NAME} ${SRC_COMMON_FILES} ${CLIENT_SRC_FILES})

target_link_libraries(${SERVER_PRJ_NAME} 

)

target_link_libraries(${CLIENT_PRJ_NAME} 

)