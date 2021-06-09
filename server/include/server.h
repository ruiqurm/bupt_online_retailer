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

class Parser{
    //处理报文
    public:
        void parse(char buffer[]);
        
        void write_header_good(char write_buf[]);
        void write_header_error(char write_buf[]); 
        void write_header_authenticate_error(char write_buf[]);

};



class UserRecordWriter{
    public:
        // typedef std::shared_ptr<User> (*p_user_construct)(UserData *);
        static constexpr const char* USER_FILE_NAME="user-record.txt";
        static UserRecordWriter& get_record(){
            static UserRecordWriter manager;//单例模式
            return manager;
        }
        
        bool exist (const std::string& name) {
            struct stat buffer;   
            return (stat (name.c_str(), &buffer) == 0); 
        }

        /*
        * 登录功能和鉴权
        */
        string login(const protoData::UserForm & form);
        bool logout(protoData::User*user);
        bool check_password(protoData::User*user,const string& password);
        bool _register(protoData::UserForm & form);
        void update(protoData::User&);

        protoData::User* get_user_by_token(const char* token);
        protoData::User* get_user_by_username(const string&username);
        
        
    private:
        UserRecordWriter();
        ~UserRecordWriter(){}
        UserRecordWriter(const UserRecordWriter&){};
        UserRecordWriter& operator=(const UserRecordWriter&);


        /*
        * 内存缓冲
        */
        std::map<int,std::string> id_to_token;
        std::map<std::string,int> token_to_id;//token和ID
        std::map<std::string,protoData::User> name_to_data;
        std::map<int,std::string> id_to_name;
        int max_pk;

        /*
        * 文件操作
        */
        std::fstream database;
        int load();
        // static constexpr int MAX_LINE = sizeof(UserData) + 10;
        static constexpr int MAX_LINE = 256;
        static constexpr int TOKEN_SIZE = 16;

        
        void insert_data(protoData::UserForm& user);
        
        void remove_data(int id);
        void set_write_cursor_to_nth_line(int id);
        void set_read_cursor_to_nth_line(int  id);
        void write_LF_nth_line(int id);
        string generate_token();
};




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
                default:
                    break;
            }
            return nullptr;
        }

};
