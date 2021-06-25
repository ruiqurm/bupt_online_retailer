/**
 * @file protocol.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 协议
 */
#pragma once
#include<string>
#include<vector>
#include"protoData.pb.h"
using std::vector;
using std::string;


/// 端口号
constexpr int PROTOCOL_PORT = 12345;


#pragma pack(1)
/**
 * @brief 协议
 * @details 类似RPC框架，
 * 0              16               32bit
 * ---------------------------------
 * |    id        |     type       |
 * ---------------------------------
 * |   status     |response| 未用  |
 * ---------------------------------
 * |            长度               |
 * ---------------------------------
 * |  token       |       数据     |
 * --------------------------------- 
 * |（不定长，使用protobuf序列化)    |
 * ---------------------------------
 */
struct Protocol{
    enum{
        /*0-63个分配给User*/
        USER_LOGIN,   ///> 登录           
        USER_AUTHENTICATE_PASSWORD, ///> 登录后校验密码
        USER_LOGOUT,   ///> 登出
        USER_UPDATE,   ///> 更新用户数据    
        USER_REGISTER, ///> 注册
        USER_INFO,     ///> 用户信息

        /*前64-127分配给Goods*/
        GOOOS_CREATE=64, ///> 创建商品
        GOOOS_GET_BY_ID, ///> 通过id获取商品
        GOODS_GET_BY_ID_MULITIPLE, ///>通过id获取多个商品
        GOODS_GET_BY_SELLER, ///> 通过卖家id获取商品
        GOODS_GET_ALL,  ///> 获取全部商品
        GOODS_GET_BY_NAME, ///> 通过名字模糊查找获取商品
        GOODS_UPDATE,   ///> 更新商品信息
        GOODS_REMOVE,   ///> 移除商品

        /*128-191分配给Discount*/
        DISCOUNT_CREATE = 128, ///> 创建折扣
        DISOCUNT_UPDATE,   ///> 更新折扣
        DISCOUNT_GET_ALL_CATEGORY, ///> 获取所有品类折扣
        DISCOUNT_GET_GOODS_DISCOUNT, ///> 获取某商品的折扣
        DISCOUNT_GET_CATEGORY_DISCOUNT, ///> 获取某品类折扣
        DISCOUNT_REMOVE_BY_GOODS,///> 通过商品id移除折扣
        DISCOUNT_REMOVE, ///> 移除折扣

        TRANSACTION_GET_BY_USER=192, ///> 通过用户id获取所有交易订单
        TRANSACTION_GET, ///> 通过订单id获取订单
        TRANSACTION_SET, ///> 创建订单
        TRANSACTION_CANCEL,///> 取消订单
        TRANSACTION_SET_FINISHED, ///> 完成订单

        CART_GET=256, ///> 获取购物车
        CART_SET, ///> 新建购物车项
        CART_REMOVE, ///> 移除购物车项
        
    };
    enum{
        OK, ///> 成功
        OK_LOGOUT, ///> 成功，且已退出登录
        AUTHENTICATE_FAILED, ///> 认证失败
        PARAMETER_ERROR, ///> 参数错误
        RUN_FAILED, ///> 运行失败
        PERMISSION_DENIED, ///> 无权限
    };
    uint16_t transaction_id;///> 服务id
    uint16_t type; ///> 服务类型 
    uint16_t status; ///> 状态码
    uint8_t is_response;///> 请求或者响应
    uint8_t ___;///> 未用
    uint32_t length;///> 数据长度
    char token[16];///> 令牌,请求报文给出令牌，回复报文没有令牌。
    //后面是proto数据或者普通int数据
};
#pragma pack()

/**
 * @brief 把协议写入到缓冲区的格式化类
 * 
 */
class ProtocolWriter{
    public:
        // ProtocolWriter(char* buffer){};
        ProtocolWriter(char *buffer,int type,const char* token);
        /**
         * @brief 把goods数据加载到数据部分
         * 
         * @param goods 商品信息
         * @return int 写入了多少字节
         */
        int load(const protoData::Goods&goods);
        
        /**
         * @brief 把goods列表数据加载到数据部分
         * 
         * @param array 商品列表
         * @return int 写入了多少字节
         */
        int load(const protoData::GoodsArray& array);

        /**
         * @brief 把用户账号和密码加载到数据部分
         * 
         * @param form 用户登录表单
         * @return int 写入了多少字节
         */
        int load(const protoData::UserForm &form);

        /**
         * @brief 把用户数据加载到数据部分
         * 
         * @param user 用户数据
         * @return int 写入了多少字节
         */
        int load(const protoData::User& user);

