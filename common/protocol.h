#pragma once
#include<string>
#include<vector>
#include"protoData.pb.h"
using std::vector;
using std::string;

/**
 * @brief 报文格式
 * 
 */

//端口
constexpr int PROTOCOL_PORT = 12345;

#pragma pack(1)
struct Protocol{
    enum{
        /*0-63个分配给User*/
        USER_LOGIN,              
        USER_AUTHENTICATE_PASSWORD, /*登录后检验密码*/
        USER_LOGOUT,             
        USER_UPDATE,             
        USER_REGISTER,
        USER_INFO,

        /*前64-127分配给Goods*/
        GOOOS_CREATE=64,
        GOOOS_GET_BY_ID,
        GOODS_GET_BY_ID_MULITIPLE,
        GOODS_GET_BY_SELLER,
        GOODS_GET_ALL,
        GOODS_GET_BY_NAME,
        GOODS_UPDATE,             
        GOODS_REMOVE,

        /*128-191分配给Discount*/
        DISCOUNT_CREATE = 128,
        DISOCUNT_UPDATE,
        DISCOUNT_GET_ALL_CATEGORY,
        DISCOUNT_GET_GOODS_DISCOUNT,
        DISCOUNT_GET_CATEGORY_DISCOUNT,
        DISCOUNT_REMOVE_BY_GOODS,
        DISCOUNT_REMOVE,

        TRANSACTION_GET_BY_USER=192,
        TRANSACTION_GET,
        TRANSACTION_SET,
        TRANSACTION_CANCEL,
        TRANSACTION_SET_FINISHED,

        CART_GET=256,
        CART_SET,
        CART_REMOVE,
        
    };
    enum{
        OK,
        OK_LOGOUT,
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
#pragma pack()

class ProtocolWriter{
    public:
        // ProtocolWriter(char* buffer){};
        ProtocolWriter(char *buffer,int type,const char* token);
        int load(const protoData::Goods&goods);
        int load(const protoData::GoodsArray& array);
        int load(const protoData::UserForm &form);
        int load(const protoData::User& user);

        int load(const protoData::CartItem&item);
        int load(const protoData::Transaction&transcation);
        int load(const protoData::Discount&discount);


        int size()const{return _size + sizeof(Protocol);}//报文总长
        void clear_size(){((Protocol*)buffer)->length = 0;_size=0;}

        int id()const {return ((Protocol*)buffer)->transaction_id ;}
        uint16_t& type()const{return ((Protocol*)buffer)->type;}
        // uint16_t& type(){return ((Protocol*)buffer)->type;}
        int status(){return ((Protocol*)buffer)->status;}
        bool is_response(){return ((Protocol*)buffer)->is_response;}
        int length(){return ((Protocol*)buffer)->length;}
        const char* buf(){return buffer;}
    private:

        void add_length(int length){
            _size = ((Protocol*)buffer)->length =  _size + length;
        }
        // //设置数据长度
        // void set_length(int length){
        //     if(length>0){
        //         ((Protocol*)buffer)->length = length;
        //     }
        // }
        char*buffer;
        int _size;
};
class ProtocolReader{
    public:
        // ProtocolWriter(char* buffer){};
        ProtocolReader(char *_buffer):buffer(_buffer){}
        ProtocolReader(char *_buffer,int size):buffer(_buffer),_buf_size(8192){}
        int id()const {return ((Protocol*)buffer)->transaction_id ;}
        int type(){return ((Protocol*)buffer)->type;}
        int status(){return ((Protocol*)buffer)->status;}
        bool is_response(){return ((Protocol*)buffer)->is_response;}
        int length(){return ((Protocol*)buffer)->length;}

        bool is_good(){return (((Protocol*)buffer)->status == Protocol::OK) ||  (((Protocol*)buffer)->status ==Protocol::OK_LOGOUT);}
        void log_header();
        void log_data(const protoData::Goods&goods);
        void log_data(protoData::GoodsArray& array);
        void log_data(protoData::Transaction&transcation);
        void log_data(protoData::TransactionArray&array);
        void log_data(protoData::Discount&discount);
        void log_data(protoData::DiscountArray&array);
        void log_data(protoData::Cart&cart);
        void log_data(protoData::CartItem&item);
        void log_data(protoData::UserForm &form);
        void log_data(protoData::User& user);
        void get(protoData::Goods&goods);
        void get(protoData::GoodsArray& array);
        void get(protoData::UserForm &form);
        void get(protoData::User& user);

