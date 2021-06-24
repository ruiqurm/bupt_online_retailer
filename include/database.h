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

/**
 * @brief 数据库接口
 * @details 用于提供数据库cursor，提供一个简单的执行函数
 */
class Database{
    public:
        /**
         * @brief 获取一个sqlite3 db结构体
         * 
         * @return sqlite3指针
         */
        static sqlite3* get_db(){
            // static int run_once=1;
            static sqlite3* db;         
            sqlite3_open("data.db",&db);
            return db;
        }
        /**
         * @brief 执行一条命令，若返回数据，执行回调函数
         * 
         * @param db sqlite3指针
         * @param sql sql语句
         * @param callback 回调函数
         * @param arg 参数的地址
         * @return 是否执行成功
         */
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
/**
 * @brief 数据库接口基类
 * 
 * @tparam T_class 处理逻辑的类
 * @tparam T_data 存放数据的结构体
 */
template<typename T_class,typename T_data>
class MetaRecord{//通用基类
    public:
        MetaRecord():db(Database::get_db()){};
        
        // 模板类实现必须放在头文件里，否则无法编译
        /**
         * @brief 获取
         * 
         * @param id 数据库id
         * @return std::shared_ptr<T_class> 
         */
        virtual std::shared_ptr<T_class> get(int id)=0;

        /**
         * @brief 保存数据到数据库
         * @details 子类需要实现insert_data_to_string
         * @param data 数据
         * @return int 新数据的id
         */
        int set(const T_data& data){
            static char buffer[512];
            insert_data_to_string(buffer,data);// 把sql语句写到缓存区
            Database::exec(db,buffer,nullptr,nullptr);
            return sqlite3_last_insert_rowid(db);// 获取当前线程上一次操作的行号
        }

        /**
         * @brief 数据库表大小
         * 
         * @return int 大小
         */
        int size()const{
            static const char sql[] = "SELECT COUNT(*) FROM %s";
            static char buffer[48];
            int tmp;
            snprintf(buffer,48,sql,this->TABLE_NAME);// 把sql语句写到缓存区
            Database::exec(db,buffer,get_size,&tmp);
            return tmp;
        }

        /**
         * @brief 移除一条记录
         * 
         * @param id 数据id
         */
        virtual void remove(int id){
            static const char sql[] = "DELETE FROM %s WHERE ID = %d; ";
            static char buffer[256]; //可能缓冲区溢出？
            
            sprintf(buffer,sql,this->TABLE_NAME,id);// 把sql语句写到缓存区
            puts(buffer);
            Database::exec(db,buffer,nullptr,nullptr);
        }

        /**
         * @brief 清空数据库表
         * 
         */
        void clear(){
            static const char sql[] = "DELETE FROM %s";
            static char buffer[128];
            sprintf(buffer,sql,this->TABLE_NAME);// 把sql语句写到缓存区
            Database::exec(db,buffer,nullptr,nullptr);
        }
    protected:
        // 留给子类实现
        // virtual T_class _construct_object(const T_data)=0;
        virtual void insert_data_to_string(char buffer[],const T_data&)=0;
        static const char TABLE_NAME[];
        sqlite3 *db;
    private:
        /// sqlite3回调函数
        static int get_size(void*_data, int argc, char **argv, char **azColName){
            int*data = (int*)_data;
            *data = atoi(argv[0]);
            return 0;
        }
};

