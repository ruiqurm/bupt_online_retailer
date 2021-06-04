#pragma once
#include"user.h"
#include"goods.h"
namespace USER_TYPE{
enum{
    customer,
    seller
};
}
class Seller:public UserTemplate<USER_TYPE::seller,Seller>{
    public:
        Seller(UserData* p):UserTemplate(p),has_load_goods(false){}
        int get_user_type()const override{
            return _TYPE;
        }
        void add_goods(GoodsData g){
            auto& record = GoodsRecord::get_record();
            g.seller = data->id;
            record.set(g);
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

class Customer:public UserTemplate<USER_TYPE::customer,Customer>{
    public:
        Customer(UserData* p):UserTemplate(p){}
        int get_user_type()const override{
            return _TYPE;
        }
        int buy(Goods&,int num);
        ~Customer(){}

};
