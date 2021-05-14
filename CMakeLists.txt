cmake_minimum_required(VERSION 2.8)
project(online_ratailers)
SET(CMAKE_C_COMPILER /usr/bin/clang)
SET(CMAKE_CXX_COMPILER /usr/bin/clang++)


#Generate the shared library from the library sources
# add_library(online_ratailers SHARED 
    # src/Hello.cpp
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
