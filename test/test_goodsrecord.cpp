#include"goods.h"
#include"test.h"
#include<iostream>
using namespace std;

int main(int argc,char**argv){
    database_clean::parse_argument(argc,argv);
    {
        GoodsRecord record;
        GoodsData data{"aaaa",2.0,1};
        int x = record.set(data);
        cout<<"insert data into row "<<x<<endl;
        auto goods = record.get(x);
        ASSERT(goods.get_name()=="aaaa","获取数据出错");
        record.remove(x);
        cout<<"remove data from row "<<x<<endl;
    }
    database_clean::remove_database();
}