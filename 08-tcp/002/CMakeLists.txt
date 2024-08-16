Cmake_minimum_required(VERSION 3.5)

project(tcp_service)

find_package(Boost REQUIRED COMPONENTS system)


include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)


message("SRC_FILES:" ${SRC_FILES})

add_executable(tcp_service ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} 
    Boost::system
    boost_thread
)