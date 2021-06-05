#include"concreteGoods.h"
#include"test.h"
#include"concreteUser.h"
#include"transaction.h"
#include<iostream>
#define DEBUG 1
using namespace std;

int main(int argc,char**argv){
    // database_clean::parse_argument(argc,argv);
    User::register_(USER_TYPE::seller,"seller1","123456");
    auto seller1 =  Seller::cast(User::login("seller1","123456"));
    seller1->balance()+=100000;
    seller1->save();
    User::register_(USER_TYPE::customer,"customer","123456");
    auto customer = Customer::cast(User::login("customer","123456"));
    GoodsData goods[5]{{"c++ primer",100,seller1->id(),GOODS_TYPE::Books,10,"c++:从入门到入土"},
                        {"Effective C++",100,seller1->id(),GOODS_TYPE::Books,10,"c++:从入门到入土"},
                        {"More Effective C++",100,seller1->id(),GOODS_TYPE::Books,10,"c++:从入门到入土"},
                        {"卫衣",100,seller1->id(),GOODS_TYPE::Clothes,20,"一件普通的卫衣"},
                        {"T-shirt",100,seller1->id(),GOODS_TYPE::Clothes,10,"一件普通的T-shirt"}};
    int goods1_id = seller1->add_goods(goods[0]);
    int goods2_id = seller1->add_goods(goods[1]);
    int goods3_id = seller1->add_goods(goods[2]);
    int goods4_id = seller1->add_goods(goods[3]);
    int goods5_id = seller1->add_goods(goods[4]);
    std::map<int,std::tuple<string,int,double>> data;
    data[goods1_id] = std::make_tuple(goods[0].name,5,goods[0].price * 5);
    data[goods2_id] = std::make_tuple(goods[1].name,4,goods[1].price * 4);
    data[goods3_id] = std::make_tuple(goods[2].name,3,goods[2].price * 3);
    Transaction t(seller1->id(),customer->id(),goods[0].price * 5+goods[1].price * 4+goods[2].price * 3,data);
    {
        auto&record =GoodsRecord::get_record();
        ASSERT(record.get(goods1_id)->remain() == goods[0].remain-5,"没有扣除1");
        ASSERT(record.get(goods2_id)->remain() == goods[1].remain-4,"没有扣除2");
        ASSERT(record.get(goods3_id)->remain() == goods[2].remain-3,"没有扣除2");
        auto&trecord = TransactionRecord::get_record();
        int size1 = trecord.get_transaction(seller1->id(),false)->size();
        int size2 = trecord.get_transaction(seller1->id(),true)->size();
        // ASSERT(trecord.get_transaction(seller1->id(),true)->size()==0,"未完成的订单数目错误");
        t.set_finished();
        ASSERT(trecord.get_transaction(seller1->id(),false)->size()==size1-1,"未完成的订单数目错误");
        ASSERT(trecord.get_transaction(seller1->id(),true)->size()==size2+1,"完成的订单数目错误");
    }
    Transaction t2(seller1->id(),customer->id(),goods[0].price * 5+goods[1].price * 4+goods[2].price * 3,data);
    {
        auto&trecord = TransactionRecord::get_record();
        int size = trecord.get_transaction(seller1->id(),false)->size();
        t2.cancel();
        ASSERT(trecord.get_transaction(seller1->id(),false)->size()==size-1,"未完成的订单数目错误");
        auto&record =GoodsRecord::get_record();
        ASSERT(record.get(goods1_id)->remain() == goods[0].remain-5,"多扣除了");
    }
    cout<<"测试完成"<<endl;
    return 0;
//    database_clean::remove_database();
}