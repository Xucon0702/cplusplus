cmake_minimum_required(VERSION 3.10)

project(signalTest)


include_directories()

file(GLOB SEND_SIGNAL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/send/*.cpp)
file(GLOB RECV_SIGNAL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/recv/*.cpp)
file(GLOB SEND_TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/send_test/*.cpp)

add_executable(send_signal ${SEND_SIGNAL_FILES})
add_executable(recv_signal ${RECV_SIGNAL_FILES})
add_executable(send_test ${SEND_TEST_FILES})