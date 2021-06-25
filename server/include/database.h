/**
 * @file database.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 服务端数据库
 */
#pragma once
#include<fstream>
#include<string>
#include"sqlite3.h"
#include <sys/stat.h> 
#include"protoData.pb.h"
using std::fstream;
using std::string;

/**
 * @brief 用户类的数据库操作
 * @details 这里做了一个类似cache的结构，初始化时，读取文件内容，然后按名字作为键加载到一个map中；需要用户数据时，直接取map中的数据即可。
 * 如果遇到写操作，那么就修改cache，并写入到文件中。
 * token是一个16字节的字符串，由字母和数字组成
 */
class UserRecordWriter{
    public:
        // typedef std::shared_ptr<User> (*p_user_construct)(UserData *);
        
        /// 文件名
        static constexpr const char* USER_FILE_NAME="user-record.txt";
        
        static UserRecordWriter& get_record(){
            static UserRecordWriter manager;//单例模式
            return manager;
        }
        
        /// 判断是否存在
        bool exist (const std::string& name) {
            struct stat buffer;   
            return (stat (name.c_str(), &buffer) == 0); 
        }

        /*
        * 登录功能和鉴权
        */
        
        /**
         * @brief 登录
         * 
         * @param form 账号，密码
         * @return string 
         */
        string login(const protoData::UserForm & form);

        /**
         * @brief 登出
         * 
         * @param user 用户指针
         */
        bool logout(protoData::User*user);

        /**
         * @brief 校验密码
         * 
         * @param user 用户指针
         * @param password 密码
         * @return 密码是否正确
         */
        bool check_password(protoData::User*user,const string& password);

        /**
         * @brief 注册
         * 
         * @param form 账号，密码和类型
         * @return 注册是否成功
         */
        bool _register(protoData::UserForm & form);

        /**
         * @brief 更新用户信息
         * 
         */
        void update(protoData::User&);


        /**
         * @brief 通过token获取用户
         * 
         * @param token 16个字节的字符串，可以不带\0
         * @return protoData::User* 
         */
        protoData::User* get_user_by_token(const char* token);

        /**
         * @brief 通过用户名获取用户
         * 
         * @param username 用户名
         * @return protoData::User* 
         */
        protoData::User* get_user(const string&username);

        /**
         * @brief 通过id获取用户
         * 
         * @param id 用户id
         * @return protoData::User* 
         */
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

        /// 插入数据
        void insert_data(protoData::UserForm& user);
        
        /// 移除一条数据
        void remove_data(int id);

        /// 移动游标到第n行
        void set_write_cursor_to_nth_line(int id);

        /// 移动读游标在第n行
        void set_read_cursor_to_nth_line(int  id);

        /// 在n行最后写一个换行符(不带回车)
        void write_LF_nth_line(int id);

        /// 生成token
        string generate_token();
};

/**
 * @brief 数据库接口基类
 * @details 此处采用的是继承的形式，用组合的形式可能会更好
 * 
 */
class Database{
    public:
        Database();
        
        /// 执行一条sql语句
        bool exec(const char *sql, int (*callback)(void *, int, char **, char **),void *arg);

        /// 静态成员函数，执行一条sql语句，调用一个全局的sqlite指针
        static bool exec(sqlite3* db,const char *sql, int (*callback)(void *, int, char **, char **),void *arg);
    private:
        sqlite3 *db;
};

/**
 * @brief 商品数据库接口
 * 
 */
class GoodsDatabase:public Database{
    public:
        /// 建表，程序启动时执行
        static bool init();
        GoodsDatabase(){}

        /**
         * @brief 创建商品
         * 
         * @param goods 商品信息
         * @return int 数据库内行号
         */
        int create(protoData::Goods& goods);

        /**
         * @brief 获取指定id的商品，并填回原参数
         * 
         * @param goods 
         */
        void get(protoData::Goods& goods);

        /**
         * @brief 获取若干个商品
         * 
         * @param array 
         */
        void get(protoData::GoodsArray&array);

        /**
         * @brief 通过用户获取商品
         * 
         * @param seller_id 用户id
         * @param array 
         */
        void get_user_goods(int seller_id,protoData::GoodsArray&array);

        /**
         * @brief 获取全部商品，结果在参数中返回
         * 
         * @param array 
         */
        void get_all_goods(protoData::GoodsArray&array);

