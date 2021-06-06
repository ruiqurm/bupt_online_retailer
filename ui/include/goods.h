#pragma once
#include<string>
#include<vector>
#include<memory>
#include<ctime>
#include<set>
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
        explicit GoodsData(const string&name,double price,int seller,int type,int remain,const string &description):
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
        // bool buy(User*u,int num);
       double get_price(const GoodsContext&);

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
        // bool update(map<string,>)
        pGoodsVec get_user_goods(int seller_id);
        pGoodsVec get_all_goods();
        // https://stackoverflow.com/questions/28888375/run-a-query-with-a-limit-offset-and-also-get-the-total-number-of-rows


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
        void remove(int id)override; 
        pGoodsVec get(const std::vector<int>&l);
    protected:
        // using _remove= MetaRecord<Goods,GoodsData>::remove;
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


class GoodsContext{
    public:
        GoodsContext(Goods* g,int num,User* u):
            _num(num),p_goods(g),p_user(u){ }
        User& user()const{return *p_user;}
        Goods& goods()const{return *p_goods;}
        int num()const{return _num;}
        int&num(){return _num;}
        void add_price(double p){
            total_price += p;
        }
    private:
        // std::map<int,std::pair<int,double>> price;
        double total_price;
        int _num;
        Goods* p_goods;
        User* p_user;
};//购买上下文

class Discount{
    public:
        enum{
            DISCOUNT, /*单品打折*/
            OFF, /*单品满减*/
            /*预留*/

            TYPE_DISCOUNT = 16, /*同品打折*/
            /*TYPE_OFF, *//*同品满减*/
            /*预留*/

            TOTAL_DISCOUNT = 32,/*全单打折*/
            TOTAL_OFF /*全单满减*/
        };
        Discount(int id,unsigned char type,int operand,double discount,double threshold=-1):
            _id(id),_type(type),_operand(operand),_discount(discount),_threshold(threshold){}
    virtual double price(const GoodsContext&context)=0;//鉴定客户的折扣价格
        int id(){return _id;}
        double discount(){return _discount;}
        int operand(){return _operand;}
        double threshold(){return _threshold;}
        unsigned char type(){return _type;}
    protected:
        int _id;
        unsigned char _type;
        int _operand;//操作数
        double _discount;//打折数或者满减数
        double _threshold;//阈值
};//打折策略

typedef std::unique_ptr<std::vector<std::shared_ptr<Discount>>> pDisVec;
class DiscountSimple:public Discount{
    public:
        DiscountSimple(int id,unsigned char type,int operand,double discount,double threshold=-1):
            Discount(id,type,operand,discount,threshold){}
        double price(const GoodsContext&context)override{
            if ( (_threshold<=0) || (_threshold>0&& context.goods().price() * context.num() >_threshold)){
                return context.goods().price() *context.num()*  _discount;
            }else{
                return context.goods().price()*context.num();
            }
        }
};

class DiscountCategory:public Discount{
    public:
        DiscountCategory(int id,unsigned char type,int operand,double discount,double threshold=-1):
            Discount(id,type,operand,discount,threshold){}
        double price(const GoodsContext&context)override{
            if ((_threshold>0&&context.goods().price()*context.num()>_threshold) || (_threshold<=0)){
                return context.goods().price() *context.num()* _discount;
            }else{
                return context.goods().price()*context.num();
            }
        }
};

class DiscountRecord{
    public:
        static DiscountRecord& get_record(){
            static DiscountRecord record;
            return record;
        }
        DiscountRecord(const DiscountRecord&)=delete;//禁止拷贝造
        
        //创建一条折扣
        bool make_discount(unsigned char type,int operand,double discount,double threshold=-1);
        void update_discount(int id,double discount,double thresold);
        std::shared_ptr<Discount> get_category_discount(int category);
        std::shared_ptr<Discount> get_goods_discount(int goods_id);
        pDisVec get_all_category_discount();
        void remove_by_goods(int goods_id);
        void remove(int id);
    protected:
        DiscountRecord();
        static int fetch_to_object(void*_data, int argc, char **argv, char **azColName);
        static int fetch_to_vector(void*_data, int argc, char **argv, char **azColName);
    private:
        sqlite3 *db;
};