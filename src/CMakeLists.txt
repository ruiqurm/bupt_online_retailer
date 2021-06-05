# SET(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_FLAGS,"-Wall")
# aux_source_directory(. DIR_SRCS)

add_library(sqlite3 STATIC sqlite3.c)
target_link_libraries(sqlite3 pthread dl)
add_library(database STATIC database.cpp)


add_library(users STATIC user.cpp concreteUser.cpp)


add_library(goods STATIC transaction.cpp goods.cpp user.cpp database.cpp concreteUser.cpp )

add_library(bupt_online_ratailer_static STATIC concreteUser.cpp concreteGoods.cpp transaction.cpp goods.cpp user.cpp database.cpp sqlite3.c)
target_link_libraries(bupt_online_ratailer_static pthread dl)
# TARGET_LINK_LIBRARIES(bupt_online_ratailer_static users goods)
# add_library(concreteImpl STATIC concreteUser.cpp concreteGoods.cpp)
# TARGET_LINK_LIBRARIES(concreteImpl bupt_online_ratailer_static sqlite3)
# add_library(bupt_online_ratailer SHARED goods.cpp user.cpp database.cpp concreteUser.cpp ${PROJECT_SOURCE_DIR}/include/concreteGoods.h)
# target_link_libraries(bupt_online_ratailer sqlite3)
