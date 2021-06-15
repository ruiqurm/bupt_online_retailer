#include "concreteUser.h"

using std::vector;
using std::map;
using std::string;
std::tuple<string,vector<int>> Customer::buy(Cart&cart,std::vector<int> &selection){
    auto&record = GoodsRecord::get_record();
    auto data =  record.get(selection);
    if (data->size()<selection.size())return {"存在已删除的商品",vector<int>()};
    if(selection.size()==0)return {"传入商品数量为空",vector<int>()};
    // int seller = -1;
    map<int,vector<Goods*>> tasks;
    for(auto& goods:*data){
        if(goods->remain()<cart[goods->id()]){
            return {"商品数量不足",vector<int>()};
        }
        // if(seller==-1)seller = goods->seller();
        tasks[goods->seller()].push_back(goods.get());
        // if(goods->seller()!=seller){
        //     return -3;//不允许多个
        // }
    }
    if(tasks.find(this->data->id)!=tasks.end()){
        return {"尝试自交易",vector<int>()};//自交易
    }
    // if(seller == this->data->id)return -4;
    vector<int> transaction_ids;
    transaction_ids.reserve(tasks.size());
    for (const auto &[seller, goods_vector] : tasks){
        //c++ 17 
        //https://stackoverflow.com/questions/4207346/how-can-i-traverse-iterate-an-stl-map
        std::map<int,std::tuple<std::string,int,double>>m;
        double total = 0;
        for(auto& goods:goods_vector){
            int count  = cart[goods->id()];
            cart.remove(goods->id());
            GoodsContext context(goods,count,this);
            double p = goods->get_price(context);
            m[goods->id()] = std::make_tuple(
                goods->name(),
                count,
                p
            );
            total += p;
        }
        std::cout<<"本份订单有"<<goods_vector.size()<<"条"<<std::endl;
        Transaction t(seller,this->data->id,total,m);
        transaction_ids.push_back(t.id());
    }
    
   return {"",transaction_ids};
 }

std::tuple<string,vector<int>> Seller::buy(Cart&cart,std::vector<int> &selection){
    auto&record = GoodsRecord::get_record();
    auto data =  record.get(selection);
    if (data->size()<selection.size())return {"存在已删除的商品",vector<int>()};
    if(selection.size()==0)return {"传入商品数量为空",vector<int>()};
    // int seller = -1;
    map<int,vector<Goods*>> tasks;
    for(auto& goods:*data){
        if(goods->remain()<cart[goods->id()]){
            return {"商品数量不足",vector<int>()};
        }
        // if(seller==-1)seller = goods->seller();
        tasks[goods->seller()].push_back(goods.get());
        // if(goods->seller()!=seller){
        //     return -3;//不允许多个
        // }
    }
    if(tasks.find(this->data->id)!=tasks.end()){
        return {"尝试自交易",vector<int>()};//自交易
    }
    // if(seller == this->data->id)return -4;
    vector<int> transaction_ids;
    transaction_ids.reserve(tasks.size());
    for (const auto &[seller, goods_vector] : tasks){
        //c++ 17 
        //https://stackoverflow.com/questions/4207346/how-can-i-traverse-iterate-an-stl-map
        std::map<int,std::tuple<std::string,int,double>>m;
        double total = 0;
        for(auto& goods:goods_vector){
            int count  = cart[goods->id()];
            cart.remove(goods->id());
            GoodsContext context(goods,count,this);
            double p = goods->get_price(context);
            m[goods->id()] = std::make_tuple(
                goods->name(),
                count,
                p
            );
            total += p;
        }
        Transaction t(seller,this->data->id,total,m);
        transaction_ids.push_back(t.id());
    }
    
   return {"",transaction_ids};
 }