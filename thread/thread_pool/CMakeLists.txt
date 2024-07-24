Cmake_minimum_required(VERSION 3.5)
project(thread_priority)

include_directories(
    .
    ./include
    )

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

message("SRC_FILES:" ${SRC_FILES})

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME}
    pthread
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/install/bin)
