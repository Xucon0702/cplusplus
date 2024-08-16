cmake_minimum_required(VERSION 3.10)
project(tcp_server)

set(PRJ_TCP_SERVER tcp_server)
set(PRJ_TCP_CLIETN tcp_client)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_BUILD_TYPE DEBUG)
#set(CMAKE_CXX_FLAGS "-fpermissive")
set(CMAKE_CXX_FLAGS "-O3 -fpermissive")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/client
    ${PROJECT_SOURCE_DIR}/include/server
    ${PROJECT_SOURCE_DIR}/include/data

    ${PROJECT_SOURCE_DIR}/test/server
    
)

file(GLOB SRC_COMMON_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

file(GLOB SRC_SERVER_FILES ${PROJECT_SOURCE_DIR}/src/server/*.cpp)
file(GLOB SRC_SERVER_TEST_FILES ${PROJECT_SOURCE_DIR}/test/server/*.cpp)

file(GLOB SRC_CLIENT_FILES ${PROJECT_SOURCE_DIR}/src/client/*.cpp)
file(GLOB SRC_CLIENT_TEST_FILES ${PROJECT_SOURCE_DIR}/test/client/*.cpp)

message("SRC_COMMON_FILES:" ${SRC_COMMON_FILES})
message("SRC_SERVER_FILES:" ${SRC_SERVER_FILES})
message("SRC_SERVER_TEST_FILES:" ${SRC_SERVER_TEST_FILES})
message("SRC_CLIENT_FILES:" ${SRC_CLIENT_FILES})
message("SRC_CLIENT_TEST_FILES:" ${SRC_CLIENT_TEST_FILES})

link_directories(

)

add_executable(${PRJ_TCP_SERVER}
	${SRC_COMMON_FILES}
    ${SRC_SERVER_FILES}
    ${SRC_SERVER_TEST_FILES}
)

add_executable(${PRJ_TCP_CLIETN}
	${SRC_COMMON_FILES}
    ${SRC_CLIENT_FILES}
    ${SRC_CLIENT_TEST_FILES}
)

target_link_libraries(${PRJ_TCP_SERVER}

    pthread
)

target_link_libraries(${PRJ_TCP_CLIETN}

    pthread
)