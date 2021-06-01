#include"concreteGoods.h"
#include"test.h"
#include<iostream>

using namespace std;

int main(int argc,char**argv){
    // database_clean::parse_argument(argc,argv);
    try
    {
        {
            auto& record = GoodsRecord::get_record();
            GoodsData data("aaaa",2.0,1,GOODS_TYPE::Clothes,1,"11111");
            cout<<"init data. data.name = "<<data.name<<endl;
            int x = record.set(data);
            cout<<"insert data into row "<<x<<endl;
            auto goods = record.get(x);
            cout<<"storing data. data.name = "<<goods->name()<<endl;
            ASSERT(goods->name()==string("aaaa"),"获取数据出错");
            ASSERT(goods->type()==GOODS_TYPE::Clothes,"获取数据出错(2)");
            record.remove(x);
            cout<<"remove data from row "<<x<<endl;
        }
        {
            auto& record = GoodsRecord::get_record();
            record.clear();
            GoodsData data[10]{
                {"aaaa",2.0,1,GOODS_TYPE::Clothes,1,"11111"},
                {"bbbb",2.0,1,GOODS_TYPE::Clothes,1,"11111"},
                {"cccc",2.0,1,GOODS_TYPE::Clothes,1,"11111"},
                {"dddd",2.0,1,GOODS_TYPE::Clothes,1,"11111"},
                {"eeee",2.0,1,GOODS_TYPE::Clothes,1,"11111"},
                {"ffff",2.0,1,GOODS_TYPE::Clothes,1,"11111"},

            };
            for(int i=0;i<4;i++){
                record.set(data[i]);
            }
            auto vec = record.get_user_goods(1);
            ASSERT(vec->size()==4,"vector数量错误");
            for(int i=0;i<4;i++){
                ASSERT((*vec)[i]->name()==string(data[i].name),"数据错误");
            }
            (*vec)[0]->price() = 4.0;
            (*vec)[0]->save();
            auto g = record.get((*vec)[0]->id());
            cout<<g->price();
            ASSERT(g->price()==4.0,"修改数据错误")
        
        }
    }catch(const char*error){
        cout<<error;
        return -1;
    }
    cout<<"测试get_user_goods完成"<<endl;
    return 0;
//    database_clean::remove_database();
}