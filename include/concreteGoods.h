#pragma once
#include"goods.h"

namespace GOODS_TYPE{
    enum{
        Books,
        Clothes
    };
}
class Books:public GoodsTemplate<GOODS_TYPE::Books,Books>{
    public:
        Books(const GoodsData&data):GoodsTemplate(data){}
        double get_price(const GoodsContext& context)override{
            return data.price*context.num();
        }
        int get_goods_type()const override{
            return _TYPE;
        }
};

class Clothes:public GoodsTemplate<GOODS_TYPE::Clothes,Clothes>{
    public:
        Clothes(const GoodsData&data):GoodsTemplate(data){}
        double get_price(const GoodsContext& context)override{
            return data.price*context.num();
        }
        int get_goods_type()const override{
            return _TYPE;
        }
};
// class GoodsFactory{
//     public:
//         static std::shared_ptr<Goods> get_goods(const GoodsData& data);
// };


