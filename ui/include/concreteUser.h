#pragma once
#include"user.h"
#include"goods.h"
#include"transaction.h"
#include<map>
#include<vector>
#include<string>

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
        int add_goods(GoodsData g){
            auto& record = GoodsRecord::get_record();
            g.seller = data->id;
            return record.set(g);
        }
        const std::vector<std::shared_ptr<Goods>>& goods(){
            if (!has_load_goods){
                auto& record = GoodsRecord::get_record();
                _goods = record.get_user_goods(data->id);
                has_load_goods = true;
            }
            return *_goods;
        }
        std::tuple<std::string,std::vector<int>>  buy(Cart&cart,std::vector<int>&selection);
        ~Seller(){}
        // int buy(std::vector<int> selection);
    private:
        std::unique_ptr<std::vector<std::shared_ptr<Goods>>>_goods;
        bool has_load_goods;
};

class Customer:public UserTemplate<USER_TYPE::customer,Customer>{
    public:
        Customer(UserData* p):UserTemplate(p){}
        std::tuple<std::string,std::vector<int>>  buy(Cart&cart,std::vector<int>&selection);
        int get_user_type()const override{
            return _TYPE;
        }
        
        ~Customer(){}

};
