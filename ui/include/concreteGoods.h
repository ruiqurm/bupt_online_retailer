/**
 * @file concreteGoods.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 商品派生类
 */
#pragma once
#include"goods.h"
#include<algorithm>
namespace GOODS_TYPE{
    enum{
        Books,/// 书
        Clothes,/// 衣服
        EGoods/// 电子商品
    };
}

/**
 * @brief 商品书
 * 
 */
class Books:public GoodsTemplate<GOODS_TYPE::Books,Books>{
    public:
        Books(const GoodsData&data):GoodsTemplate(data){}
        
        /// 获取商品类别
        int get_goods_type()const override{
            return _TYPE;
        }
};

class Clothes:public GoodsTemplate<GOODS_TYPE::Clothes,Clothes>{
    public:
        Clothes(const GoodsData&data):GoodsTemplate(data){}

        /// 获取商品类别
        int get_goods_type()const override{
            return _TYPE;
        }
};

class EGoods:public GoodsTemplate<GOODS_TYPE::Clothes,EGoods>{
    public:
        EGoods(const GoodsData&data):GoodsTemplate(data){}
        /// 获取商品类别
        int get_goods_type()const override{
            return _TYPE;
        }
       
    //    double get_price(const GoodsContext& context){
    //         return get_price(context,false);
    //     }
    //     double get_price(const GoodsContext& context,bool flush){
    //         if(!init_price || flush){
    //             auto&record = DiscountRecord::get_record();
    //             double min_price=context.goods().price();
    //             auto p1 = record.get_goods_discount(context.goods().id());
    //             if(p1){
    //                 min_price = std::min(p1->price(context),min_price);
    //             }
    //             auto p2 = record.get_category_discount(context.goods().type());
    //             if(p2){
    //                 min_price = std::min(p2->price(context),min_price);
    //             }
    //             final_price = min_price;
    //             init_price = true;
    //             return min_price;
    //         }else{
    //             return context.num() * final_price;
    //         }
    //     }
    // private:
    //     bool init_price;
    //     double final_price;
};

