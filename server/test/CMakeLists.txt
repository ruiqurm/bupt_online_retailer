
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS protoData.proto)
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS EXPORT_MACRO DLL_EXPORT protoData.proto)

add_executable(client_test client.cpp)
target_link_libraries(client_test protocol)