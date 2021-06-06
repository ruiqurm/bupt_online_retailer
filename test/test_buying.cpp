#include "concreteGoods.h"
#include "concreteUser.h"
#include "transaction.h"
#include"test.h"
#include<iomanip>
#include<iostream>
using namespace std;

int main(){
    User::register_(USER_TYPE::seller,"seller1","123456");
    auto seller1 =  Seller::cast(User::login("seller1","123456"));
    seller1->balance()+=40.2;
    seller1->save();
    User::register_(USER_TYPE::customer,"customer","123456");
    auto customer = Customer::cast(User::login("customer","123456"));
    customer->balance()+=100000;
    customer->save();
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
    Cart cart(customer->id());
    cart.add(goods1_id,4);
    cart.add(goods2_id,5);
    cart.add(goods3_id,5);
    vector<int>v{goods1_id,goods2_id};
    int id;
    try{
        id = customer->buy(cart,v);
        std::cout<<id<<std::endl;
        ASSERT(id>0,"交易失败1");
        std::cout<<"成功生成订单"<<std::endl;
        auto&record = TransactionRecord::get_record();
       auto p = record.get(id);
       ASSERT(p->set_finished(),"交易失败2 ");
    }catch(const char*str){
        puts(str);
        exit(EXIT_FAILURE);
    }
    // try{
        
        
    // }catch(const char* str){

    // }
    std::cout<<"交易成功";
    return 0;
}