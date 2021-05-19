#pragma once
#include"user.h"
#include"concreteGoods.h"

class Seller:public UserTemplate<seller,Seller>{
    public:
        Seller(UserData* p):UserTemplate(p),has_load_goods(false){}
        int get_user_type()const override{
            return _TYPE;
        }
        void add_goods(Goods*);
        void goods(){
            if (!has_load_goods){
                auto& record = GoodsRecord::get_record();
                // all_goods.assign();
            }
        }
        ~Seller(){}
    private:
        std::vector<std::shared_ptr<Goods>>all_goods;
        bool has_load_goods;
};

class Customer:public UserTemplate<customer,Customer>{
    public:
        Customer(UserData* p):UserTemplate(p){}
        int get_user_type()const override{
            return _TYPE;
        }
        ~Customer(){}
};
