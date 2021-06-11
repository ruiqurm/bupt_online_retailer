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

class Executor{
    //执行器
    public:
        enum{
            PERMISSION_ANY,
            PERMISSION_USER
        };
        Executor(char* buffer,char*write_buf,protoData::User* user=nullptr,int permission = PERMISSION_USER);
        void exec();
        virtual void execImp()=0;
        int size(){return _size + sizeof(Protocol);}
        bool is_finished(){return _is_finished;}
        bool is_error(){return _is_error;}
        protoData::User* get_user(){return user;}

        void set_error(int);
        void set_length(int);
        void set_status(int);
    protected:
        bool _is_error;
        bool _is_finished;
        
        int _size;
        char* read_buf;
        char* write_buf;
        char* read_data;
        char* write_data;
        protoData::User* user;
};

/*
命名规范:
lambda x:print(x.lower().replace("_"," ").title().replace(" ","")+"Executor")
可以用template来实现下面一堆函数
*/

class UserLoginExecutor: public Executor{
    public:
        UserLoginExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1,PERMISSION_ANY){}
        void execImp()override;
};

class UserLogoutExecutor: public Executor{
    public:
        UserLogoutExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

class UserRegisterExecutor: public Executor{
    public:
        UserRegisterExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1,PERMISSION_ANY){}
        void execImp()override;
};

class UserUpdateExecutor:public Executor{
    public:
        UserUpdateExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

class UserAuthenticatePasswordExecutor:public Executor{
    public:
        UserAuthenticatePasswordExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

class UserInfoExecutor:public Executor{
    public:
        UserInfoExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};



class GoodsCreateExecutor:public Executor{
    public:
        GoodsCreateExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;    
};

class GoodsGetByIdExecutor:public Executor{
    public:
        GoodsGetByIdExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
class GoodsGetByIDMultipleExecutor:public Executor{
    public:
        GoodsGetByIDMultipleExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
class GoodsGetBySellerExecutor:public Executor{
    public:
        GoodsGetBySellerExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
class GoodsGetAllExecutor:public Executor{
    public:
        GoodsGetAllExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1,PERMISSION_ANY){}
        void execImp()override;
};
class GoodsGetByNameExecutor:public Executor{
    public:
        GoodsGetByNameExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
class GoodsUpdateExecutor:public Executor{
    public:
        GoodsUpdateExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
class GoodsRemoveExecutor:public Executor{
    public:
        GoodsRemoveExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

/**
 * @brief discount
 * 
 */
class DiscountCreateExecutor:public Executor{
    public:
        DiscountCreateExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
class DisocuntUpdateExecutor:public Executor{
    public:
        DisocuntUpdateExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
class DiscountGetAllCategoryExecutor:public Executor{
    public:
        DiscountGetAllCategoryExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

class DiscountGetGoodsDiscountExecutor:public Executor{
    public:
        DiscountGetGoodsDiscountExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
class DiscountGetCategoryDiscountExecutor:public Executor{
    public:
        DiscountGetCategoryDiscountExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

class DiscountRemoveByGoodsExecutor:public Executor{
    public:
        DiscountRemoveByGoodsExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

class DiscountRemoveExecutor:public Executor{
    public:
        DiscountRemoveExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};


class TransactionGetExecutor:public Executor{
    public:
        TransactionGetExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

class TransactionGetByUserExecutor:public Executor{
    public:
        TransactionGetByUserExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
class TransactionSetExecutor:public Executor{
    public:
        TransactionSetExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

class TransactionCancelExecutor:public Executor{
    public:
        TransactionCancelExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
class TransactionSetFinishedExecutor:public Executor{
    public:
        TransactionSetFinishedExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
class CartGetExecutor:public Executor{
    public:
        CartGetExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
class CartSetExecutor:public Executor{
    public:
        CartSetExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};
class CartRemoveExecutor:public Executor{
    public:
        CartRemoveExecutor(char*a,char*b,protoData::User*user1=nullptr):Executor(a,b,user1){}
        void execImp()override;
};

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
