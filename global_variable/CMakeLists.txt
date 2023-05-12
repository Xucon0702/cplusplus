cmake_minimum_required(VERSION 3.10)

project(global_variable_test)

include_directories(
    ${PROJECT_SOURCE_DIR}/include

)


file(GLOB SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
        
  )

