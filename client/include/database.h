#pragma once
#include<fstream>
#include<map>
#include<cstring>
#include<cctype>
#include<string>
#include<cstdlib>
#include<memory>
#include<vector>
// #include<for
#include"sqlite3.h"
class Database{
    public:
        static Database* get_db(){
            static Database db;
            return &db;
        }
        Database()
        static  get_db(){
            // static int run_once=1;
            static sqlite3* db;         
            sqlite3_open("data.db",&db);
            return db;
        }
        static bool exec(sqlite3 *db, const char *sql, 
            int (*callback)(void *, int, char **, char **), 
            void *arg){
            char *errmsg=nullptr;
            int rc = sqlite3_exec(db,sql,callback,arg,&errmsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errmsg);
                sqlite3_free(errmsg);
                return false;
            }
            return true;
        }
    protected:
        // static constexpr char database_name[] = "data.db";
        //为什么不能用
    private:

};

template<typename T_class,typename T_data>
class MetaRecord{//通用基类
    public:
        MetaRecord():db(Database::get_db()){};
        
        // 模板类实现必须放在头文件里，否则无法编译
        virtual std::shared_ptr<T_class> get(int id)=0;
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
            insert_data_to_string(buffer,data);
            Database::exec(db,buffer,nullptr,nullptr);
            return sqlite3_last_insert_rowid(db);
        }
        // int set_many(std::vector<T_data*> datas){

        // }
        int size()const{
            static const char sql[] = "SELECT COUNT(*) FROM %s";
            static char buffer[48];
            int tmp;
            snprintf(buffer,48,sql,this->TABLE_NAME);
            Database::exec(db,buffer,get_size,&tmp);
            return tmp;
        }
        virtual void remove(int id){
            static const char sql[] = "DELETE FROM %s WHERE ID = %d; ";
            static char buffer[256]; //可能缓冲区溢出？
            
            sprintf(buffer,sql,this->TABLE_NAME,id);
            puts(buffer);
            Database::exec(db,buffer,nullptr,nullptr);
        }
        void clear(){
            static const char sql[] = "DELETE FROM %s";
            static char buffer[128];
            sprintf(buffer,sql,this->TABLE_NAME);
            Database::exec(db,buffer,nullptr,nullptr);
        }
    protected:
        // 留给子类实现
        // virtual T_class _construct_object(const T_data)=0;
        virtual void insert_data_to_string(char buffer[],const T_data&)=0;
        static const char TABLE_NAME[];
        sqlite3 *db;
    private:
        static int get_size(void*_data, int argc, char **argv, char **azColName){
            int*data = (int*)_data;
            *data = atoi(argv[0]);
            return 0;
        }
};

// struct SqlStatement{
//     public:
//         enum{
//             OR,
//             AND,            

//             GET,
//             SET,
//             UPDATE,
//             DELETE,

//             EQUAL,
//             NO_EQUAL,
//             LESS_THAN,
//             LESS_EQUAL_THAN,
//             BIGGER_THAN,
//             BIGGER_EQUAL_THAN
//         };
//         explicit SqlStatement(int predicate,const char* database);

//         void add_condition();
//     private:
//         int predicate;
//         string database;
// };
// class SimpleSql{
//     //简单sql语句,支持
//     private:
        
// };
// class Stringify{
    
// };