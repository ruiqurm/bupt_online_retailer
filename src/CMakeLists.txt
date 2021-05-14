# SET(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_C_FLAGS,"-werror -std=c++14")
# aux_source_directory(. DIR_SRCS)

add_library(database STATIC database.cpp)
add_library(users STATIC user.cpp)

