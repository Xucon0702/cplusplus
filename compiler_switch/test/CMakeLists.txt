cmake_minimum_required(VERSION 3.10)
project(test)

# add_definitions(-DSIMU=${SIMULATION_SWITCH})

if(SIMULATION_SWITCH)
    message("simulation on")
    set(a "ON")
    add_definitions(-DSIMU=\"${a}\")
else()
    message("simulation off")
    set(a "OFF")
    # add_definitions(-DSIMU=\"${a}\")
endif()

if(LOG_TYPE STREQUAL "GAC")
    message("log type is GAC")
elseif(LOG_TYPE STREQUAL "SPDLG")
    message("log type is SPDLG")
else()
    message("log type is others")
endif()

message("a:" ${a})

include_directories(
    ${CMAKE_SOURCE_DIR}

)

file(GLOB  SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

