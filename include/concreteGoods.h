#include"goods.h"

namespace goods_type{
    enum{
        Books,
        Clothes
    };
}
class Books:public Goods{
    public:
        Books(const GoodsData&data):Goods(data){}
        double get_price()override{
            return data.price;
        }
};

class Clothes:public Goods{
    public:
        Clothes(const GoodsData&data):Goods(data){}
        double get_price()override{
            return data.price;
        }
};
std::shared_ptr<Goods> GoodsFactory::get_goods(const GoodsData& data){
    switch (data.type)
    {
    case goods_type::Books:
        return std::make_shared<Books>(data);
    case goods_type::Clothes:
        return std::make_shared<Clothes>(data);
    default:
        return nullptr;
    }
}

