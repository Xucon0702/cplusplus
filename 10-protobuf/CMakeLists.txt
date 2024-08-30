cmake_minimum_required(VERSION 3.10)

SET(PROTO_SERVER server_proto)
SET(PROTO_CLIENT client_proto)
SET(MV_PROTO_LIB mv_proto)

SET(INSTALL_LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/lib)

PROJECT(proto_test)

FIND_PACKAGE(Protobuf REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/protobuf
    ${CMAKE_CURRENT_LIST_DIR}/protobuf/proto_out

    ${CMAKE_CURRENT_LIST_DIR}/src/client
    ${CMAKE_CURRENT_LIST_DIR}/src/server
)

file(GLOB_RECURSE SRC_PROTOBUF_FILES
    ${CMAKE_CURRENT_LIST_DIR}/protobuf/proto_out/*.cc
    # ${CMAKE_CURRENT_LIST_DIR}/protobuf/hmi_3d/*.cc
)

file(GLOB SRC_CLIENT_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/client/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/client/animal_test/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/client/hmi_3d_transfer/*.cpp
)

file(GLOB SRC_SERVER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/server/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/server/animal_test/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/server/hmi_3d_transfer/*.cpp
)

link_directories(
    ${${INSTALL_LIB_DIR}}
)

add_library(${MV_PROTO_LIB} ${SRC_PROTOBUF_FILES})

add_executable(${PROTO_SERVER} ${SRC_SERVER_FILES})
add_executable(${PROTO_CLIENT} ${SRC_CLIENT_FILES})

message("${CMAKE_CURRENT_LIST_DIR}/src/server/")
message("SRC_SERVER_FILES:" ${SRC_SERVER_FILES})

TARGET_LINK_LIBRARIES(${PROTO_SERVER} 
    ${PROTOBUF_LIBRARIES}
    ${MV_PROTO_LIB}
)

TARGET_LINK_LIBRARIES(${PROTO_CLIENT} 
    ${PROTOBUF_LIBRARIES}
    ${MV_PROTO_LIB}
)

message("install proto:${INSTALL_LIB_DIR}")
install(TARGETS ${MV_PROTO_LIB} DESTINATION ${INSTALL_LIB_DIR})
