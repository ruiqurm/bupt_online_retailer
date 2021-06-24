#pragma once
#include"user.h"
#include"goods.h"
namespace USER_TYPE{
enum{
    customer,///< 消费者类型
    seller ///< 商家类型
};
}

/**
 * @brief 商家子类
 * @details 商家可以添加商品
 */
class Seller:public UserTemplate<USER_TYPE::seller,Seller>{
    public:
        Seller(UserData* p):UserTemplate(p),has_load_goods(false){}

        /**
         * @brief 获取用户类型
         * 
         * @return 一个数字，根据USER_TYPE表示类型
         */
        int get_user_type()const override{
            return _TYPE;
        }

        /**
         * @brief 添加一件商品
         * 
         * @param g 商品数据 @ref GoodsData "GoodsData类"
         */
        void add_goods(GoodsData g){
            auto& record = GoodsRecord::get_record();
            g.seller = data->id;
            record.set(g);
        }
        
        /**
         * @brief 查看商家管理的商品
         * @details 创建商家对象时，不会马上获取商品；而是等第一次获取后才会向数据库请求。返回的是引用。
         * @return 一个商品对象vector的列表的引用 @ref Goods "商品类型"
         */
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
        std::unique_ptr<std::vector<std::shared_ptr<Goods>>>_goods;// 商家的商品
        bool has_load_goods;// 是否已经读取了商品(懒加载)
};

/**
 * @brief 顾客子类
 * @details 顾客可以购买商品
 */
class Customer:public UserTemplate<USER_TYPE::customer,Customer>{
    public:
        Customer(UserData* p):UserTemplate(p){}
        int get_user_type()const override{
            return _TYPE;
        }
        /**
         * @brief 购买商品
         * @param Goods 商品 @ref Goods "Goods类" 
         * @param num 数量
         * @return int 购买金额
         */
        int buy(Goods&,int num);
        ~Customer(){}

};
