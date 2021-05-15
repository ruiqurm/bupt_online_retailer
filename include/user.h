#include<string>
#include<iostream>
#include<memory>
#include<regex>
#include<map>
#include"database.h"
using std::string;
using std::ostream;
using std::shared_ptr;
enum USER_TYPE{
    customer=0,
    seller=1
};
// ostream& operator<<(ostream&os,USER_TYPE u){
//     switch (u)
//     {
//     case USER_TYPE::customer:
//         os<<"customer";
//         break;
//     case USER_TYPE::seller:
//         os<<"seller";
//     default:
//         break;
//     }
//     return os;
// }

class User{
    public:
        // friend std::shared_ptr<User> UserManager::login(string username,string password);//允许login调用构造函数
        virtual ~User(){}
        virtual int get_user_type()const=0;
        string get_user_name()const{
            return data->username;
        }
        int get_balance()const{
            return data->balance;
        }
        int get_pk()const{
            return data->pk;
        }
        bool operator==(const User& u)const{
            return data==u.data;
        }
    protected:
        User(UserData* p):data(p){}
        unsigned short _TYPE;
    private:
        UserData* data;
        // ConsumingRecordsManager* records;
        
};

template<int TYPE, typename IMPL>
class UserTemplate: public User{
    // 用于向Manager注册类
    // 参考：https://stackoverflow.com/questions/2850213/dynamically-register-constructor-methods-in-an-abstractfactory-at-compile-time-u
   
   enum {_USER_TYPE_ID = TYPE };
   public:
        static User* instance(UserData * data) { return new IMPL(data); }
        static const unsigned short USER_TYPE_ID; 
        // static void Enable() { volatile uint16_t x =  }
   protected:
      UserTemplate(UserData* p):User(p) { _TYPE = USER_TYPE_ID; } 
};
class UserManager final{
    //管理所有在线的用户
    public:
        typedef User* (*p_user_construct)(UserData *);

        /*
        单例模式
        */
        static UserManager& getInstance(){
            static UserManager manager;
            return manager;
        }

        // void logout(){
        // }
        bool register_(USER_TYPE type,string username,string password){
            auto& record = UserRecord::get_record();
            if(!validate_username(username)){
                throw "username syntax error";
            }
            if(!validate_password(password)){
                throw "password syntax error";
            }
            if (record.exist(username)){
                throw "username exists";
            }
            return record.set(UserData(-1,username.c_str(),password.c_str(),0,static_cast<int>(type)));//写入数据库
        }
        shared_ptr<User> login(string username,string password){
            // auto it =users.find(username);
            // if (it!=users.end()){
            //     //已经登录过了
            //     return create_user(it->second.type,&it->second);
            // }
            auto&record = UserRecord::get_record();
            UserData* data = record.update(username);
            if(!data){
                throw "no exist";
                // return ;
            }
            if(strcmp(data->password,password.c_str())){
                throw "password error";
                // return nullptr;
            }
            return create_user(data->type,data);
        }
        unsigned short register_type(unsigned short id, p_user_construct factoryMethod){
            printf("Registering constructor for user id %d\n", id);
            register_types[id] = factoryMethod;
            return id;
        }
    protected:
        UserManager(){}
        shared_ptr<User> create_user(uint16_t msgid,UserData* data){
            return shared_ptr<User>(register_types[msgid](data));
        }
    private:
        ~UserManager(){}
        UserManager(const UserManager&){};
        UserManager& operator=(const UserManager&);

        static const std::regex USERNAME_PATTERN;
        static const std::regex PASSWORD_PATTERN;
        bool validate_username(const string&username){
            return std::regex_match(username,USERNAME_PATTERN);
        }
        bool validate_password(const string&password){
            return std::regex_match(password,PASSWORD_PATTERN);
        }
        p_user_construct register_types[256];
        // std::map<string,UserData> users;
};


template <int TYPE, typename IMPL>
const unsigned short UserTemplate<TYPE, IMPL>::USER_TYPE_ID = UserManager::getInstance().register_type(
    UserTemplate<TYPE, IMPL >::_USER_TYPE_ID, &UserTemplate<TYPE, IMPL >::instance);

class Seller:public UserTemplate<seller,Seller>{
    public:
        Seller(UserData* p):UserTemplate(p){}
        int get_user_type()const override{
            return _TYPE;
        }
        ~Seller(){}
};

class Customer:public UserTemplate<customer,Customer>{
    public:
        Customer(UserData* p):UserTemplate(p){}
        int get_user_type()const override{
            return _TYPE;
        }
        ~Customer(){}
};



