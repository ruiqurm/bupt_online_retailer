#include"concreteGoods.h"
#include"test.h"
#include"concreteUser.h"
#include<iostream>
#define DEBUG 1
using namespace std;

int main(int argc,char**argv){
    // database_clean::parse_argument(argc,argv);
    try
    {
        /**
         * 
         * 测试交易对象
         * 
         */
        TransactionData datas[10] {{1,2,"aaa",1,25.0,2,15},
                            {1,2,"bbb",2,30.0,3,10},    
                            {1,2,"ccc",2,60.0,3,10},    
                            {1,2,"ddd",2,20.0,3,10},    
                            {1,2,"eee",2,50.0,3,10},                                        
                            };
        {
            auto& record = TransactionRecord::get_record();
            record.clear();
            int t = 0;
            for(int i=0;i<5;i++)
                t = record.set(datas[i]);
            ASSERT(record.size()==5,"插入失败");
            auto it = record.get(t);
            cout<<"it->goodsName: "<<it->goodsName()<<endl;
            ASSERT(it->goodsName()==string("eee"),"名字错误");
            ASSERT(!it->from(),"错误");
            User::register_(USER_TYPE::seller,"111111","111111");
            cout<<it->from()->username()<<endl;
            ASSERT(it->from()->username()==string("111111"),"错误");
        }
        {
            cout<<"第二个测试"<<endl;
            auto& u = UserRecord::get_record();
            u.clear();
            auto& goodsRecord = GoodsRecord::get_record();
            goodsRecord.clear();
            int seller_id = User::register_(USER_TYPE::seller,"aaaa","111111");
            int consumer_id = User::register_(USER_TYPE::customer,"bbbb","111111");

            int good_id = goodsRecord.set(GoodsData {"c++ primer",2.0,seller_id,GOODS_TYPE::Books,10,"c++:从入门到入土"});
            auto goods = goodsRecord.get(good_id);
            cout<<"Goods: "<<goods->name()<<endl;
            auto seller = std::dynamic_pointer_cast<Seller>(User::login("aaaa","111111"));
            cout<<"seller: "<<seller->username()<<endl;
            auto consumer = std::dynamic_pointer_cast<Customer>(User::login("bbbb","111111"));
            cout<<"consumer: "<<seller->username()<<endl;            
            consumer->balance() +=1000;
            consumer->save();
            cout<<"尝试交易"<<endl;
            ASSERT( (consumer->buy(*goods,1)) == true,"购买失败");
            cout<<"consumer余额"<<consumer->balance()<<endl;
            ASSERT(consumer->balance()==998,"余额出错");
            cout<<"seller余额"<<seller->balance()<<endl;
            if (!(u.get("aaaa")->balance==seller->balance())){
                cout<<"user信息无法同步"<<endl;
            }      
            ASSERT(seller->balance()==2,"余额出错");
            consumer->balance() = 0;
            consumer->save();
            // cout<<""<<endl;
        }
    }catch(const char*error){
        cerr<<error;
        return -1;
    }catch(const exception& e){
        cerr<<e.what()<<endl;
        return -1;
    }
    cout<<"测试完成"<<endl;
    return 0;
//    database_clean::remove_database();
}