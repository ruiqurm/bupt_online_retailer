#pragma once
#include"goods.h"
#include<algorithm>
namespace GOODS_TYPE{
    enum{
        Books,
        Clothes,
        EGoods
    };
}

class Books:public GoodsTemplate<GOODS_TYPE::Books,Books>{
    public:
        Books(const GoodsData&data):GoodsTemplate(data){}

        double get_price(const GoodsContext& context)override{
            auto&record = DiscountRecord::get_record();
            double min_price=context.goods().price()*context.num();//TODO: magic number;
            auto p1 = record.get_goods_discount(context.goods().id());
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
            // if(p1!=N)
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
// template<int type,typename cls>
// class SimpleGoods:public GoodsTemplate<type,cls>{
//     public:
//         SimpleGoods(const GoodsData&data):GoodsTemplate<int type,typename cls>(data){}
//         double get_price(const GoodsContext& context)override{
//             return get_price(context,false);
//         }
//         double get_price(const GoodsContext& context,bool flush){
//             if(!init_price || flush){
//                 auto&record = DiscountRecord::get_record();
//                 double min_price=context.goods().price();
//                 auto p1 = record.get_goods_discount(context.goods().id());
//                 if(p1){
//                     min_price = std::min(p1->price(context),min_price);
//                 }
//                 auto p2 = record.get_category_discount(context.goods().type());
//                 if(p2){
//                     min_price = std::min(p2->price(context),min_price);
//                 }
//                 final_price = min_price;
//                 init_price = true;
//                 return min_price;
//             }else{
//                 return context.num() * final_price;
//             }
//         }
//     private:
//         bool init_price;
//         double final_price;
// };
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
            // if(p1!=N)
            if(p1){
                min_price = std::min(p1->price(context),min_price);
            }
            auto p2 = record.get_category_discount(context.goods().type());   
            if(p2){
                min_price = std::min(p2->price(context),min_price);
            }
            return min_price;
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
// class GoodsFactory{
//     public:
//         static std::shared_ptr<Goods> get_goods(const GoodsData& data);
// };


