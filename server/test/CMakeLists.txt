
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS protoData.proto)
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS EXPORT_MACRO DLL_EXPORT protoData.proto)

add_executable(test_user test_user.cpp)
target_link_libraries(test_user protocol)

add_executable(test_goods test_goods.cpp)
target_link_libraries(test_goods protocol)

add_executable(test_cart test_cart.cpp)
target_link_libraries(test_cart protocol)

add_executable(test_transaction test_transaction.cpp)
target_link_libraries(test_transaction protocol)

add_executable(test_discount test_discount.cpp)
target_link_libraries(test_discount protocol)
