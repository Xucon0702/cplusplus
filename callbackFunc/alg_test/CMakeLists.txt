add_definitions(-DWINDOWS_DEBUG)

aux_source_directory(./ SRC)							#aux_source_directory是收集指定目录中所有源文件的名称，并将列表存储在提供的变量中。

#mingw编译添加下面的options的话需要额外安装东西,否则可能报错,用cmake . -G "MinGW Makefiles"  --> make是正常的
# add_compile_options(
#       "-Werror=return-type"
# )

SET(INCLUDE_DIR . 
                )

MESSAGE(STATUS "INCLUDE_DIR: ${INCLUDE_DIR}")

include_directories(${INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    # ${CMAKE_CURRENT_SOURCE_DIR}/../

)

message("src:" ${SRC})

# 同时生成静态库和动态库;注意动态库要放在后面
ADD_LIBRARY(alg_test ${SRC})  #STATIC
# set_target_properties(helloWorld_static PROPERTIES OUTPUT_NAME "helloWorld")

# add_library(helloWorld SHARED ${SRC})  #SHARED

# 编译:msvc
# cmake -Bbuild
# cmake --build build