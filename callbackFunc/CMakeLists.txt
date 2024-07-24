cmake_minimum_required(VERSION 3.10)

project(callback_test)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CLANG_TIDY_COMMAND clang-tidy;
            # -header-filter=^${PROJECT_ROOT_DIR}/src/.*;
            -config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy;)
set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_COMMAND})

set(CMAKE_BUILD_TYPE Release)

message("111111:${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy")

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

file(GLOB LIB_FILES
    ${PROJECT_SOURCE_DIR}/lib/*.cpp
)

add_library(alg_test STATIC ${LIB_FILES})


add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
        alg_test
  )

set_target_properties(${PROJECT_NAME} PROPERTIES
CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")