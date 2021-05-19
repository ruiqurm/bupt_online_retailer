cmake_minimum_required(VERSION 3.1.0)
project(online_ratailers)

if(LINUX)
  SET(CMAKE_C_COMPILER /usr/bin/clang)
  SET(CMAKE_CXX_COMPILER /usr/bin/clang++)
endif()
set(CMAKE_CXX_STANDARD 17) # 使用c++17


find_package (SQLite3)
if (SQLITE3_FOUND)
  include_directories(${SQLITE3_INCLUDE_DIRS})
else()
  #如果没有找到，那么把自己的文件复制进去
  message(STATUS "Can't find sqlite3. Copy compiled files")
  IF(LINUX)
    file(COPY ${PROJECT_SOURCE_DIR}/external/libsqlite3.a DESTINATION ${CMAKE_BINARY_DIR}/src/ FOLLOW_SYMLINK_CHAIN)
    file(COPY ${PROJECT_SOURCE_DIR}/external/libsqlite3.a DESTINATION ${CMAKE_BINARY_DIR}/test/ FOLLOW_SYMLINK_CHAIN)    
MESSAGE(STATUS "current platform: Linux ")
  ELSEIF (WIN32)
    file(COPY ${PROJECT_SOURCE_DIR}/external/sqlite3.lib DESTINATION ${CMAKE_BINARY_DIR}/src/ FOLLOW_SYMLINK_CHAIN)
    file(COPY ${PROJECT_SOURCE_DIR}/external/libsqlite3.a DESTINATION ${CMAKE_BINARY_DIR}/test/ FOLLOW_SYMLINK_CHAIN)
    MESSAGE(STATUS "current platform: Windows")
  ELSE()
    MESSAGE(FATAL_ERROR "Not suitable sqlite3 lib")
  endif()
endif (SQLITE3_FOUND)

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

############################################################
# Create an executable
############################################################

# Add an executable with the above sources

add_subdirectory(src)


enable_testing()
add_subdirectory(test)
