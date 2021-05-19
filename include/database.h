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
#include<sqlite3.h>
#pragma comment(lib,"sqlite3.lib")
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
            _data_to_string(buffer,data);
            Database::exec(db,buffer,nullptr,nullptr);
            return sqlite3_last_insert_rowid(db);
        }
        // int set_many(std::vector<T_data*> datas){

        // }
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