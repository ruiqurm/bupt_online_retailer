set(CMAKE_CXX_STANDARD 17) # 使用c++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
  ./include/
  ../common/
)
# find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
add_subdirectory(src)
add_subdirectory(test)
