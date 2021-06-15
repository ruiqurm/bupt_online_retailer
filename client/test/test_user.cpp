// #include"testfunction.h"
#include"concreteGoods.h"
#include"concreteUser.h"
int main(){
    
    
    try{
        User::login("ruiqurm","123456");
        auto&record = GoodsRecord::get_record();
        GoodsData data("aa21312312a",20.0,1,0,12,"asdasdads");
        std::cout<<"..............."<<std::endl;
        std::cout<<"id="<<record.set(data)<<std::endl;
    }
    catch(const char* s){
        std::cout<<s;
    }
}