        /**
         * @brief 搜索商品，结果在参数array中返回
         * 
         * @param search_for_name 搜索的名字
         * @param array 返回的结果
         */
        void get_goods_by_name(const string& search_for_name,protoData::GoodsArray&array);

        /**
         * @brief 更新商品
         * 
         * @param data 商品数据
         * @return 是否更新成功
         */
        bool update(const protoData::Goods& data);

        /**
         * @brief 移除商品
         * 
         * @param data 商品id（结构体只需要填id即可）
         */
        void remove(const protoData::Goods& data);


        const static bool is_init;
    private:
        /// 把sql输出转换成对象
        static int fetch_to_object(void*_data, int argc, char **argv, char **azColName);
        
        /// 把sql输出转换成对象，放进vector
        static int fetch_to_vector(void*_data, int argc, char **argv, char **azColName);
};

/**
 * @brief 折扣数据库接口
 * 
 */
class DiscountDatabase:public Database{
    public:
        /// 建表，程序启动时执行
        static bool init();
        DiscountDatabase(){}

        /**
         * @brief 创建折扣
         * 
         * @param discount 折扣对象
         * @return int 数据库中的id号
         */
        int create(protoData::Discount& discount);

        /**
         * @brief 更新折扣
         * 
         * @param discount 折扣
         */
        void update(protoData::Discount& discount);

        /**
         * @brief 获取品类折扣
         * 
         * @param discount 折扣
         */
        void get_category_discount(protoData::Discount& discount);

        /**
         * @brief 获取商品折扣
         * 
         * @param discount 折扣对象
         */
        void get_goods_discount(protoData::Discount& discount);

        /**
         * @brief 获取所有品类折扣
         * 
         * @param discount 折扣
         * @param array 
         */
        void get_all_category_discount(protoData::Discount&discount,protoData::DiscountArray& array);

        /**
         * @brief 移除商品折扣
         * 
         * @param discount 折扣
         */
        void remove_by_goods(protoData::Discount& discount);

        /**
         * @brief 移除折扣
         * 
         * @param discount 
         */
        void remove(protoData::Discount& discount);

        const static bool is_init;
    protected:
        /// 把sql输出转换成对象
        static int fetch_to_object(void*_data, int argc, char **argv, char **azColName);

        /// 把sql输出转换成对象，放进vector
        static int fetch_to_vector(void*_data, int argc, char **argv, char **azColName);
};

/**
 * @brief 订单数据库接口
 * 
 */
class TransactionDatabase:public Database{
    public:
        /// 建表，程序启动时执行
        static bool init();
        const static bool is_init;
        TransactionDatabase(){}

        /**
         * @brief 获取订单
         * 
         * @param array 
         */
        void get_transaction(protoData::Transaction&,protoData::TransactionArray&array);
        
        /**
         * @brief 生成订单
         * 
         * @return int 
         */
        int set(protoData::Transaction&);
        
        /**
         * @brief 获取订单
         * 
         */
        void get(protoData::Transaction&);

        /**
         * @brief 移除订单。如果未完成会释放资源，否则跳过
         * 
         */
        void cancel(protoData::Transaction&);

        /**
         * @brief 确认订单
         * 
         */
        void set_finished(protoData::Transaction&);

    protected:
        /// 把sql输出转换成对象
        static int fetch_to_object(void*_data, int argc, char **argv, char **azColName);

        /// 把sql输出转换成对象，放进vector
        static int fetch_to_vector(void*_data, int argc, char **argv, char **azColName);
};


/**
 * @brief 购物车数据库接口
 * 
 */
class CartDatabase:public Database{
    public:
        /// 建表，程序启动时执行 
        static bool init();
        const static bool is_init;
        CartDatabase(){}

        /**
         * @brief 获取购物车数据
         * @details 这样的参数设计比较奇怪，但是为了节省时间，就暂时这样设计了
         * @param item 只读取里面的用户id
         */
        void get(protoData::CartItem&item, protoData::Cart&);

        /**
         * @brief 创建购物车项
         * 
         * @param item 购物车项
         * @return int 新项的数据库id
         */
        int set(protoData::CartItem&item);

        /**
         * @brief 移除一个数据库项
         * 
         * @param item 只用id
         */
        void remove(protoData::CartItem&item);

    protected:
        /// 把sql输出转换成对象，放进vector
        static int fetch_to_vector(void*_data, int argc, char **argv, char **azColName);
};