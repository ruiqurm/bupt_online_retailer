#pragma once
#include<fstream>
#include<map>
#include<cstring>
#include<cctype>
#include<string>
#include<cstdlib>
#include<memory>
#include<sqlite3.h>
// #pragma comment(lib,"sqlite3.lib")
class Database{
    public:
        static sqlite3* get_db(){
            // static int run_once=1;
            static sqlite3* db;         
            sqlite3_open("data.db",&db);
            return db;
        }
        static void exec(sqlite3 *db, const char *sql, 
            int (*callback)(void *, int, char **, char **), 
            void *arg){
            char *errmsg=nullptr;
            int rc = sqlite3_exec(db,sql,callback,arg,&errmsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errmsg);
                sqlite3_free(errmsg);
            }
        }
    protected:
        // static constexpr char database_name[] = "data.db";
        //为什么不能用
    private:

};


template<typename T>
class RecordInterfase{
    public:
        virtual const T* get(int id)=0;
        virtual int set(T)=0;
        virtual void remove(int id)=0;
        virtual T* update(int id)=0;
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

class UserRecord:public RecordInterfase<UserData>{
    public:
        UserRecord(const UserRecord&) = delete;
        UserRecord &operator=(const UserRecord&) = delete;
        //阻止拷贝构造

        static UserRecord& get_record(){
            static UserRecord record;
            return record;
        }
        const UserData* get(int id)override;
        const UserData* get(const std::string& username);
        int set(UserData data)override;
        void remove(const std::string& username);
        void remove(int id)override;
        UserData* update(int id)override;
        UserData* update(const std::string& username);
        int size(){return (name_to_data.size());}
        bool exist(const std::string& username){return (name_to_data.find(username)!=name_to_data.end());}
        int load();
        void save();
        ~UserRecord(){}
    protected:
        bool file_exist(const char* file) {
            std::ifstream f(file);
            return f.good();
        }
        UserRecord();
    private:
        std::map<std::string,UserData> name_to_data;
        std::map<int,std::string> pk_to_name;
        std::ofstream database;
        int max_pk;
        static constexpr const char* USER_FILE_NAME="user-record.txt";
        static constexpr int USERDATA_SIZE = sizeof(UserData);
};

template<typename T_class,typename T_data>
class MetaRecord{//通用基类
    public:
        MetaRecord():db(Database::get_db()){};
        
        // 模板类实现必须放在头文件里，否则无法编译
        // T_class get(int id){
        //     static const char sql[] = "SELECT * FROM %s WHERE ID=%d";
        //     static char buffer[256]; //可能缓冲区溢出？
        //     T_data data;

        //     sprintf(buffer,sql,this->TABLE_NAME,id);
        //     Database::exec(db,buffer,fetch_in_struct,&data);
        //     return _construct_object(data);
        // }
        // void update_key(const string& key_name,const string key_value);
        int set(const T_data& data){
            static char buffer[512];
            _data_to_string(buffer,data);
            Database::exec(db,buffer,nullptr,nullptr);
            return sqlite3_last_insert_rowid(db);
        }
        void remove(int id){
            static const char sql[] = "DELETE FROM %s WHERE ID = %d; ";
            static char buffer[256]; //可能缓冲区溢出？

            sprintf(buffer,sql,this->TABLE_NAME,id);
            Database::exec(db,buffer,nullptr,nullptr);
        }
    protected:
        // 留给子类实现
        // virtual T_class _construct_object(const T_data)=0;
        virtual void _data_to_string(char buffer[],const T_data&)=0;
        static const char TABLE_NAME[];
        sqlite3 *db;
        // static int fetch_in_struct(void*data, int argc, char **argv, char **azColName);
        //static是因为作为sql回调
};

// class GoodsRecord{
//     public:
//         GoodsRecord();
//         Goods get(int id);
//         Goods get(const std::string& username);
//         void set(GoodsData data);
        
//         void remove(const std::string& username);
//         void remove(int id);
//     private:
//         static int fetch_in_struct(void*data, int argc, char **argv, char **azColName);
//         sqlite3 *db;
// };