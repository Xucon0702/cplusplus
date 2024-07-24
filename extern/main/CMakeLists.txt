cmake_minimum_required(VERSION 3.22)

project(externTest)

include_directories(
    ./
    ./../libsrc    
)

link_directories(./../libsrc/build)

file(GLOB SRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)


add_executable(${PROJECT_NAME} ${SRC_FILE})

target_link_libraries(${PROJECT_NAME}
    testLib
)

