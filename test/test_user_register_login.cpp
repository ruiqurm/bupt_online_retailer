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
        
        User* u = manager.login("aaaa","123456");
        cout<<"ok"<<endl;
        ASSERT(u!=nullptr,"login failed");

        ASSERT(u->getUserType()!=customer,"类别错误");
    }catch(const char*err){
        cerr<<err<<endl;
        remove_database();
        return -1;
    }
    remove_database();
    return 0;

}
