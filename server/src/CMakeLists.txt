set(CMAKE_CXX_FLAGS,"-Wall")
add_library(sqlite3 STATIC sqlite3.c)
target_link_libraries(sqlite3 pthread dl)

add_library(protocol protocol.cpp log.c protoData.pb.cc)
target_link_libraries(protocol protobuf sqlite3 pthread)

add_executable(server main.cpp server.cpp protocol.cpp database.cpp log.c protoData.pb.cc)
target_link_libraries(server protobuf sqlite3 pthread)