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
        // char username[32];
        // char password[16];
        string username;
        string password;
        int id;
        int balance;
        int type;
        UserData(){}
        UserData(int id,const char*username,
                const char*password,int balance,
                int type):id(id),balance(balance),type(type),username(username),password(password){}
        UserData(int id,const string& username,
        const string& password,int balance,
        int type):id(id),balance(balance),type(type),username(username),password(password){}
};
class User{
    public:
        virtual ~User(){}
        virtual int get_user_type()const=0;
        
        //登录与注册
        static std::unique_ptr<User> login(const string& username,const string& password);
        static bool register_(USER_TYPE type,const string& username,const string& password);
        
        // bool change_password(const string&password){
        //     if(!validate_password)"throw password syntax error";
        //     auto& record = UserRecord::get_record();
        //     record.
        // }
        const string& username()const{return data->username;}
        int balance()const{return data->balance;}
        int id()const{return data->id;}
        bool operator==(const User& u)const{
            return data==u.data;
        }
        static bool validate_username(const string&username){
            return std::regex_match(username,USERNAME_PATTERN);
        }
        static bool validate_password(const string&password){
            return std::regex_match(password,PASSWORD_PATTERN);
        }
    protected:
        User(const UserData& p):data(std::make_unique<UserData>(p)){}
        std::unique_ptr<UserData> data;
        unsigned short _TYPE;
    private:
        static const std::regex USERNAME_PATTERN;
        static const std::regex PASSWORD_PATTERN;
        
};

template<int TYPE, typename IMPL>
class UserTemplate: public User{
    // 用于向Manager注册类
    // CRTP
    // 参考：https://stackoverflow.com/questions/2850213/dynamically-register-constructor-methods-in-an-abstractfactory-at-compile-time-u
   
   enum {_USER_TYPE_ID = TYPE };
   public:
        static std::unique_ptr<User> instance(const UserData& data) { return std::make_unique<IMPL>(data);}
        static const unsigned short USER_TYPE_ID; 
        // static void Enable() { volatile uint16_t x =  }
   protected:
      UserTemplate(const UserData& p):User(p) { _TYPE = USER_TYPE_ID; } 
};

#ifdef MY_DEBUG
class test_usermanager;
#endif
class UserRecord final{
    #ifdef MY_DEBUG
        friend class test_usermanager;
    #endif
    public:
        typedef std::unique_ptr<User> (*p_user_construct)(const UserData &);

        static UserRecord& get_record(){
            static UserRecord manager;//单例模式
            return manager;
        }
        
        void set(USER_TYPE type,const string& username,const string& password);
        UserData* get(const std::string&username);
        void remove(int id);
        void remove(const std::string& username);
        
        std::unique_ptr<User> create_user(uint16_t msgid,const UserData& data){
            return register_types[msgid](data);
        }
        unsigned short register_type(unsigned short id, p_user_construct factoryMethod){
            printf("Registering constructor for user id %d\n", id);
            register_types[id] = factoryMethod;
            return id;
        }
        int load();
        static constexpr const char* USER_FILE_NAME="user-record.txt";
    
    private:
        UserRecord();
        ~UserRecord(){}
        UserRecord(const UserRecord&){};
        UserRecord& operator=(const UserRecord&);

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
const unsigned short UserTemplate<TYPE, IMPL>::USER_TYPE_ID = UserRecord::get_record().register_type(
    UserTemplate<TYPE, IMPL >::_USER_TYPE_ID, &UserTemplate<TYPE, IMPL >::instance);



/*
* 内联实现
*/
inline UserRecord::UserRecord(){
    
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
inline void UserRecord::insert_data(const UserData&data){
    if(data.id<=0)return;
    set_write_cursor_to_nth_line(data.id);
    database<<data.id<<" "<<data.username<<" "<<data.password<<" "<<data.balance<<" "<<data.type<<"\0\0\0";
    write_LF_nth_line(data.id);
}
// inline void UserRecord::in(const UserData&data){
//     if(data.id<=0)return;
//     set_write_cursor_to_nth_line(data.id);
//     database<<data.id<<" "<<data.username<<" "<<data.password<<" "<<data.balance<<" "<<data.type<<"\0\0\0";
//     write_LF_nth_line(data.id);
// }
inline void UserRecord::remove_data(int id){
    if(id<=0)return;
    //不判断是否存在了.
    set_write_cursor_to_nth_line(id);
    database<<"0\0";database.flush();
    write_LF_nth_line(id);
}
inline void UserRecord::remove(int id){
    auto it = pk_to_name.find(id);
    if(it==pk_to_name.end())return;
    name_to_data.erase(it->second);
    pk_to_name.erase(id);
    remove_data(id);
}
inline void UserRecord::remove(const std::string& username){
    auto it = name_to_data.find(username);
    if(it==name_to_data.end())return;
    int id = it->second.id;
    pk_to_name.erase(it->second.id);
    name_to_data.erase(it);
    remove_data(id);
}
inline UserData* UserRecord::get(const string&username){
    auto it = name_to_data.find(username);
    if(it!=name_to_data.end()){
        return &it->second;
    }
    return nullptr;
}


