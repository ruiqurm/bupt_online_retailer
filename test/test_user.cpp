#include "test.h"
#include "user.h"
#include "database.h"
#include<iostream>
#include<fstream>

using namespace std;

int main(int argc,char** argv){

    database_clean::parse_argument(argc,argv);
    try{
        auto& manager = UserManager::getInstance();
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
        // ifstream f("")
    }catch(const char*err){
        cerr<<err<<endl;
        database_clean::remove_database();
        return -1;
    }
    database_clean::remove_database();
    return 0;

}
