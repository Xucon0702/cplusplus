cmake_minimum_required(VERSION  3.14)
project(codeCheck)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 该命令会生成
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find_program(CLANG_TIDY_BIN NAMES "clang-tidy")
# if(CLANG_TIDY_BIN)
#   set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_BIN}" "-checks=*")
# endif()

# set(CMAKE_CXX_CLANG_TIDY
#         clang-tidy;
#         -header-filter=.*;
#         -checks=*;
#         --dump-config > .clang-tidy;
#     )


include_directories(
     ./include
)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})





