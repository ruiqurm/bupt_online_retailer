#pragma once
#include"user.h"
#include"concreteGoods.h"

class Seller:public UserTemplate<seller,Seller>{
    public:
        Seller(const UserData& p):UserTemplate(p),has_load_goods(false){}
        int get_user_type()const override{
            return _TYPE;
        }
        void add_goods(const Goods& g){

        }
        const std::vector<std::shared_ptr<Goods>>& goods(){
            if (!has_load_goods){
                auto& record = GoodsRecord::get_record();
                _goods = record.get_user_goods(data->id);
                has_load_goods = true;
            }
            return *_goods;
        }
        ~Seller(){}
    private:
        std::unique_ptr<std::vector<std::shared_ptr<Goods>>>_goods;
        bool has_load_goods;
};

class Customer:public UserTemplate<customer,Customer>{
    public:
        Customer(const UserData& p):UserTemplate(p){}
        int get_user_type()const override{
            return _TYPE;
        }
        void buy();
        ~Customer(){}
};