        void get(protoData::CartItem&item);
        void get(protoData::Cart&cart);
        void get(protoData::Transaction&transcation);
        void get(protoData::TransactionArray&array);
        void get(protoData::Discount&discount);
        void get(protoData::DiscountArray&array);

        const char* token(){return ((Protocol*)buffer)->token;}
        char* buf(){return buffer;}
        int buf_size(){return 8192;}
    private:
        char*buffer;
        int _buf_size;
};
/*
 * 辅助函数
 */


inline
const char* protocol_status_to_str(int type){
    switch (type)
    {
    case Protocol::OK:
        // return "OK";
        return "正常";
    case Protocol::OK_LOGOUT:
        return "正常,已经退出登录";
    case Protocol::AUTHENTICATE_FAILED:
        return "AUTHENTICATE_FAILED";
    case Protocol::PARAMETER_ERROR:
        return "PARAMETER_ERROR";
    case Protocol::RUN_FAILED:
        return "RUN_FAILED";
    case Protocol::PERMISSION_DENIED:
        return "PERMISSION_DENIED";
    // case Protocol::USER_LOGIN:
    //     return ;
    // case Protocol::USER_LOGIN:
    //     return ;
    // case Protocol::USER_LOGIN:
    //     return ;                                                    
    default:
        return "Unknow";
    }
}
inline
const char* protocol_type_to_str(int type){
    switch (type)
    {
    /*user*/
    case Protocol::USER_LOGIN:
        return "USER_LOGIN";
    case Protocol::USER_AUTHENTICATE_PASSWORD:
        return "USER_AUTHENTICATE_PASSWORD";
    case Protocol::USER_LOGOUT:
        return "USER_LOGOUT";
    case Protocol::USER_UPDATE:
        return "USER_UPDATE";
    case Protocol::USER_REGISTER:
        return "USER_REGISTER";
    case Protocol::USER_INFO:
        return "USER_INFO";

    /*goods*/
    case Protocol::GOOOS_CREATE:
        return "GOOOS_CREATE";
    case Protocol::GOOOS_GET_BY_ID:
        return "GOOOS_GET_BY_ID";
    case Protocol::GOODS_GET_BY_ID_MULITIPLE:
        return "GOODS_GET_BY_ID_MULITIPLE";
    case Protocol::GOODS_GET_BY_SELLER:
        return "GOODS_GET_BY_SELLER";
    case Protocol::GOODS_GET_ALL:
        return "GOODS_GET_ALL";
    case Protocol::GOODS_GET_BY_NAME:
        return "GOODS_GET_BY_NAME";

       
    case Protocol::DISCOUNT_CREATE:
        return "DISCOUNT_CREATE";                                        
    case Protocol::DISOCUNT_UPDATE:
        return "DISOCUNT_UPDATE";       
    case Protocol::DISCOUNT_GET_ALL_CATEGORY:
        return "DISCOUNT_GET_ALL_CATEGORY";  
    case Protocol::DISCOUNT_GET_GOODS_DISCOUNT:
        return "DISCOUNT_GET_GOODS_DISCOUNT";  
    case Protocol::DISCOUNT_GET_CATEGORY_DISCOUNT:
        return "DISCOUNT_GET_CATEGORY_DISCOUNT";  
    case Protocol::DISCOUNT_REMOVE_BY_GOODS:
        return "DISCOUNT_REMOVE_BY_GOODS";  
    case Protocol::DISCOUNT_REMOVE:
        return "DISCOUNT_REMOVE";  


    case Protocol::TRANSACTION_GET_BY_USER:
        return "TRANSACTION_GET_BY_USER";  
    case Protocol::TRANSACTION_GET:
        return "TRANSACTION_GET";  
    case Protocol::TRANSACTION_SET:
        return "TRANSACTION_SET";                                        
    case Protocol::TRANSACTION_CANCEL:
        return "TRANSACTION_CANCEL";       
    case Protocol::TRANSACTION_SET_FINISHED:
        return "TRANSACTION_SET_FINISHED";  

    case Protocol::CART_GET:
        return "CART_GET";  
    case Protocol::CART_SET:
        return "CART_SET";  
    case Protocol::CART_REMOVE:
        return "CART_REMOVE";  

    default:
        return "Unknow";
    }
}