set(CMAKE_CXX_FLAGS,"-Wall")
add_library(sqlite3 STATIC sqlite3.c)
target_link_libraries(sqlite3 pthread dl)

add_executable(server main.cpp server.cpp database.cpp)
target_link_libraries(server protocol protobuf sqlite3 pthread)