#include "concreteUser.h"

int Customer::buy(Cart&cart,std::vector<int> &selection){
    auto&record = GoodsRecord::get_record();
    auto data =  record.get(selection);
    if (data->size()<selection.size())return -1;
    if(selection.size()==0)return -1;
    int seller = -1;
    for(auto& goods:*data){
        if(goods->remain()<cart[goods->id()]){
            return -2;
        }
        if(seller==-1)seller = goods->seller();
        if(goods->seller()!=seller){
            return -3;//不允许多个
        }
    }
    if(seller == this->data->id)return -4;
    std::map<int,std::tuple<std::string,int,double>>m;
    double total = 0;
    for(auto& goods:*data){
        int count  = cart[goods->id()];
        cart.remove(goods->id());
        GoodsContext context(goods.get(),count,this);
        double p = goods->get_price(context);
        m[goods->id()] = std::make_tuple(
            goods->name(),
            count,
            p
        );
        total += p;
    }
   Transaction t(seller,this->data->id,total,m);
   return t.id();
 }
 int Seller::buy(Cart&cart,std::vector<int> &selection){
    auto&record = GoodsRecord::get_record();
    auto data =  record.get(selection);
    if (data->size()<selection.size())return -1;
    if(selection.size()==0)return -1;
    int seller = -1;
    for(auto& goods:*data){
        if(goods->remain()<cart[goods->id()]){
            return -2;
        }
        if(seller==-1)seller = goods->seller();
        if(goods->seller()!=seller){
            return -3;//不允许多个
        }
    }
    if(seller == this->data->id)return -4;
    std::map<int,std::tuple<std::string,int,double>>m;
    double total = 0;
    for(auto& goods:*data){
        int count  = cart[goods->id()];
        goods->remain()-=count;
        goods->save();
        cart.remove(goods->id());
        GoodsContext context(goods.get(),count,this);
        double p = goods->get_price(context);
        m[goods->id()] = std::make_tuple(
            goods->name(),
            count,
            p
        );
        total += p;
    }
   Transaction t(seller,this->data->id,total,m);
   return t.id();
 }
 