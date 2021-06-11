
set(CMAKE_CXX_FLAGS,"-Wall")
# aux_source_directory(. DIR_SRCS)

# add_library(database STATIC database.cpp protocol.cpp protoData.pb.cc)


# add_library(users STATIC user.cpp concreteUser.cpp)
# target_link_libraries(users protobuf pthread)

# add_library(goods STATIC transaction.cpp goods.cpp user.cpp database.cpp concreteUser.cpp protocol.cpp protoData.pb.cc)
# target_link_libraries(goods protobuf pthread)
add_library(bupt_online_ratailer_static STATIC concreteUser.cpp concreteGoods.cpp transaction.cpp  user.cpp goods.cpp database.cpp)
target_link_libraries(bupt_online_ratailer_static protocol protobuf pthread dl)