        /**
         * @brief 把购物车数据加载到数据部分
         * 
         * @param item 购物车项
         * @return int 写入了多少字节
         */
        int load(const protoData::CartItem&item);

        /**
         * @brief 把交易订单数据写到数据部分
         * 
         * @param transcation 交易订单
         * @return int 写入了多少字节
         */
        int load(const protoData::Transaction&transcation);

        /**
         * @brief 把折扣数据写到数据部分
         * 
         * @param discount 折扣
         * @return int 写入了多少字节
         */
        int load(const protoData::Discount&discount);

        /// 报文总长，数据部分+头长度
        int size()const{return _size + sizeof(Protocol);}

        /// 清空数据（不去除头）
        void clear_size(){((Protocol*)buffer)->length = 0;_size=0;}

        /// 报文id
        int id()const {return ((Protocol*)buffer)->transaction_id ;}

        /// 报文类型
        uint16_t& type()const{return ((Protocol*)buffer)->type;}
        
        /// 报文状态
        int status(){return ((Protocol*)buffer)->status;}

        /// 报文是否是回复
        bool is_response(){return ((Protocol*)buffer)->is_response;}

        /// 报文长度
        int length(){return ((Protocol*)buffer)->length;}

        /// 报文字符串头地址
        const char* buf(){return buffer;}

    private:
        /// 增加长度
        void add_length(int length){
            _size = ((Protocol*)buffer)->length =  _size + length;
        }

        char*buffer;
        int _size;
};

/**
 * @brief 把协议从缓冲区读出的类
 * 
 */
class ProtocolReader{
    public:
        ProtocolReader(char *_buffer):buffer(_buffer),_buf_size(8192){}// 随意设定的一个数，可以改大一点

        /// 限定缓冲区大小
        ProtocolReader(char *_buffer,int size):buffer(_buffer),_buf_size(8192){}

        /// 报文id
        int id()const {return ((Protocol*)buffer)->transaction_id ;}

        /// 报文类型
        int type(){return ((Protocol*)buffer)->type;}

        /// 报文状态
        int status(){return ((Protocol*)buffer)->status;}

        /// 是否是回复
        bool is_response(){return ((Protocol*)buffer)->is_response;}

        /// 长度
        int length(){return ((Protocol*)buffer)->length;}

        /// 报文状态是否是good
        bool is_good(){return (((Protocol*)buffer)->status == Protocol::OK) ||  (((Protocol*)buffer)->status ==Protocol::OK_LOGOUT);}

        /*下面几个函数用于调试*/
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

        /**
         * @brief 从缓冲区读取数据
         * 
         * @param goods 
         */
        void get(protoData::Goods&goods);

        /**
         * @brief 从缓冲区读取数据
         * 
         * @param goods 
         */        
        void get(protoData::GoodsArray& array);

        /**
         * @brief 从缓冲区读取数据
         * 
         * @param goods 
         */              
        void get(protoData::UserForm &form);

        /**
         * @brief 从缓冲区读取数据
         * 
         * @param goods 
         */              
        void get(protoData::User& user);

        /**
         * @brief 从缓冲区读取数据
         * 
         * @param goods 
         */      
        void get(protoData::CartItem&item);

        /**
         * @brief 从缓冲区读取数据
         * 
         * @param goods 
         */              
        void get(protoData::Cart&cart);

        /**
         * @brief 从缓冲区读取数据
         * 
         * @param goods 
         */              
        void get(protoData::Transaction&transcation);

        /**
         * @brief 从缓冲区读取数据
         * 
         * @param goods 
         */              
        void get(protoData::TransactionArray&array);

        /**
         * @brief 从缓冲区读取数据
         * 
         * @param goods 
         */              
        void get(protoData::Discount&discount);

        /**
         * @brief 从缓冲区读取数据
         * 
         * @param goods 
         */              
        void get(protoData::DiscountArray&array);

        /**
         * @brief token
         * 
         * @return const char* 
         */
        const char* token(){return ((Protocol*)buffer)->token;}

        /**
         * @brief 缓冲区指针
         * 
         */
        char* buf(){return buffer;}

        int buf_size(){return 8192;}
    private:
        char*buffer;
        int _buf_size;
};

/*
 * 辅助函数
 */

/**
 * @brief 把协议状态转成字符串，用于调试
 * 
 * @param type 协议状态
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

/**
 * @brief 把协议类型转成字符串，用于调试
 * 
 * @param type 类型
 */
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