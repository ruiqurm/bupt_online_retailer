set(CMAKE_CXX_FLAGS,"-Wall")

macro(UNIT_TEST filename lib)
   add_executable(        ${filename}  ${filename}.cpp )
   target_link_libraries( ${filename}  ${lib})
   add_test(              ${filename}  ${CMAKE_CURRENT_BINARY_DIR}/${filename} )
endmacro(UNIT_TEST)

macro(UNIT_TEST_MULT_LIB filename lib)
   add_executable(        ${filename}  ${filename}.cpp )
   target_link_libraries( ${filename}  ${${lib}})
   add_test(              ${filename}  ${CMAKE_CURRENT_BINARY_DIR}/${filename} )
endmacro(UNIT_TEST_MULT_LIB)

set(test_user_lib 
    users 
    database)
UNIT_TEST_MULT_LIB(test_user test_user_lib)

set(test_goodsrecord_lib
    goods
    sqlite3)
UNIT_TEST_MULT_LIB(test_goodsrecord  test_goodsrecord_lib)

# UNIT_TEST(aaa sqlite3)

# add_executable(test_goodsrecord  test_goodsrecord.cpp )
# target_link_libraries(test_goodsrecord goods)
# target_link_libraries(test_goodsrecord database)
# add_test(test_goodsrecord  ${CMAKE_CURRENT_BINARY_DIR}/test_goodsrecord)
