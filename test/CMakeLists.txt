set(CMAKE_CXX_FLAGS,"-Wall")
add_custom_command(OUTPUT clean_test
                    COMMAND file(remove data.db)
                    COMMAND file(remove user-record.txt)
                   )
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
    database
    sqlite3)
UNIT_TEST_MULT_LIB(test_user test_user_lib)

set(test_goodsrecord_lib
    goods
    sqlite3)
UNIT_TEST_MULT_LIB(test_goodsrecord  test_goodsrecord_lib)
UNIT_TEST_MULT_LIB(test_transaction  test_goodsrecord_lib)

set(all_lib
    bupt_online_ratailer_static
    sqlite3)

UNIT_TEST_MULT_LIB(test_buying all_lib)


UNIT_TEST_MULT_LIB(test_discount all_lib)