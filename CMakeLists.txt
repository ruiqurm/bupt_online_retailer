cmake_minimum_required(VERSION 3.1.0)
project(online_ratailers)
# set (CMAKE_PREFIX_PATH "G:/qt/5.12.10/mingw73_64")
# if(LINUX)
#   SET(CMAKE_C_COMPILER /usr/bin/clang)
#   SET(CMAKE_CXX_COMPILER /usr/bin/clang++)
# endif()
set(CMAKE_CXX_STANDARD 17) # 使用c++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# link_directories(
#   ${PROJECT_SOURCE_DIR}/external
# )



#Generate the shared library from the library sources
# add_library(online_ratailers SHARED 
    # src/Hello.cpp
# )
# link_directories(
#     ${PROJECT_SOURCE_DIR}/external/
# )
include_directories(
  ${PROJECT_SOURCE_DIR}/include
)


add_subdirectory(src)
# add_subdirectory(ui)

enable_testing()
add_subdirectory(test)



set (dist_gen_name ${CMAKE_BINARY_DIR}/dist)
add_custom_target (generate
    COMMAND make -j12
    COMMAND ${CMAKE_COMMAND} -E  # 创建dist文件夹
        make_directory ${dist_gen_name}
    COMMAND ${CMAKE_COMMAND} -E  # 复制头文件
        copy_directory ${PROJECT_SOURCE_DIR}/include ${dist_gen_name}/include
    COMMAND ${CMAKE_COMMAND} -E  # 创建libs文件夹
        make_directory ${dist_gen_name}/libs
    COMMAND ${CMAKE_COMMAND} -E 
        copy ${CMAKE_BINARY_DIR}/src/libbupt_online_ratailer_static.a ${dist_gen_name}/libs/
    COMMAND echo 在${CMAKE_BINARY_DIR}/dist/下生成
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

add_custom_target(copy_to_ui
    COMMAND ${CMAKE_COMMAND} -E  # 复制头文件
    copy_directory ${dist_gen_name}/include  ${PROJECT_SOURCE_DIR}/ui/include
   COMMAND ${CMAKE_COMMAND} -E  # 复制lib
    copy_directory ${dist_gen_name}/libs  ${PROJECT_SOURCE_DIR}/ui/libs
)
add_custom_target (gen_clean
    COMMAND ${CMAKE_COMMAND} -E
        remove_directory ${dist_gen_name}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
