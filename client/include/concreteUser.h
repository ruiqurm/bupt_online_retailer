/**
 * @file concreteUser.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 用户派生类
 */
#pragma once
#include"user.h"
#include"goods.h"
#include"transaction.h"
#include<map>
#include<vector>
#include<string>

namespace USER_TYPE{
enum{
    /// 消费者类型
    customer,

    /// 商家类型
    seller
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
        int add_goods(GoodsData g){
            auto& record = GoodsRecord::get_record();
            g.seller = data->id;
            return record.set(g);
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

        /**
         * @brief 购买
         * @details 支持同时生成多张订单（购买不同商家的物品）
         * 商家不能购买自己的物品
         * @param cart 购物车
         * @param selection 选择的购物车项
         * @return std::tuple<std::string,std::vector<int>>  生成的订单号 + 错误信息（如果有的话）
         */
        std::tuple<std::string,std::vector<int>>  buy(Cart&cart,std::vector<int>&selection);
        ~Seller(){}
        // int buy(std::vector<int> selection);
    private:
        std::unique_ptr<std::vector<std::shared_ptr<Goods>>>_goods; // 商家的商品
        bool has_load_goods; // 是否已经读取了商品(懒加载)
};

class Customer:public UserTemplate<USER_TYPE::customer,Customer>{
    public:
        Customer(UserData* p):UserTemplate(p){}

        /**
         * @brief 购买
         * @details 支持同时生成多张订单（购买不同商家的物品）
         * @param cart 购物车
         * @param selection 选择的购物车项
         * @return std::tuple<std::string,std::vector<int>>  生成的订单号 + 错误信息（如果有的话）
         */        
        std::tuple<std::string,std::vector<int>>  buy(Cart&cart,std::vector<int>&selection);

        /**
         * @brief 获取用户类型
         * 
         * @return 一个数字，根据USER_TYPE表示类型
         */            
        int get_user_type()const override{
            return _TYPE;
        }
        
        ~Customer(){}

};
