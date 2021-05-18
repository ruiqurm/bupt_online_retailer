#include "test.h"

#include "database.h"
#include<iostream>
#include<fstream>
#include<map>
#define MY_DEBUG
#include "user.h"
using namespace std;

class test_usermanager{
    public:
        test_usermanager():p(new UserManager()){}
        test_usermanager(UserManager *p):p(p){}
        UserManager* p;
        void close_database(){
            p->database.close();
        }
        std::map<std::string, UserData>& name_to_data(){
            return p->name_to_data;
        }
};
void test_passing_customer(Customer u){
    cout<<u.get_user_name();
}
int main(int argc,char** argv){

    database_clean::parse_argument(argc,argv);
    try{
        {
            auto& manager = UserManager::getInstance();
            test_usermanager middle(&manager);
            ASSERT(manager.register_(customer,"aaaa","123456")==true,"aaaa register error");
            auto u = manager.login("aaaa","123456");
            ASSERT(u!=nullptr,"login failed");
            ASSERT(u->get_user_type()==customer,"类别错误");
            ASSERT(u->get_user_name()==string("aaaa"),"名字错误");
            auto u2 = manager.login("aaaa","123456");
            ASSERT(*u==*u2,"登录管理有问题");
            ASSERT(manager.register_(seller,"bbbb","123456")==true,"bbbb register error");
            ASSERT(manager.register_(customer,"cccc","123456")==true,"cccc register error");
            ASSERT(manager.register_(customer,"dddd","123456")==true,"dddd register error");
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
            auto& manager = UserManager::getInstance();
            auto u = manager.login("aaaa","123456");
            test_passing_customer(*((Customer*)u.get()));
            ASSERT(u->get_user_name()==string("aaaa"),"传递参数时被删除");
        }
    }catch(const char*err){
        cerr<<err<<endl;
        database_clean::remove_database();
        return -1;
    }
    database_clean::remove_database();
    return 0;

}
