/**
 * @file user.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 用户基类，用户数据结构体，用户数据库操作
 */

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


/**
 * @brief 用户数据保存的结构体
 * @details 主要为了方便保存到文件中。后面第三部分的时候发现不是必需的，但是为了兼容之前的版本，就没有进行更改。
 */
class UserData{
    public:
        string username; //用户名
        string password; //密码
        int id;  //主键id
        double balance; // 余额
        int type; // 用户类型
        UserData(){}
        UserData(int id,const char*username,
                const char*password,double balance,
                int type):id(id),balance(balance),type(type),username(username),password(password){}
        UserData(int id,const string& username,
        const string& password,double balance,
        int type):id(id),balance(balance),type(type),username(username),password(password){}

        /**
         * @brief 保存写入数据库
         * 
         * @return true 保存成功
         * @return false 保存失败
         */
        bool save();
};

/**
 * @brief 用户类
 * 
 * @details 提供两个静态函数：登录和注册；提供内部数据的接口和保存数据到数据库的函数
 */
class User{
    public:
        virtual ~User(){}
        virtual int get_user_type()const=0;
        
        /**
         * @brief 登录
         * @details 
         * @param username 用户名
         * @param password 密码
         * @return 一个指向user的智能指针，自动决定用户类型
         */
        static std::shared_ptr<User> login(const string& username,const string& password);
       
        /**
         * @brief 注册
         * 
         * @param type 用户类型
         * @param username 用户名
         * @param password 密码
         * @return true 注册成功
         * @return false 注册失败
         */
        static bool register_(int type,const string& username,const string& password);
        
        /**
         * @brief 修改密码
         * @deprecated 为了方便起见，gui中修改密码不需要再输入旧密码
         * @param pre_pass 原来的密码
         * @param after_pass 现在的密码
         * @return true 修改成功
         * @return false 修改失败
         */
        bool change_password(const string&pre_pass,const string&after_pass){
            if(pre_pass!=data->password)throw"password incorrect";
            if(!validate_password(after_pass))throw "password syntax error";
            data->password.assign(after_pass);
            return save();
        }
        /**
         * @brief 返回密码的引用（可修改）
         * 
         * @return string& 
         */
        string& password(){return data->password;}

        /**
         * @brief 返回密码
         * 
         * @return const string& 
         */
        const string& password()const{return data->password;}

        /**
         * @brief 返回用户名的引用
         * 
         * @return const string& 
         */
        const string& username()const{return data->username;}

        /**
         * @brief 返回用户名
         * 
         * @return string& 
         */
        string& username(){return data->username;}

        /**
         * @brief 存钱
         * 
         * @param money 要存的钱数
         * @return double 
         */
        double deposit(double money){
            data->balance+=money;
            save();
            return data->balance;
        }

        /**
         * @brief 余额的引用
         * 
         * @return double& 
         */
        double& balance(){
            return data->balance;
        }        

        /**
         * @brief 余额
         * 
         * @return double
         */
        double balance()const{return data->balance;}

        /**
         * @brief id
         * 
         * @return int 
         */
        int id()const{return data->id;}

        /**
         * @brief 判断两个用户是否相等
         * 
         * @param u 用户对象
         * @return 是否相同
         */
        bool operator==(const User& u)const{
            return data->id==u.data->id;
        }
        /**
         * @brief 校验密码
         * 
         * @param password 密码
         * @return 是否相同
         */
        bool validate_password(const string&password){
            return (data->password == password);
        }

        /**
         * @brief 校验用户名格式
         * @details 用户名要求大于4个字符且小于16个字符，并且不能有空白符
         * @param username 用户名 
         * @return 格式是否正确
         */
        static bool validate_username_format(const string&username){
            return std::regex_match(username,USERNAME_PATTERN);
        }

        /**
         * @brief 校验密码格式
         * @details 密码要求大于4个字符且小于16个字符，并且不能有空白符
         * @param username 密码 
         * @return 格式是否正确
         */
        static bool validate_password_format(const string&password){
            return std::regex_match(password,PASSWORD_PATTERN);
        }
        /**
         * @brief 保存用户数据
         * @return 是否保存成功
         */
        bool save();
    protected:
        User(UserData* p):data(p){}
        UserData* data;///< 用户数据
        unsigned short _TYPE;///< 用户类型
    private:
        static const std::regex USERNAME_PATTERN;// 校验用户名格式
        static const std::regex PASSWORD_PATTERN;// 校验密码格式
        
};

