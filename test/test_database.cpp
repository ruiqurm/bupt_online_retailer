#include "database.h"
#include "test.h"
#include<cstdio>


UserData data[10]{
    {1,"testuser1","123",10,0},
    {2,"testuser2","123",20,0},
    {3,"testuser3","123",30,0},
    {4,"testuser4","123",40,0},
    {5,"testuser5","123",40,0},    
    {6,"testuser6","123",40,0},
    {7,"testuser7","123",40,0},    
    {8,"test user8","123",40,0},//错误数据
};
std::ostream& operator<<(std::ostream&os,const UserData& data){
    os<<data.id<<" "<<data.username<<" "<<data.password<<" "<<data.balance;
    return os;
}
int main(){
   remove_database();
   try{
    /*********************
    *测试基础写入功能
    ***********************/
    UserRecord& record = UserRecord::get_record();
    record.set(data[0]);
    record.set(data[1]);
    record.set(data[2]);
    record.set(data[3]);
    record.set(data[4]);
    record.set(data[5]);
    ASSERT(record.size()==6,"size error");
    record.remove(4);
    record.remove("testuser1");
    record.remove(8);
    record.remove("testuser0");
    ASSERT(record.size()==4,"size error");
    record.save();
    ASSERT(record.size()==4,"size error");
    ASSERT(record.get("testuser1")==nullptr,"get testuser1 failed");
    ASSERT(record.get("testuser2")!=nullptr,"get testuser2 failed");
    ASSERT(record.get("testuser3")!=nullptr,"get testuser3 failed");
    ASSERT(record.get("testuser4")==nullptr,"get testuser4 failed");
    ASSERT(record.get("testuser5")!=nullptr,"get testuser5 failed");
    ASSERT(record.get("testuser6")!=nullptr,"get testuser6 failed");
    /*********************
    *测试单例模式
    ***********************/
    UserRecord& record2 = UserRecord::get_record();
    ASSERT(record2.get("testuser2")!=nullptr,"record2 error");
   /********************
    * 测试放入带空格数据/重复数据
    * ******************/ 
    try{
        record2.set(data[7]);
    }catch(const char*error){
        ASSERT(strncmp("disallow space character in username",error,10)==0,"不能检测出空格");
    }
    ASSERT(record2.set(data[2])==-1,"不能去重");
   }catch (const char* error){
       cerr<<error<<std::endl;
       remove_database();
       return -1;
   }
   remove_database();
   return 0;
}