#include<string>
#include<iostream>
#include<memory>
#include<regex>
#include<map>
#include"database.h"
using std::string;
using std::ostream;

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
        

        virtual ~User();
        virtual int getUserType()const;

    protected:
        User(UserData* p):data(p){}
        unsigned short _TYPE;
    private:
        UserData * data;
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

   protected:
      UserTemplate(UserData* p):User(p) { _TYPE = _USER_TYPE_ID; } 
};
class UserManager final{
    //管理所有在线的用户
    public:
        typedef User* (*p_user_construct)(UserData *);
        static UserManager& getInstance(){
            //单例模式
            static UserManager manager;
            return manager;
        }
        void logout(){

        }
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
        User* login(string username,string password){
            auto&record = UserRecord::get_record();
            auto data = record.get(username);
            if(data->pk==-1){
                return nullptr;
            }
            if(data->password != password){
                return nullptr;
            }
            if(users.find(data->username)==users.end()){
                //没有登录过
                users.emplace(data->username,*data);
            }
            return create_user(data->type,&users[data->username]);
        }
        unsigned short register_type(unsigned short id, p_user_construct factoryMethod){
            printf("Registering constructor for msg id %d\n", id);
            register_types[id] = factoryMethod;
            return id;
        }
    protected:
        UserManager(){}
        User *create_user(uint16_t msgid,UserData* data){
            return register_types[msgid](data);
        }
    private:
        static const std::regex USERNAME_PATTERN;
        static const std::regex PASSWORD_PATTERN;
        bool validate_username(const string&username){
            return std::regex_match(username,USERNAME_PATTERN);
        }
        bool validate_password(const string&password){
            return std::regex_match(password,PASSWORD_PATTERN);
        }
        p_user_construct register_types[256];
        std::map<string,UserData> users;
};
template <int TYPE, typename IMPL>
const unsigned short UserTemplate<TYPE, IMPL>::USER_TYPE_ID = UserManager::getInstance().register_type(
    UserTemplate<TYPE, IMPL >::USER_TYPE_ID, &UserTemplate<TYPE, IMPL >::instance);


class Seller:public UserTemplate<seller,Seller>{
    public:
        Seller(UserData* p):UserTemplate(p){}
        int getUserType()const override{
            return _TYPE;
        }
        ~Seller(){}
};

class Customer:public UserTemplate<customer,Customer>{
    public:
        Customer(UserData* p):UserTemplate(p){}
        ~Customer(){}
};


