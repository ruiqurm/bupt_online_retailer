#pragma once
#include<string>
#include<vector>
#include<memory>
#include<ctime>
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
class GoodsContext;
class Goods{
    public:
        
        const int id(){return data.id;}
        string& name(){return data.name;}
        const string& name()const{return data.name;}
        string& description(){return data.description;}
        const string& description()const{return data.description;}
        int& remain(){return data.remain;}
        int remain()const{return data.remain;}
        double& price(){return data.price;}
        double price()const{return data.price;}
        int seller()const{return data.seller;}
        string seller_name()const{
            auto& record = UserRecord::get_record();
            return record.get_username(data.seller);
        }
        int type()const{return data.type;}
        
        bool save();
        bool buy(User*u,int num);
        virtual double get_price(const GoodsContext&)=0;

        virtual int get_goods_type()const=0;

        ~Goods(){}
    protected:
        Goods(const GoodsData&data):data(data){}
        Goods(){}
        GoodsData data;
        unsigned short _TYPE;
};

template<int TYPE, typename IMPL>
class GoodsTemplate: public Goods{
    // 用于向Manager注册类
    // CRTP
    // 参考：https://stackoverflow.com/questions/2850213/dynamically-register-constructor-methods-in-an-abstractfactory-at-compile-time-u
   
   enum {_GOODS_TYPE_ID = TYPE};
   public:
        static std::shared_ptr<Goods> instance(const GoodsData&data) { return std::make_shared<IMPL>(data);}
        static const unsigned short GOODS_TYPE_ID; 
        static std::shared_ptr<IMPL> cast(std::shared_ptr<Goods>u){
            return std::dynamic_pointer_cast<IMPL>(u);
        }
        // static void Enable() { volatile uint16_t x =  }
   protected:
      GoodsTemplate(const GoodsData&data):Goods(data) {
          _TYPE = GOODS_TYPE_ID; 
      } 

};




template <>
constexpr char MetaRecord<Goods,GoodsData>::TABLE_NAME[];

class GoodsRecord: public MetaRecord<Goods,GoodsData>{
    public:
        typedef std::unique_ptr<std::vector<std::shared_ptr<Goods>>> pGoodsVec;
        typedef std::shared_ptr<Goods> (*p_goods_construct)(const GoodsData&data);
        static GoodsRecord& get_record(){
            static GoodsRecord record;
            return record;
        }
        GoodsRecord(const GoodsRecord&)=delete;//禁止拷贝构造
        
        std::shared_ptr<Goods> get(int id);
        bool update(const GoodsData&);
        pGoodsVec get_user_goods(int seller_id);
        pGoodsVec get_all_goods();
        pGoodsVec get_goods_by_name(const string& search_for_name);
        //按输入的字符串搜索.采用模糊搜索
        void clear(){
            static const char sql[] = "DELETE FROM %s";
            static char buffer[128];
            sprintf(buffer,sql,this->TABLE_NAME);
            Database::exec(db,buffer,nullptr,nullptr);
        }
        unsigned short static register_type(unsigned short id, p_goods_construct factoryMethod){
            GoodsRecord::register_types[id] =factoryMethod;
            return id;
        }
        
    protected:
        GoodsRecord();

        void insert_data_to_string(char buffer[],const GoodsData& data){
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
            vec.push_back(register_types[data.type](data));
            return 0;
        }
        

    
    private:
        const UserRecord& manager;
        static p_goods_construct register_types[256];
};

template <int TYPE, typename IMPL>
const unsigned short GoodsTemplate<TYPE, IMPL>::GOODS_TYPE_ID = GoodsRecord::register_type(
    GoodsTemplate<TYPE, IMPL >::_GOODS_TYPE_ID, &GoodsTemplate<TYPE, IMPL >::instance);


