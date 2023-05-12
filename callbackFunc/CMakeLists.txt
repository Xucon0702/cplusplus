cmake_minimum_required(VERSION 3.10)

project(callback_test)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

link_directories(
    ${PROJECT_SOURCE_DIR}/lib
)

file(GLOB SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
        alg_test
  )

