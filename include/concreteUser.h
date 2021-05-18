#pragma once
#include"user.h"
#include"goods.h"
class Seller:public UserTemplate<seller,Seller>{
    public:
        Seller(UserData* p):UserTemplate(p){}
        int get_user_type()const override{
            return _TYPE;
        }
        void add_goods(Goods*);
        void goods();
        ~Seller(){}
    private:
        std::vector<std::shared_ptr<Goods>>all_goods;
};

class Customer:public UserTemplate<customer,Customer>{
    public:
        Customer(UserData* p):UserTemplate(p){}
        int get_user_type()const override{
            return _TYPE;
        }
        ~Customer(){}
};