/**
 * @brief 用于动态构造不同类型的用户的中间类
 * @details  
 * 使用CRTP，动态地构造不同类型的用户。
 * 关于CRTP，参见：参考：https://stackoverflow.com/questions/2850213/dynamically-register-constructor-methods-in-an-abstractfactory-at-compile-time-u
 * 例如，有一个叫A类型的User，那么，它的声明应该这样写：
 * @code
 * class A:public UserTemplate<1,A>{
 *   public:
 *       A(UserData* p):UserTemplate(p){}
 *       int get_user_type()const override{
 *           return 1;
 *       }
 * //......
 * };
 * @endcode 
 * 这样，使用login的时候就能动态分配类型了。
 * @tparam TYPE 一个正数，小于256，用于表示类型id
 * @tparam IMPL 不同类型的子类
 */
template<int TYPE, typename IMPL>
class UserTemplate: public User{
   enum {_USER_TYPE_ID = TYPE };
   public:
        /**
         * @brief 保存子类的构造函数，在父类调用构造的时候，会委托这个函数进行构造。
         * 
         * @param data 用户数据
         * @return std::shared_ptr<User> 
         */
        static std::shared_ptr<User> instance(UserData* data) { return std::make_shared<IMPL>(data);}
        static const unsigned short USER_TYPE_ID; 

        /**
         * @brief 把基类指针转换成子类指针
         * 
         * @return std::shared_ptr<IMPL> 
         */
        static std::shared_ptr<IMPL> cast(std::shared_ptr<User>u){
            return std::dynamic_pointer_cast<IMPL>(u);
        }

   protected:
        /**
         * @brief 构造函数
         * @details 这里的_TYPE看起来是冗余的，但实际是必需的，如果把User基类和此处的_TYPE中去掉，编译就会失败
         * @param p UserData指针
         */
        UserTemplate(UserData* p):User(p) { _TYPE = USER_TYPE_ID; } 
};

#if DEBUG==1
class test_usermanager;
#endif

/**
 * @brief 用户类的数据库操作
 * @details 这里为了方便起见，事实上把数据库接口（类比sql语句）和数据库操作（类比sql语句的执行）合并在一起了。
 * 这里做了一个类似cache的结构，初始化时，读取文件内容，然后按名字作为键加载到一个map中；需要用户数据时，直接取map中的数据即可。
 * 如果遇到写操作，那么就修改cache，并写入到文件中。
 */
class UserRecord final{
    #if DEBUG==1
        friend class test_usermanager;
    #endif
    public:
        //指向构造函数的指针
        typedef std::shared_ptr<User> (*p_user_construct)(UserData *);

        static UserRecord& get_record(){
            static UserRecord manager;///< 单例模式
            return manager;
        }
        /**
         * @brief 增加一条user记录
         * 
         * @param type 类型
         * @param username 用户名
         * @param password 密码
         */
        void set(int type,const string& username,const string& password);

        /**
         * @brief 更新一条user记录
         * @details 不能更改用户名和主键
         * @param data user数据
         * @return 修改是否成功
         */
        bool update(const UserData&data);

        /**
         * @brief 获取一条user记录
         * @details 
         * @param username 用户名
         * @return UserData* 记录指针，不需要释放内存
         */
        UserData* get(const std::string&username);

        /**
         * @brief 获取一条user记录
         * 
         * @param id id
         * @return UserData* 记录指针，不需要释放内存
         */
        UserData* get(int id);

        /**
         * @brief 从id获取用户名
         * 
         * @param id id
         * @return string 用户名
         */
        string get_username(int id);

        /**
         * @brief 通过id移除一条记录
         * 
         * @param id id
         */
        void remove(int id);

        /**
         * @brief 通过用户名移除一条记录
         * 
         * @param username 用户名
         */
        void remove(const std::string& username);

        /**
         * @brief 清空数据库
         * @details 后面没有用过
         */
        void clear();

        /**
         * @brief 通过userdata构造相应的user对象的父类指针
         * @details 这里也可以把返回值改成auto(c++17)，然后直接cast成对应的子类指针
         * @param data 
         * @return std::shared_ptr<User> user对象的父类指针
         */
        std::shared_ptr<User> create_user(UserData* data){
            if(data==nullptr)return std::shared_ptr<User>(nullptr);
            else return register_types[data->type](data);
        }

