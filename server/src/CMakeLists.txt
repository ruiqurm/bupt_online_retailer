
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS protoData.proto)
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS EXPORT_MACRO DLL_EXPORT protoData.proto)

add_executable(server main.cpp server.cpp log.c protoData.pb.cc)
target_link_libraries(server protobuf pthread)