add_library(protocol STATIC protocol.cpp log.c protoData.pb.cc)
target_link_libraries(protocol protobuf pthread)