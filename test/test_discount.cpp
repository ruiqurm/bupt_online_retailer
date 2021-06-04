#include"concreteGoods.h"
#include"concreteUser.h"
#include"test.h"
#include<iostream>
int main(){
    User::register_(USER_TYPE::seller,"seller1","123456");
    auto seller1 =  Seller::cast(User::login("seller1","123456"));
    seller1->balance()+=40.2;
    seller1->save();
    User::register_(USER_TYPE::customer,"customer","123456");
    auto customer = Customer::cast(User::login("customer","123456"));
    GoodsData goods[5]{{"c++ primer",100,seller1->id(),GOODS_TYPE::Books,10,"c++:从入门到入土"},
                           {"Effective C++",100,seller1->id(),GOODS_TYPE::Books,10,"c++:从入门到入土"},
                           {"More Effective C++",100,seller1->id(),GOODS_TYPE::Books,10,"c++:从入门到入土"},
                           {"卫衣",100,seller1->id(),GOODS_TYPE::Clothes,20,"一件普通的卫衣"},
                           {"T-shirt",100,seller1->id(),GOODS_TYPE::Clothes,10,"一件普通的T-shirt"}};
    seller1->add_goods(goods[0]);
    seller1->add_goods(goods[1]);
    seller1->add_goods(goods[2]);
    seller1->add_goods(goods[3]);
    seller1->add_goods(goods[4]);
    auto& record = DiscountRecord::get_record();
    record.make_discount(Discount::DISCOUNT,1,0.7);
    record.make_discount(Discount::DISCOUNT,2,0.9);
    record.make_discount(Discount::DISCOUNT,3,0.9);
    record.make_discount(Discount::DISCOUNT,4,0.9);
    record.make_discount(Discount::DISCOUNT,5,0.9);    

    auto& goodsrecord = GoodsRecord::get_record();
    auto g0 = goodsrecord.get(1);
    if(g0!=0){
        GoodsContext context(g0.get(),1,customer.get());
        double price = g0->get_price(context);
        ASSERT(price ==70,"打折失败")
        std::cout<<g0->price()<<" "<<price;
    }else{
        exit(EXIT_FAILURE);
    }
    record.make_discount(Discount::DISCOUNT,0,0.8);
    try{
        if(g0!=0){
            GoodsContext context(g0.get(),1,customer.get());
            double price = g0->get_price(context);
            ASSERT(price ==70,"打折失败")
            std::cout<<g0->price()<<" "<<price<<std::endl;
        }else{
            exit(EXIT_FAILURE);
        }
    }catch(const char*str){
        puts(str);
    }
    record.update_discount(1,0.6,-1);
    try{
        if(g0!=0){
            GoodsContext context(g0.get(),1,customer.get());
            double price = g0->get_price(context);
            std::cout<<price<<std::endl;
            ASSERT(price ==60,"打折失败3")
        }else{
            exit(EXIT_FAILURE);
        }
    }catch(const char*str){
        puts(str);
    }
    record.make_discount(Discount::TYPE_DISCOUNT,GOODS_TYPE::Books,0.8);
     try{
        if(g0!=0){
            GoodsContext context(g0.get(),1,customer.get());
            double price = g0->get_price(context);
            std::cout<<price<<std::endl;
            ASSERT(price ==60,"打折失败3")
        }else{
            exit(EXIT_FAILURE);
        }
    }catch(const char*str){
        puts(str);
    }
    try{
        auto g1 = goodsrecord.get(2);
        if(g1!=0){
            GoodsContext context(g1.get(),1,customer.get());
            double price = g1->get_price(context);
            std::cout<<price<<std::endl;
            ASSERT(price ==80,"打折失败3")
        }else{
            exit(EXIT_FAILURE);
        }
    }catch(const char*str){
        puts(str);
    }
    g0->price() = 200;
    g0->save();
    try{
        if(g0!=0){
            GoodsContext context(g0.get(),1,customer.get());
            double price = g0->get_price(context);
            std::cout<<price<<std::endl;
            ASSERT(price ==120,"打折失败3")
        }else{
            exit(EXIT_FAILURE);
        }
    }catch(const char*str){
        puts(str);
    }
    goodsrecord.remove(1);
    auto tmp = record.get_all_category_discount();
    // std::cout<<tmp->size()<<std::endl;
   try{
       auto g = goodsrecord.get(5);
       std::cout<<g->name()<<std::endl;
        if(g!=0){
            GoodsContext context(g.get(),1,customer.get());
            double price = g->get_price(context);
            std::cout<<"price: "<<price<<std::endl;
            context.num() = 2;
            price = g->get_price(context);
            std::cout<<"price: "<<price<<std::endl;
            context.num() = 3;
            price = g->get_price(context);
            std::cout<<"price: "<<price<<std::endl;
            ASSERT(price ==270,"打折失败3")
        }else{
            exit(EXIT_FAILURE);
        }
    }catch(const char*str){
        puts(str);
    }
    return 0;
}