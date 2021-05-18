#pragma once
#include<string>
#include<iostream>
#include<memory>
#include<regex>
#include<map>
#include <filesystem>
#include"database.h"


using std::string;
using std::ostream;
using std::shared_ptr;
enum USER_TYPE{
    customer=0,
    seller=1
};
class UserData{
    public:
        char username[32];
        char password[16];
        int id;
        int balance;
        int type;
        UserData(){}
        UserData(int id,const char*username,
                const char*password,int balance,
                int type):id(id),balance(balance),type(type){
                    strcpy(this->username,username);
                    strcpy(this->password,password);
        }
};
class User{
    public:
        virtual ~User(){}
        virtual int get_user_type()const=0;
        string get_user_name()const{
            return data->username;
        }
        int get_balance()const{
            return data->balance;
        }
        int get_pk()const{
            return data->id;
        }
        bool operator==(const User& u)const{
            return data==u.data;
        }
    protected:
        User(UserData* p):data(p){}
        UserData* data;
        unsigned short _TYPE;
        // ConsumingRecordsManager* records;
        
};

template<int TYPE, typename IMPL>
class UserTemplate: public User{
    // 用于向Manager注册类
    // CRTP
    // 参考：https://stackoverflow.com/questions/2850213/dynamically-register-constructor-methods-in-an-abstractfactory-at-compile-time-u
   
   enum {_USER_TYPE_ID = TYPE };
   public:
        static User* instance(UserData * data) { return new IMPL(data); }
        static const unsigned short USER_TYPE_ID; 
        // static void Enable() { volatile uint16_t x =  }
   protected:
      UserTemplate(UserData* p):User(p) { _TYPE = USER_TYPE_ID; } 
};

#ifdef MY_DEBUG
class test_usermanager;
#endif
class UserManager final{
    //管理所有在线的用户
    #ifdef MY_DEBUG
        friend class test_usermanager;
    #endif
    public:
        typedef User* (*p_user_construct)(UserData *);

        static UserManager& getInstance(){
            static UserManager manager;//单例模式
            return manager;
        }
        
        bool register_(USER_TYPE type,const string& username,const string& password);
        shared_ptr<User> login(string username,string password);
        void remove(int id);
        void remove(const std::string& username);
        UserData* get_userdata(const string&username);
        unsigned short register_type(unsigned short id, p_user_construct factoryMethod){
            printf("Registering constructor for user id %d\n", id);
            register_types[id] = factoryMethod;
            return id;
        }
        int load();
        static constexpr const char* USER_FILE_NAME="user-record.txt";
    
    private://用于后续的测试
        UserManager();
        shared_ptr<User> create_user(uint16_t msgid,UserData* data){
            return shared_ptr<User>(register_types[msgid](data));
        }
        bool validate_username(const string&username){
            return std::regex_match(username,USERNAME_PATTERN);
        }
        bool validate_password(const string&password){
            return std::regex_match(password,PASSWORD_PATTERN);
        }
        UserData* get(const std::string& username);

        ~UserManager(){}
        UserManager(const UserManager&){};
        UserManager& operator=(const UserManager&);

        static const std::regex USERNAME_PATTERN;
        static const std::regex PASSWORD_PATTERN;

        p_user_construct register_types[256];

        /*
        * 内存缓冲
        */
        std::map<std::string,UserData> name_to_data;
        std::map<int,std::string> pk_to_name;
        
        int max_pk;

        /*
        * 文件操作
        */
        std::fstream database;
        static constexpr int MAX_LINE = sizeof(UserData) + 10;
        void insert_data(const UserData&data);
        void remove_data(int id);
        void set_write_cursor_to_nth_line(int id){
            database.seekp((id-1)*(MAX_LINE));
        }
        void set_read_cursor_to_nth_line(int  id){
            database.seekg((id-1)*(MAX_LINE));
        }
        void write_LF_nth_line(int id){
            database.seekp(id*MAX_LINE -1);
            database<<'\n';
        }
};


template <int TYPE, typename IMPL>
const unsigned short UserTemplate<TYPE, IMPL>::USER_TYPE_ID = UserManager::getInstance().register_type(
    UserTemplate<TYPE, IMPL >::_USER_TYPE_ID, &UserTemplate<TYPE, IMPL >::instance);



/*
* 内联实现
*/
inline UserManager::UserManager(){
    
    if (std::filesystem::exists(USER_FILE_NAME)){
        load();
        database.open(USER_FILE_NAME,std::ios::in|std::ios::out);
        //不会覆盖原来的内容
    }else{
        database.open(USER_FILE_NAME,std::ios::out);
    }
    if (!database.is_open()){
        throw "can't open user record file.";
    }
}
inline void UserManager::insert_data(const UserData&data){
    if(data.id<=0)return;
    set_write_cursor_to_nth_line(data.id);
    database<<data.id<<" "<<data.username<<" "<<data.password<<" "<<data.balance<<" "<<data.type<<"\0\0\0";
    write_LF_nth_line(data.id);
}
inline void UserManager::remove_data(int id){
    if(id<=0)return;
    //不判断是否存在了.
    set_write_cursor_to_nth_line(id);
    database<<"0\0";database.flush();
    write_LF_nth_line(id);
}
inline void UserManager::remove(int id){
    auto it = pk_to_name.find(id);
    if(it==pk_to_name.end())return;
    name_to_data.erase(it->second);
    pk_to_name.erase(id);
    remove_data(id);
}
inline void UserManager::remove(const std::string& username){
    auto it = name_to_data.find(username);
    if(it==name_to_data.end())return;
    int id = it->second.id;
    pk_to_name.erase(it->second.id);
    name_to_data.erase(it);
    remove_data(id);
}
inline UserData*  UserManager::get_userdata(const string&username){
    auto it = name_to_data.find(username);
    if(it!=name_to_data.end()){
        return &it->second;
    }
    return nullptr;
}

inline shared_ptr<User> UserManager::login(string username,string password){
    UserData* data = get_userdata(username);
    if(!data){
        throw "no exist";
    }
    if(strcmp(data->password,password.c_str())){
        throw "password error";
    }
    return create_user(data->type,data);
}