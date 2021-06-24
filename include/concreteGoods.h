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

        /**
         * @brief 获取商品书的价格
         * 
         * @param context 上下文
         * @return double 价格
         */
        double get_price(const GoodsContext& context)override{
            auto&record = DiscountRecord::get_record();
            double min_price=context.goods().price()*context.num();
            auto p1 = record.get_goods_discount(context.goods().id());
            /// 分别查看单品打折和品类打折，取最小值
            if(p1){
                min_price = std::min(p1->price(context),min_price);
                std::cout<<p1->price(context)<<std::endl;
            }
            auto p2 = record.get_category_discount(context.goods().type());
            if(p2){
                min_price = std::min(p2->price(context),min_price);
            }

            return min_price;
        }

        /// 获取商品类别
        int get_goods_type()const override{
            return _TYPE;
        }
};

class Clothes:public GoodsTemplate<GOODS_TYPE::Clothes,Clothes>{
    public:
        Clothes(const GoodsData&data):GoodsTemplate(data){}
        double get_price(const GoodsContext& context)override{
            auto&record = DiscountRecord::get_record();
            double min_price=context.goods().price()*context.num();
            auto p1 = record.get_goods_discount(context.goods().id());
            /// 分别查看单品打折和品类打折，取最小值
            if(p1){
                min_price = std::min(p1->price(context),min_price);
            }
            auto p2 = record.get_category_discount(context.goods().type());   
            if(p2){
                min_price = std::min(p2->price(context),min_price);
            }
            return min_price;
        }
        int get_goods_type()const override{
            return _TYPE;
        }
};

class EGoods:public GoodsTemplate<GOODS_TYPE::Clothes,EGoods>{
    public:
        EGoods(const GoodsData&data):GoodsTemplate(data){}
        int get_goods_type()const override{
            return _TYPE;
        }
        double get_price(const GoodsContext& context)override{
            auto&record = DiscountRecord::get_record();
            double min_price=context.goods().price()*context.num();
            auto p1 = record.get_goods_discount(context.goods().id());
            /// 分别查看单品打折和品类打折，取最小值
            if(p1){
                min_price = std::min(p1->price(context),min_price);
            }
            auto p2 = record.get_category_discount(context.goods().type());   
            if(p2){
                min_price = std::min(p2->price(context),min_price);
            }
            return min_price;
        }
    
};