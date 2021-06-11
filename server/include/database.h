#pragma once
#include<fstream>
#include<string>
#include"sqlite3.h"
#include <sys/stat.h> 
#include"protoData.pb.h"
using std::fstream;
using std::string;
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
        protoData::User* get_user(const string&username);
        protoData::User* get_user(int id);
        
        
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

class Database{
    public:
        Database();
        bool exec(const char *sql, int (*callback)(void *, int, char **, char **),void *arg);
        static bool exec(sqlite3* db,const char *sql, int (*callback)(void *, int, char **, char **),void *arg);
    private:
        sqlite3 *db;
};
class GoodsDatabase:public Database{
    public:
        static bool init();
        GoodsDatabase(){}
        int create(protoData::Goods& goods);
        void get(protoData::Goods& goods);
        void get(protoData::GoodsArray&array);
        void get_user_goods(int seller_id,protoData::GoodsArray&array);
        void get_all_goods(protoData::GoodsArray&array);
        void get_goods_by_name(const string& search_for_name,protoData::GoodsArray&array);
        bool update(const protoData::Goods& data);
        void remove(const protoData::Goods& data);
        const static bool is_init;
    private:
        static int fetch_to_object(void*_data, int argc, char **argv, char **azColName);
        static int fetch_to_vector(void*_data, int argc, char **argv, char **azColName);
};

class DiscountDatabase:public Database{
    public:
        //创建一条折扣
        static bool init();
        DiscountDatabase(){}
        int create(protoData::Discount& discount);
        void update(protoData::Discount& discount);
        void get_category_discount(protoData::Discount& discount);
        void get_goods_discount(protoData::Discount& discount);
        void get_all_category_discount(protoData::Discount&discount,protoData::DiscountArray& array);
        void remove_by_goods(protoData::Discount& discount);
        void remove(protoData::Discount& discount);
        const static bool is_init;
    protected:
        static int fetch_to_object(void*_data, int argc, char **argv, char **azColName);
        static int fetch_to_vector(void*_data, int argc, char **argv, char **azColName);
};

class TransactionDatabase:public Database{
    public:
        static bool init();
        const static bool is_init;
        TransactionDatabase(){}

        void get_transaction(protoData::Transaction&,protoData::TransactionArray&array);
        
        //生成订单
        int set(protoData::Transaction&);
        
        //获取订单
        void get(protoData::Transaction&);

        //移除订单。如果未完成会释放资源，否则跳过
        void cancel(protoData::Transaction&);

        //确认订单
        void set_finished(protoData::Transaction&);

    protected:
        static int fetch_to_object(void*_data, int argc, char **argv, char **azColName);
        static int fetch_to_vector(void*_data, int argc, char **argv, char **azColName);
};


class CartDatabase:public Database{
    public:
        static bool init();
        const static bool is_init;
        CartDatabase(){}

        void get(protoData::CartItem&item, protoData::Cart&);
        int set(protoData::CartItem&item);
        void remove(protoData::CartItem&item);

    protected:
        static int fetch_to_vector(void*_data, int argc, char **argv, char **azColName);
};