class TransactionData{
    public:
        TransactionData(){}
        TransactionData(int id,int from,int to,string goodsName,int goodsID,double volume,double price,int count,time_t timestamp):
                        id(0),from(from),to(to),goodsName(goodsName),goodsID(goodsID),volume(volume),price(price),count(count),timestamp(timestamp){}
        TransactionData(int from,int to,string goodsName,int goodsID,double volume,double price,int count):
                        TransactionData(0,from,to,goodsName,goodsID,volume,price,count,0){}
        int id;
        std::string  goodsName;
        double price;//交易单价
        double volume;//总价
        time_t timestamp;//时间戳
        int from;
        int to;
        int count;//交易量
        int goodsID;
};

class Transaction{
    public:
        Transaction(int id,int from,int to,string goodsName,int goodsID,double volume,double price,int count,time_t timestamp):
                    data(id,from,to,goodsName,goodsID,volume,price,count,timestamp){}
        explicit Transaction(const TransactionData& data):data(data){}
        static int make_transaction(int from,int to,string goodsName,int goodsID,double volume,double price,int count);
        Transaction(){}
        const int id()const{return data.id;}
        std::shared_ptr<User> from()const{
            auto& record = UserRecord::get_record();
            return record.create_user(record.get(data.from));
        }
       std::shared_ptr<User> to()const{
            auto& record = UserRecord::get_record();
            return record.create_user(record.get(data.to));
        }
        const string& goodsName()const{return data.goodsName;}
        std::shared_ptr<Goods> goods()const;
        const double volume()const{
            return data.volume;
        }
        const double price()const{
            return data.price;
        }
        const int count()const{
            return data.count;
        }
        const time_t time()const{
            return data.timestamp;
        }
    private:
        TransactionData data;
};
template <>
constexpr char MetaRecord<Transaction,TransactionData>::TABLE_NAME[];

class TransactionRecord: public MetaRecord<Transaction,TransactionData>{
    public:
        typedef std::unique_ptr<std::vector<std::shared_ptr<Transaction>>> pTransVec;
        static TransactionRecord& get_record(){
            static TransactionRecord record;
            return record;
        }
        TransactionRecord(const TransactionRecord&)=delete;//禁止拷贝构造
        std::shared_ptr<Transaction> get(int id);
        pTransVec get_user_transactions(int user_id);
    protected:
        TransactionRecord();

        void insert_data_to_string(char buffer[],const TransactionData& data){
            static const char sql[] =  "INSERT INTO %s (_FROM,_TO,GOODSNAME,GOODSID,VOLUME,PRICE,COUNT,TIMESTAMP) VALUES"\
                                                      "(%d, %d ,'%s',%d, %f, %f ,%d,%d); ";
            snprintf(buffer,512,sql,TABLE_NAME,data.from,data.to,data.goodsName.c_str(),data.goodsID,data.volume,data.price,data.count,(int)time(nullptr));

        }

        static int fetch_in_struct(void*_data, int argc, char **argv, char **azColName){
            TransactionData* data = (TransactionData*)_data;
            data->id = atoi(argv[0]);
            data->goodsName.assign(argv[1]);
            data->from = atoi(argv[2]);
            data->to = atoi(argv[3]);
            data->goodsID = atoi(argv[4]);
            data->volume = atof(argv[5]);
            data->price = atof(argv[6]);
            data->count = atoi(argv[7]);
            data->timestamp = atoi(argv[8]);
            return 0;
        }
        static int fetch_in_vector(void*_data, int argc, char **argv, char **azColName){
            auto& vec = *(std::vector<std::shared_ptr<Transaction>>*)_data;
            vec.emplace_back(std::make_shared<Transaction>(atoi(argv[0]),atoi(argv[2]),atoi(argv[3]),argv[1],atoi(argv[4]),atof(argv[5]),atof(argv[6]),atoi(argv[7]),atoi(argv[8])));
            return 0;
        }
};


class GoodsContext{
    public:
        GoodsContext(const Goods* g,const int num,const User* u):
            _num(num),p_goods(g),p_user(u){}
        const User& user()const{return *p_user;}
        const Goods& goods()const{return *p_goods;}
        const int num()const{return _num;}
    private:
        const Goods* p_goods;
        const User* p_user;
        const int _num;
};//购买上下文
