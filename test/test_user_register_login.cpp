#include "test.h"
#include "user.h"
#include "database.h"
#include<iostream>


using namespace std;

int main(){
    remove_database();
    try{
        UserRecord& record = UserRecord::get_record();
        auto& manager = UserManager::getInstance();
        ASSERT(manager.register_(customer,"aaaa","123456")==1,"register error");
        ASSERT(record.size()==1,"register error(database error)");
        
        auto u = manager.login("aaaa","123456");
        ASSERT(u!=nullptr,"login failed");
        ASSERT(u->get_user_type()==customer,"类别错误");
        ASSERT(u->get_user_name()==string("aaaa"),"名字错误");
        auto u2 = manager.login("aaaa","123456");
        ASSERT(*u==*u2,"登录管理有问题");
        
    }catch(const char*err){
        cerr<<err<<endl;
        remove_database();
        return -1;
    }
    remove_database();
    return 0;

}
