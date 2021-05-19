#pragma once
#include<string>
#include<vector>
#include<memory>
#include"database.h"
#include"user.h"

using std::string;

class GoodsData{
    public:
        GoodsData(){}
        GoodsData(int id,const char name[],double price,int seller,int type,int remain,const char description[]):
            id(id),name(name),price(price),seller(seller),type(type),remain(remain),description(description){}
        GoodsData(const char name[],double price,int seller,int type,int remain,const char description[]):
            id(0),name(name),price(price),seller(seller),type(type),remain(remain),description(description){}
        string name;
        string description;
        double price;
        int seller;// primary key of seller
        int id;
        int type;
        int remain;
};

class Goods{
    public:
        string& name(){return data.name;}
        const string& name()const{return data.name;}
        string& description(){return data.description;}
        const string& description()const{return data.description;}
        int& remain(){return data.remain;}
        int remain()const{return data.remain;}
        double& price(){return data.price;}
        double price()const{return data.price;}

        int type()const{return data.type;}
        virtual double get_price()=0;//实际价格
        void save();
        // int get_id(){return id;}
        ~Goods(){}
    protected:
        Goods(const GoodsData&data):data(data){}
        Goods(){}
        GoodsData data;
};

class GoodsFactory{
    public:
        static std::shared_ptr<Goods> get_goods(const GoodsData& data);
};

template <>
constexpr char MetaRecord<Goods,GoodsData>::TABLE_NAME[] ="GOODS";

class GoodsRecord: public MetaRecord<Goods,GoodsData>{
    public:
        typedef std::unique_ptr<std::vector<std::shared_ptr<Goods>>> pGoodsVec;
        static GoodsRecord& get_record(){
            static GoodsRecord record;
            return record;
        }
        GoodsRecord(const GoodsRecord&)=delete;//禁止拷贝构造
        
        std::shared_ptr<Goods> get(int id);

        pGoodsVec get_user_goods(int seller_id);
        pGoodsVec get_all_goods();
    protected:
        GoodsRecord();

        void _data_to_string(char buffer[],const GoodsData& data){
            static const char sql[] =  "INSERT INTO %s (NAME,PRICE,SELLER,TYPE,REMAIN,DESCRIPTION) VALUES ('%s', %f, %d ,%d,%d,'%s'); ";
            snprintf(buffer,512,sql,TABLE_NAME,data.name.c_str(),data.price,data.seller,data.type,data.remain,data.description.c_str());
        }

        static int fetch_in_struct(void*_data, int argc, char **argv, char **azColName){
            GoodsData* data = (GoodsData*)_data;
            data->id = atoi(argv[0]);
            data->name.assign(argv[1]);
            data->price = atof(argv[2]);
            data->seller = atoi(argv[3]);
            data->type = atoi(argv[4]);
            data->remain = atoi(argv[5]);
            data->description.assign(argv[6]);
            return 0;
        }
        static int fetch_in_vector(void*_data, int argc, char **argv, char **azColName){
            GoodsData data(atoi(argv[0]),argv[1],atof(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),argv[6]);
            auto& vec = *(std::vector<std::shared_ptr<Goods>>*)_data;
            vec.push_back(GoodsFactory::get_goods(data));
            return 0;
        }
    private:
        const UserRecord& manager;
        
};


class TransactionData{
    public:
        TransactionData(){}
        TransactionData(int from,int to,double price,int count,time_t timestamp):
                        from(from),to(to),price(price),count(count),timestamp(timestamp){}
        double price;//交易单价
        time_t timestamp;//时间戳
        int from;
        int to;
        int count;//交易量
        
};