#pragma once
#include<string>
#include<iostream>
#include<memory>
#include<regex>
#include<map>
// #include <filesystem>
#include <sys/stat.h>
#include"database.h"


using std::string;
using std::ostream;
using std::shared_ptr;

class UserData{
    public:
        // char username[32];
        // char password[16];
        string username;
        string password;
        int id;
        double balance;
        int type;
        UserData(){}
        UserData(int id,const char*username,
                const char*password,double balance,
                int type):id(id),balance(balance),type(type),username(username),password(password){}
        UserData(int id,const string& username,
        const string& password,double balance,
        int type):id(id),balance(balance),type(type),username(username),password(password){}
        bool save();
};
class User{
    public:
        virtual ~User(){}
        virtual int get_user_type()const=0;
        
        //登录与注册
        static std::shared_ptr<User> login(const string& username,const string& password);
        static bool register_(int type,const string& username,const string& password);
        
        bool change_password(const string&pre_pass,const string&after_pass){
            if(pre_pass!=data->password)throw"password incorrect";
            if(!validate_password(after_pass))throw "password syntax error";
            data->password.assign(after_pass);
            return save();
        }
        string& password(){return data->password;}
        const string& password()const{return data->password;}
        const string& username()const{return data->username;}
        string& username(){return data->username;}
        double deposit(double money){
            data->balance+=money;
            save();
            return data->balance;
        }
        double& balance(){
            return data->balance;
        }        
        const double& balance()const{return data->balance;}
        int id()const{return data->id;}
        bool operator==(const User& u)const{
            return data==u.data;
        }
        bool validate_password(const string&password){
            return (data->password == password);
        }
        static bool validate_username_format(const string&username){
            return std::regex_match(username,USERNAME_PATTERN);
        }
        static bool validate_password_format(const string&password){
            return std::regex_match(password,PASSWORD_PATTERN);
        }
        bool save();
    protected:
        User(UserData* p):data(p){}
        UserData* data;
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
        static std::shared_ptr<User> instance(UserData* data) { return std::make_shared<IMPL>(data);}
        static const unsigned short USER_TYPE_ID; 
        static std::shared_ptr<IMPL> cast(std::shared_ptr<User>u){
            return std::dynamic_pointer_cast<IMPL>(u);
        }
        // static void Enable() { volatile uint16_t x =  }
   protected:
      UserTemplate(UserData* p):User(p) { _TYPE = USER_TYPE_ID; } 
};

#if DEBUG==1
class test_usermanager;
#endif

class UserRecord final{
    #if DEBUG==1
        friend class test_usermanager;
    #endif
    public:
        typedef std::shared_ptr<User> (*p_user_construct)(UserData *);

        static UserRecord& get_record(){
            static UserRecord manager;//单例模式
            return manager;
        }
        
        void set(int type,const string& username,const string& password);
        bool update(const UserData&data);
        UserData* get(const std::string&username);
        UserData* get(int id);
        string get_username(int id);
        void remove(int id);
        void remove(const std::string& username);
        void clear();
        std::shared_ptr<User> create_user(UserData* data){
            if(data==nullptr)return std::shared_ptr<User>(nullptr);
            else return register_types[data->type](data);
        }
        unsigned short register_type(unsigned short id, p_user_construct factoryMethod);
        int load();
        static constexpr const char* USER_FILE_NAME="user-record.txt";
        bool exist (const std::string& name) {
            struct stat buffer;   
            return (stat (name.c_str(), &buffer) == 0); 
        }
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
            #ifdef WIN32
            database.seekp(id*MAX_LINE -2);
            #else
            database.seekp(id*MAX_LINE -1);
            #endif
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
    
    if (exist(USER_FILE_NAME)){
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
inline void UserRecord::remove_data(int id){
    if(id<=0)return;
    //不判断是否存在了.
    set_write_cursor_to_nth_line(id);
    database<<"0\0";database.flush();
    // write_LF_nth_line(id);
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

inline UserData* UserRecord::get(int id){
    auto it = pk_to_name.find(id);
    if(it!=pk_to_name.end()){
        return get(it->second);
    }
    return nullptr;
}
inline string UserRecord::get_username(int id){
    auto it =pk_to_name.find(id);
    return (it!=pk_to_name.end())?it->second:string();
}