        /**
         * @brief 动态注册用户类，只在程序初始化的时候运行
         * @details 把用户类的构造函数保存到一个数组里；此处为了方便起见，用的是256单位的静态函数指针数组 
         * @param id 用户类id
         * @param factoryMethod 工厂函数，
         * @see UserTemplate::instance
         * @return unsigned short 
         */
        unsigned short register_type(unsigned short id, p_user_construct factoryMethod);

        /**
         * @brief 加载本地用户文件
         * 
         * @return 读取了多少个用户
         * @details 判断本地文件是否存在；如果不存在，创建；否则，尝试读取本地文件
         */
        int load();

        ///用户数据库名
        static constexpr const char* USER_FILE_NAME="user-record.txt";

        /**
         * @brief 判断文件是否存在
         * 
         * @param name 文件名
         * @return 文件是否存在
         */
        bool exist (const std::string& name)const{
            struct stat buffer;   
            return (stat (name.c_str(), &buffer) == 0); 
        }
    private:
        UserRecord();
        ~UserRecord(){}
        UserRecord(const UserRecord&){};
        UserRecord& operator=(const UserRecord&);

        /// 子类构造函数的数组
        p_user_construct register_types[256];

        /*
        * 内存缓冲
        */
        /// 保存名字到用户名的映射
        std::map<std::string,UserData> name_to_data;

        /// 保存id到用户名的映射
        std::map<int,std::string> pk_to_name;

        /// 当前最大id
        int max_pk;

        /*
        * 文件操作
        */
        
        /// 数据库文件流
        std::fstream database;

        /// 每行固定长度
        static constexpr int MAX_LINE = sizeof(UserData) + 10;

        /**
         * @brief 向文件中插入一条数据
         * 
         * @param data 
         */
        void insert_data(const UserData&data);

        /**
         * @brief 在文件中移除一条数据
         * 
         * @param id 
         */
        void remove_data(int id);

        /**
         * @brief 把写指针移到第id-1行开始
         * 
         * @param id id比行数多1，例如id=1应该写第0行
         */
        void set_write_cursor_to_nth_line(int id){
            database.seekp((id-1)*(MAX_LINE));
        }
        /**
         * @brief 把读指针放到第id-1行开始
         * 
         * @param id id比行数多1，例如id=1应该写第0行
         */
        void set_read_cursor_to_nth_line(int  id){
            database.seekg((id-1)*(MAX_LINE));
        }

        /**
         * @brief 在行末写入换行符
         * @details windows下会写入\n\r，因此要-2
         * @param id 第id-1行
         */
        void write_LF_nth_line(int id){
            #ifdef WIN32
            database.seekp(id*MAX_LINE -2);
            #else
            database.seekp(id*MAX_LINE -1);
            #endif
            database<<'\n';
        }
};

/// 动态为所有继承的用户子类注册类型
template <int TYPE, typename IMPL>
const unsigned short UserTemplate<TYPE, IMPL>::USER_TYPE_ID = UserRecord::get_record().register_type(
    UserTemplate<TYPE, IMPL >::_USER_TYPE_ID, &UserTemplate<TYPE, IMPL >::instance);



/*
* 内联实现
*/
inline UserRecord::UserRecord(){
    if (exist(USER_FILE_NAME)){///> 判断是否存在
        load();///> 存在则加载内部数据
        database.open(USER_FILE_NAME,std::ios::in|std::ios::out);
        //不会覆盖原来的内容
    }else{
        /// 否则新建一个文件
        database.open(USER_FILE_NAME,std::ios::out);
    }
    if (!database.is_open()){
        throw "can't open user record file.";
    }
}
inline void UserRecord::insert_data(const UserData&data){
    if(data.id<=0)return;
    set_write_cursor_to_nth_line(data.id);///> 游标移到第id-1行
    database<<data.id<<" "<<data.username<<" "<<data.pasword<<" "<<data.balance<<" "<<data.type;///> 依次输入数据
    database.put('\0');database.put('\0');database.put('\0');///> 加上\0 防止和之前数据连起来
    write_LF_nth_line(data.id);///> 在最后加上换行符
}
inline void UserRecord::remove_data(int id){
    if(id<=0)return;
    //不判断是否存在了.
    set_write_cursor_to_nth_line(id);
    database<<"0\0";database.flush();
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
