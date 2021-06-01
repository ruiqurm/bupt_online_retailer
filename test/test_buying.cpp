#include "concreteGoods.h"
#include "concreteUser.h"
#include"test.h"
#include<iomanip>
#include<iostream>
using namespace std;

std::ostream& operator<<(std::ostream &os,const Goods& goods){
    os<<setiosflags(ios::left)<<"商品(名字name: "<<setw(32)<<goods.name()<<"价格: "<<setw(10)<<goods.price()<<"元  剩余: "<<setw(10)<<goods.remain()<<" 商家:"<<setw(16)<<goods.seller_name()<<" 描述:"<<setw(32)<<goods.description()<<")"<<endl;
    return os;
}
int main(){
    try{
        User::register_(USER_TYPE::seller,"seller1","123456");
        User::register_(USER_TYPE::seller,"seller2","123456");
        User::register_(USER_TYPE::customer,"customer","123456");
        auto seller1 =  Seller::cast(User::login("seller1","123456"));
        auto seller2 =  Seller::cast(User::login("seller2","123456"));
        auto customer = Customer::cast(User::login("customer","123456"));
        cout<<"创建了"<<seller1->username()<<"和"<<customer->username()<<endl;
        GoodsData goods[5]{{"c++ primer",50,seller1->id(),GOODS_TYPE::Books,10,"c++:从入门到入土"},
                           {"Effective C++",50,seller1->id(),GOODS_TYPE::Books,10,"c++:从入门到入土"},
                           {"More Effective C++",50,seller2->id(),GOODS_TYPE::Books,10,"c++:从入门到入土"},
                           {"卫衣",15,seller1->id(),GOODS_TYPE::Clothes,20,"一件普通的卫衣"},
                           {"T-shirt",2.0,seller2->id(),GOODS_TYPE::Books,10,"一件普通的T-shirt"}};
        seller1->add_goods(goods[0]);
        seller1->add_goods(goods[1]);
        seller2->add_goods(goods[2]);
        seller1->add_goods(goods[3]);
        seller2->add_goods(goods[4]);
        
        auto& goodsrecord = GoodsRecord::get_record();
        {
            auto pvec = goodsrecord.get_user_goods(seller1->id());
            cout<<"seller1新增加的商品:\n";
            for(auto &it:*pvec){
                cout<<*it;
            }
        }
        {
            auto pvec = goodsrecord.get_user_goods(seller2->id());
            cout<<"seller2新增加的商品:\n";
            for(auto &it:*pvec){
                cout<<*it;
            }
        }
        cout<<"用户当前余额:"<<customer->balance()<<endl;
        cout<<"充值后"<<endl;
        customer->deposit(1000);
        cout<<"用户当前余额:"<<customer->balance()<<endl;
        {
            auto pvec = goodsrecord.get_all_goods();
            ASSERT(pvec->size()>0,"数量不足");
            cout<<"购买:"<<(*(*pvec)[0]);
            cout<<"购买3个\n";
            customer->buy(*(*pvec)[0],3);
        }
        cout<<"用户当前余额:"<<customer->balance()<<endl;
        ASSERT(customer->balance()==850,"价格错误");
        {
            cout<<"查找带有c++的书"<<endl;
            auto pvec = goodsrecord.get_goods_by_name("c++");
            ASSERT(pvec->size()==3,"get_goods_by_name出错");
            for(auto &it:*pvec){
                cout<<*it;
            }
        }
    }catch(const char str[]){
        cout<<str;
        return -1;
    }catch(const exception& e){
        cout<<e.what();
        return -1;
    }
    return 0;
}