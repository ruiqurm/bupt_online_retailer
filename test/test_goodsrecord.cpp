#include"concreteGoods.h"
#include"test.h"
#include<iostream>
using namespace std;

int main(int argc,char**argv){
    database_clean::parse_argument(argc,argv);
    try
    {
        auto& record = GoodsRecord::get_record();
        GoodsData data("aaaa",2.0,1,goods_type::Clothes,1,"11111");
        cout<<"init data. data.name = "<<data.name<<endl;
        int x = record.set(data);
        cout<<"insert data into row "<<x<<endl;
        auto goods = record.get(x);
        cout<<"storing data. data.name = "<<goods->name()<<endl;
        ASSERT(goods->name()==string("aaaa"),"获取数据出错");
        ASSERT(goods->type()==goods_type::Clothes,"获取数据出错(2)");
        record.remove(x);
        cout<<"remove data from row "<<x<<endl;
    }catch(const char*error){
        cout<<error;
    }
    database_clean::remove_database();
}