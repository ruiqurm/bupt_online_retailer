/**
 * @file server.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 服务端执行器
 */
#pragma once
#include<memory>
#include<map>
#include<fstream>
#include<cstdlib>
#include<string>
#include <sys/stat.h> 
#include<unistd.h>
extern "C" {
#include"log.h"
}
#include"protocol.h"
#include"protoData.pb.h"
#include<mutex>

using std::fstream;
using std::string;

/**
 * @brief 对应类型调用的执行器基类
 * 
 */
class Executor{
    public:
        enum{
            /// 运行任何调用
            PERMISSION_ANY,

            /// 登录后调用
            PERMISSION_USER
        };

        Executor(char* buffer,char*write_buf,protoData::User* user=nullptr,int permission = PERMISSION_USER);

        /// 执行增删查改
        void exec();
        
        /// 子类实现的部分
        virtual void execImp()=0;

        /// 报文长度
        int size(){return _size + sizeof(Protocol);}

        /// 执行器是否执行结束
        bool is_finished(){return _is_finished;}

        /// 是否出现错误
        bool is_error(){return _is_error;}

        /// 获取从token中得到的user
        protoData::User* get_user(){return user;}

        /**
         * @brief 标记执行出错
         * 
         * @param error 错误代码
         */
        void set_error(int error);

        /// 设置长度
        void set_length(int);

        /// 设置报文状态
        void set_status(int);

    protected:
        bool _is_error;
        bool _is_finished;
        
        int _size;
        char* read_buf;///> 读缓冲区
        char* write_buf;///> 写缓冲区
        
        char* read_data;///> 读报文的数据段
        char* write_data;///> 写报文的数据段
        protoData::User* user;
};

/*
命名规范:
lambda x:print(x.lower().replace("_"," ").title().replace(" ","")+"Executor")
可以用template来实现下面一堆函数
*/

/**
 * @brief 用户登录执行器
 * 
 */
class UserLoginExecutor: public Executor{
    public:
        UserLoginExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1,PERMISSION_ANY){}///> 不需要登录
        void execImp()override;
};

/**
 * @brief 用户退出执行器
 * 
 */
class UserLogoutExecutor: public Executor{
    public:
        UserLogoutExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 用户注册执行器
 * 
 */
class UserRegisterExecutor: public Executor{
    public:
        UserRegisterExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1,PERMISSION_ANY){}///> 不需要登录
        void execImp()override;
};

/**
 * @brief 用户更新信息执行器
 * 
 */
