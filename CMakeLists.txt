cmake_minimum_required(VERSION 2.8)
project(online_ratailers)
SET(CMAKE_C_COMPILER /usr/bin/clang)
SET(CMAKE_CXX_COMPILER /usr/bin/clang++)

find_package (SQLite3)
if (SQLITE3_FOUND)
  include_directories(${SQLITE3_INCLUDE_DIRS})
endif (SQLITE3_FOUND)
#Generate the shared library from the library sources
# add_library(online_ratailers SHARED 
    # src/Hello.cpp
# )

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/external
)

############################################################
# Create an executable
############################################################

# Add an executable with the above sources

add_subdirectory(src)


enable_testing()
add_subdirectory(test)
