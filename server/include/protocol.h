#pragma once
#include<string>
#include<vector>
using std::vector;
using std::string;

/**
 * @brief 报文格式
 * 
 */




struct Protocol{
    enum{
        /*前64个分配给Goods*/
        USER_LOGIN,               /*读附加数据*/
        USER_AUTHENTICATE_PASSWORD, /*登录后检验密码*/
        USER_LOGOUT,              /*读附加数据*/
        USER_UPDATE,              /*读附加数据*/
        USER_REGISTER,

        /*前64-128分配给User*/
        GOOOS_GET_BY_ID,
        GOODS_GET_BY_ID_MULITIPLE,
        GOODS_GET_BY_SELLER,
        GOODS_GET_ALL,
        GOODS_GET_BY_NAME,
        GOODS_UPDATE,              /*读附加数据*/
        GOODS_REMOVE,
        GOODS_CREATE,              /*读附加数据*/

        /*128-192分配给Discount*/
    };
    enum{
        OK,
        AUTHENTICATE_FAILED,
        PARAMETER_ERROR,
        RUN_FAILED,
        PERMISSION_DENIED,
    };
    uint16_t transaction_id;//服务id
    uint16_t type;//服务类型 
    uint16_t status;//状态码
    uint8_t is_response;//请求或者响应
    uint8_t ___;//未用
    uint32_t length;//数据长度
    char token[16];//令牌,请求报文给出令牌，回复报文没有令牌。
    //后面是proto数据或者普通int数据
};