class UserUpdateExecutor:public Executor{
    public:
        UserUpdateExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 验证密码执行器
 * 
 */
class UserAuthenticatePasswordExecutor:public Executor{
    public:
        UserAuthenticatePasswordExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 用户信息执行器
 * 
 */
class UserInfoExecutor:public Executor{
    public:
        UserInfoExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};


/**
 * @brief 商品创建执行器
 * 
 */
class GoodsCreateExecutor:public Executor{
    public:
        GoodsCreateExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;    
};

/**
 * @brief 获取商品(通过id)执行器
 * 
 */
class GoodsGetByIdExecutor:public Executor{
    public:
        GoodsGetByIdExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 获取商品（通过id列表）执行器
 * 
 */
class GoodsGetByIDMultipleExecutor:public Executor{
    public:
        GoodsGetByIDMultipleExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 获取商品（通过卖家id）执行器
 * 
 */
class GoodsGetBySellerExecutor:public Executor{
    public:
        GoodsGetBySellerExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 获取所有商品执行器
 * 
 */
class GoodsGetAllExecutor:public Executor{
    public:
        GoodsGetAllExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1,PERMISSION_ANY){}
        void execImp()override;
};
/**
 * @brief 查找商品执行器
 * 
 */
class GoodsGetByNameExecutor:public Executor{
    public:
        GoodsGetByNameExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 更新商品执行器
 * 
 */
class GoodsUpdateExecutor:public Executor{
    public:
        GoodsUpdateExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 移除商品执行器
 * 
 */
class GoodsRemoveExecutor:public Executor{
    public:
        GoodsRemoveExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief discount
 * 
 */

/**
 * @brief 创建折扣执行器
 * 
 */
class DiscountCreateExecutor:public Executor{
    public:
        DiscountCreateExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 更新折扣执行器
 * 
 */
class DisocuntUpdateExecutor:public Executor{
    public:
        DisocuntUpdateExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 获取所有品类折扣执行器
 * 
 */
class DiscountGetAllCategoryExecutor:public Executor{
    public:
        DiscountGetAllCategoryExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 获取商品折扣执行器
 * 
 */
class DiscountGetGoodsDiscountExecutor:public Executor{
    public:
        DiscountGetGoodsDiscountExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 获取品类折扣执行器
 * 
 */
class DiscountGetCategoryDiscountExecutor:public Executor{
    public:
        DiscountGetCategoryDiscountExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 通过商品id移除折扣执行器
 * 
 */
class DiscountRemoveByGoodsExecutor:public Executor{
    public:
        DiscountRemoveByGoodsExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 删除折扣执行器
 * 
 */
class DiscountRemoveExecutor:public Executor{
    public:
        DiscountRemoveExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 获取订单执行器
 * 
 */
class TransactionGetExecutor:public Executor{
    public:
        TransactionGetExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 通过用户id获取订单执行器
 * 
 */
class TransactionGetByUserExecutor:public Executor{
    public:
        TransactionGetByUserExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 创建订单执行器
 * 
 */
class TransactionSetExecutor:public Executor{
    public:
        TransactionSetExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 取消订单执行器
 * 
 */
class TransactionCancelExecutor:public Executor{
    public:
        TransactionCancelExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 设置订单完成执行器
 * 
 */
class TransactionSetFinishedExecutor:public Executor{
    public:
        TransactionSetFinishedExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
/**
 * @brief 获取购物车执行器
 * 
 */
class CartGetExecutor:public Executor{
    public:
        CartGetExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 创建购物车项执行器
 * 
 */
class CartSetExecutor:public Executor{
    public:
        CartSetExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief 移除购物车项执行器
 * 
 */
class CartRemoveExecutor:public Executor{
    public:
        CartRemoveExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};


/**
 * @brief 执行器代理（根据类型选择执行器）
 * @details 使用时，创建一个ExecutorFactory，然后获取一个executor的指针。如果有反射机制，可以使用类似repl的方法。
 * 
 */
class ExecutorFactory{
    //工具类
    public:
        ExecutorFactory()=delete;
        ExecutorFactory(const ExecutorFactory&)=delete;
        static Executor* get_executor(char* read_buf,char* write_buf,protoData::User*user=nullptr,bool should_anthenticate=false){
            int type = ((Protocol*)read_buf)->type;
            log_debug("id =%d,type=%d,length=%d",((Protocol*)read_buf)->transaction_id,type,((Protocol*)read_buf)->length);
            switch (type){
                case Protocol::USER_LOGIN:
                    return (Executor*)new UserLoginExecutor(read_buf,write_buf,user);
                case Protocol::USER_LOGOUT:
                    return (Executor*)new UserLogoutExecutor(read_buf,write_buf,user);
                case Protocol::USER_REGISTER:
                    return (Executor*)new UserRegisterExecutor(read_buf,write_buf,user);
                case Protocol::USER_UPDATE:
                    return (Executor*)new UserUpdateExecutor(read_buf,write_buf,user);
                case Protocol::USER_AUTHENTICATE_PASSWORD:
                    return (Executor*)new UserAuthenticatePasswordExecutor(read_buf,write_buf,user);
                case Protocol::USER_INFO:
                    return (Executor*)new UserInfoExecutor(read_buf,write_buf,user);

                case Protocol::GOOOS_CREATE:
                    return (Executor*)new GoodsCreateExecutor(read_buf,write_buf,user);
                case Protocol::GOOOS_GET_BY_ID:
                    return (Executor*)new GoodsGetByIdExecutor(read_buf,write_buf,user);
                case Protocol::GOODS_GET_BY_ID_MULITIPLE:
                    return (Executor*)new GoodsGetByIDMultipleExecutor(read_buf,write_buf,user);
                case Protocol::GOODS_GET_BY_SELLER:
                    return (Executor*)new GoodsGetBySellerExecutor(read_buf,write_buf,user);
                case Protocol::GOODS_GET_ALL:
                    return (Executor*)new GoodsGetAllExecutor(read_buf,write_buf,user);
                case Protocol::GOODS_GET_BY_NAME:
                    return (Executor*)new GoodsGetByNameExecutor(read_buf,write_buf,user);
                case Protocol::GOODS_UPDATE:
                    return (Executor*)new GoodsUpdateExecutor(read_buf,write_buf,user);                                      
                case Protocol::GOODS_REMOVE:
                    return (Executor*)new GoodsRemoveExecutor(read_buf,write_buf,user);   

                case Protocol::DISCOUNT_CREATE:
                    return (Executor*)new DiscountCreateExecutor(read_buf,write_buf,user); 
                case Protocol::DISOCUNT_UPDATE:
                    return (Executor*)new DisocuntUpdateExecutor(read_buf,write_buf,user);        
                case Protocol::DISCOUNT_GET_ALL_CATEGORY:
                    return (Executor*)new DiscountGetAllCategoryExecutor(read_buf,write_buf,user); 
                case Protocol::DISCOUNT_GET_GOODS_DISCOUNT:
                    return (Executor*)new DiscountGetGoodsDiscountExecutor(read_buf,write_buf,user); 
                case Protocol::DISCOUNT_GET_CATEGORY_DISCOUNT:
                    return (Executor*)new DiscountGetCategoryDiscountExecutor(read_buf,write_buf,user); 
                case Protocol::DISCOUNT_REMOVE_BY_GOODS:
                    return (Executor*)new DiscountRemoveByGoodsExecutor(read_buf,write_buf,user); 
                case Protocol::DISCOUNT_REMOVE:
                    return (Executor*)new DiscountRemoveExecutor(read_buf,write_buf,user); 

                case Protocol::TRANSACTION_GET_BY_USER:
                    return (Executor*)new TransactionGetByUserExecutor(read_buf,write_buf,user); 
                case Protocol::TRANSACTION_GET:
                    return (Executor*)new TransactionGetExecutor(read_buf,write_buf,user); 
                case Protocol::TRANSACTION_SET:
                    return (Executor*)new TransactionSetExecutor(read_buf,write_buf,user);                                      
                case Protocol::TRANSACTION_CANCEL:
                    return (Executor*)new TransactionCancelExecutor(read_buf,write_buf,user);     
                case Protocol::TRANSACTION_SET_FINISHED:
                    return (Executor*)new TransactionSetFinishedExecutor(read_buf,write_buf,user);   
                    
                case Protocol::CART_GET:
                    return (Executor*)new CartGetExecutor(read_buf,write_buf,user);   
                case Protocol::CART_SET:
                    return (Executor*)new CartSetExecutor(read_buf,write_buf,user);  
                case Protocol::CART_REMOVE:
                    return (Executor*)new CartRemoveExecutor(read_buf,write_buf,user);   
                default:
                    return nullptr;
            }
        }

};
