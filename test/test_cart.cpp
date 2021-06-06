#include"concreteGoods.h"
#include"test.h"
#include"concreteUser.h"
#include"transaction.h"

int main(){
        User::register_(USER_TYPE::seller,"seller1","123456");
    auto seller1 =  Seller::cast(User::login("seller1","123456"));
    seller1->balance()+=100000;
    seller1->save();
    User::register_(USER_TYPE::customer,"customer","123456");
    auto customer = Customer::cast(User::login("customer","123456"));
    GoodsData goods[5]{{"c++ primer",100,seller1->id(),GOODS_TYPE::Books,10,"c++:从入门到入土"},
                        {"Effective C++",100,seller1->id(),GOODS_TYPE::Books,10,"c++:从入门到入土"},
                        {"More Effective C++",100,seller1->id(),GOODS_TYPE::Books,10,"c++:从入门到入土"},
                        {"卫衣",100,seller1->id(),GOODS_TYPE::Clothes,20,"一件普通的卫衣"},
                        {"T-shirt",100,seller1->id(),GOODS_TYPE::Clothes,10,"一件普通的T-shirt"}};
    int goods1_id = seller1->add_goods(goods[0]);
    int goods2_id = seller1->add_goods(goods[1]);
    int goods3_id = seller1->add_goods(goods[2]);
    int goods4_id = seller1->add_goods(goods[3]);
    int goods5_id = seller1->add_goods(goods[4]);
    Cart cart(customer->id());
    cart.add(goods1_id,6);
    cart.add(goods2_id,2);
    cart.add(goods5_id,2);
    {
        auto&d = cart.data();
        ASSERT(d.find(goods1_id)->second == 6,"内部数据出错1");
        ASSERT(d.find(goods2_id)->second == 2,"内部数据出错2");
        ASSERT(d.find(goods5_id)->second == 2,"内部数据出错3");
    }
    cart.reduce(goods1_id,7);
    cart.reduce(goods1_id,1);
    cart.reduce(goods2_id,1);
    try
    {
        auto&d = cart.data();
        ASSERT(d.find(goods1_id)==d.end(),"内部数据出错4");
        ASSERT(d.find(goods2_id)->second == 1,"内部数据出错5");
    }catch(const char*str){
        puts(str);
        exit(EXIT_FAILURE);
    }
    {
        std::vector<int> v{1,2,3,4,5};
        auto &record = GoodsRecord::get_record();
        auto p = record.get(v);
        for(auto&i:*p){
            std::cout<<i->name()<<std::endl;
        }
        std::cout<<"------------"<<std::endl;
        std::vector<int> v2{2};
        auto p2 = record.get(v2);
        for(auto&i:*p2){
            std::cout<<i->name()<<std::endl;
        }
    }
    std::cout<<"完成"<<std::endl;
    return 0;
}