#include "test.h"

#include "database.h"
#include<iostream>
#include<fstream>
#include<map>
#define DEBUG 1
#include "user.h"
#include"concreteUser.h"
using namespace std;

class test_usermanager{
    public:
        test_usermanager():p(new UserRecord()){}
        test_usermanager(UserRecord *p):p(p){}
        UserRecord* p;
        void close_database(){
            p->database.close();
        }
        std::map<std::string, UserData>& name_to_data(){
            return p->name_to_data;
        }
};
void test_passing_customer(Customer u){
    cout<<u.username();
}
int main(int argc,char** argv){
    // database_clean::parse_argument(argc,argv);
    try{
        {
            auto& manager = UserRecord::get_record();
            test_usermanager middle(&manager);
            ASSERT(User::register_(USER_TYPE::customer,"aaaa","123456")==true,"aaaa register error");
            auto u = User::login("aaaa","123456");
            ASSERT(u!=nullptr,"login failed");
            ASSERT(u->get_user_type()==USER_TYPE::customer,"类别错误");
            ASSERT(u->username()==string("aaaa"),"名字错误");
            ASSERT(User::register_(USER_TYPE::seller,"bbbb","123456")==true,"bbbb register error");
            ASSERT(User::register_(USER_TYPE::customer,"cccc","123456")==true,"cccc register error");
            ASSERT(User::register_(USER_TYPE::customer,"dddd","123456")==true,"dddd register error");
            manager.remove("bbbb");
            manager.remove(3);
            middle.close_database();
            manager.load();
            // cout<<middle.name_to_data().size()<<endl;
             ASSERT(middle.name_to_data().size()==2,"读取文件错误 size != 2");
            auto tmp = middle.name_to_data().find("aaaa");
            ASSERT(tmp!=middle.name_to_data().end() && tmp->second.id==1,"读取文件错误");
            cout<<"load 正常"<<endl;
            // manager.database.close();
        }
        {
            //测试修改密码
            
            //直接使用数据库
            auto& record = UserRecord::get_record();
            record.clear();
            UserData d(0,"aaaa","123456",0,USER_TYPE::customer);
            record.set(USER_TYPE::customer,"aaaa","123456");
            
            auto it = record.get("aaaa");
            ASSERT(it->password!="1234567","指针错误");
            d.password="1234567";
            d.id = it->id;
            cout<<record.update(d)<<endl;
            ASSERT(it->password=="1234567","无法修改数据");
            
            ASSERT(User::register_(USER_TYPE::customer,"bbbb","123456")==true,"bbbb register error");
            auto u = User::login("bbbb","123456");
            ASSERT(u->change_password("123456","1234567"),"无法修改密码");
            auto it2 = record.get("bbbb");
            ASSERT(it2->password=="1234567","无法修改数据");
            // record.update
        }
    }catch(const char*err){
        cerr<<err<<endl;
        // database_clean::remove_database();
        return -1;
    }
    cout<<"测试正常退出"<<endl;
    // database_clean::remove_database();
    return 0;

}
