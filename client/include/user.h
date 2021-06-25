/**
 * @file user.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 用户基类，用户数据结构体
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
        // char username[32];
        // char password[16];
        string username;///> 用户名
        string password;///> 密码
        int id;///> 主键id
        double balance;///> 余额
        int type;///> 用户类型
        UserData():id(-1){}
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
         * @brief 登出
         * @details 对于本地没有什么影响，但是会在服务端登出
         */
        void logout();
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

        /// 返回密码的引用（可修改）     
        string& password(){return data->password;}

        /// 返回密码       
        const string& password()const{return data->password;}

        /// 返回用户名的引用     
        const string& username()const{return data->username;}

        /// 返回用户名的引用   
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
        /// 余额
        double& balance(){
            return data->balance;
        }        
        /// 余额
        const double& balance()const{return data->balance;}

        int id()const{return data->id;}
        bool operator==(const User& u)const{
            return data==u.data;
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
        static std::shared_ptr<IMPL> cast(std::shared_ptr<User>u){
            return std::dynamic_pointer_cast<IMPL>(u);
        }

        /**
         * @brief 把基类指针转换成子类指针
         * 
         * @return std::shared_ptr<IMPL> 
         */        
        static IMPL* cast(User*u){
            return dynamic_cast<IMPL*>(u);
        }
        // static void Enable() { volatile uint16_t x =  }
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
 */
class UserRecord{
    #if DEBUG==1
        friend class test_usermanager;
    #endif
    public:
        //指向构造函数的指针    
        typedef std::shared_ptr<User> (*p_user_construct)(UserData *);

        static UserRecord& get_record(){
            static UserRecord manager;//单例模式
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
         * @brief 登录
         * 
         * @param username 用户名
         * @param password 密码
         * @return UserData* 
         */
        UserData* login(const string&username,const string&password);

        /**
         * @brief 登出
         * @details 用户账号适合socket绑定的，因此不需要提供参数
         */
        void logout();
        // void remove(int id);
        // void remove(const std::string& username);

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

        unsigned short register_type(unsigned short id, p_user_construct factoryMethod);    
    private:
        UserRecord():base(Database::get_database()){}
        ~UserRecord(){}
        UserRecord(const UserRecord&){};
        UserRecord& operator=(const UserRecord&);
        Database* base;
        char send_buf[8192];
        char recv_buf[8192];
        p_user_construct register_types[256];
};

/// 动态为所有继承的用户子类注册类型
template <int TYPE, typename IMPL>
const unsigned short UserTemplate<TYPE, IMPL>::USER_TYPE_ID = UserRecord::get_record().register_type(
    UserTemplate<TYPE, IMPL >::_USER_TYPE_ID, &UserTemplate<TYPE, IMPL >::instance);



