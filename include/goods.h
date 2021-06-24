#pragma once
#include<string>
#include<vector>
#include<memory>
#include<ctime>
#include<set>
#include"database.h"
#include"user.h"

using std::string;

/**
 * @brief 商品数据结构体
 * @details 提供商品数据的载体，用于数据库传送。
 */
class GoodsData{
    public:
        GoodsData(){}
        GoodsData(int id,const char name[],double price,int seller,int type,int remain,const char description[]):
            id(id),name(name),price(price),seller(seller),type(type),remain(remain),description(description){}
        GoodsData(const char name[],double price,int seller,int type,int remain,const char description[]):
            id(0),name(name),price(price),seller(seller),type(type),remain(remain),description(description){}
        explicit GoodsData(const string&name,double price,int seller,int type,int remain,const string &description):
            id(0),name(name),price(price),seller(seller),type(type),remain(remain),description(description){}
        string name;///< 商品名
        string description;///< 描述
        double price;///< 价格
        int seller;///< 商家的id
        int id;///< 商品id
        int type;///< 商品类型
        int remain;///< 剩余量
};

class GoodsContext;

/**
 * @brief 商品类
 */
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

        /**
         * @brief 商家名称
         * @return 获取商家名称
         */
        string seller_name()const{
            auto& record = UserRecord::get_record();
            return record.get_username(data.seller);
        }

        ///@brief 获取商品类型
        int type()const{return data.type;}
        
        /**
         * @brief 保存商品数据
         * @details 禁止修改id
         * @return 是否修改成功
         */
        bool save();

        /**
         * @brief 购买该商品
         * 
         * @param u 用户对象
         * @param num 数量
         * @return 是否购买成功
         */
        bool buy(User*u,int num);

        /**
         * @brief 获取商品价格
         * @note 纯虚
         * @return 返回价格
         */
        virtual double get_price(const GoodsContext&)=0;
        
        /**
         * @brief 获取商品类型
         * @note 纯虚
         * @return 返回价格
         */    
        virtual int get_goods_type()const=0;

        ~Goods(){}
    protected:
        Goods(const GoodsData&data):data(data){}
        Goods(){}
        
        ///商品数据 @ref GoodsData "商品数据类"
        GoodsData data;
        
        ///类型
        unsigned short _TYPE;
};


/**
 * @brief 用于动态构造不同类型的商品的中间类
 * @details  
 * 关于CRTP，参见：参考：https://stackoverflow.com/questions/2850213/dynamically-register-constructor-methods-in-an-abstractfactory-at-compile-time-u
 * 
 * 例如，有一个叫A类型的Goods，那么，它的声明应该这样写：
 * @code
 * class A:public GoodsTemplate<1,A>{
 *   public:
 *       A(GoodsData* p):GoodsTemplate(p){}
 *       int get_goods_type()const override{
 *           return 1;
 *       }
 *       double get_price()override{
 *          return 0;
 *       }
 * //......
 * };
 * @endcode 
 * @tparam TYPE 一个正数，小于256，用于表示类型id
 * @tparam IMPL 不同类型的子类
 */
template<int TYPE, typename IMPL>
class GoodsTemplate: public Goods{

   enum {_GOODS_TYPE_ID = TYPE};
   public:
        /**
         * @brief 保存子类的构造函数，在父类调用构造的时候，会委托这个函数进行构造。
         * 
         * @param data 商品数据
         * @return 指向商品的智能指针
         */
        static std::shared_ptr<Goods> instance(const GoodsData&data) { return std::make_shared<IMPL>(data);}
        static const unsigned short GOODS_TYPE_ID; 

        /**
         * @brief 把基类指针转换成子类指针
         * 
         * @return std::shared_ptr<IMPL> 
         */
        static std::shared_ptr<IMPL> cast(std::shared_ptr<Goods>u){
            return std::dynamic_pointer_cast<IMPL>(u);
        }
   protected:
      GoodsTemplate(const GoodsData&data):Goods(data) {
          _TYPE = GOODS_TYPE_ID; 
      } 

};



/// 声明表名
template <>
constexpr char MetaRecord<Goods,GoodsData>::TABLE_NAME[];

/**
 * @brief 商品数据库操作
 * @details 提供商品CURD的接口
 */
class GoodsRecord: public MetaRecord<Goods,GoodsData>{
    public:
        /// 指向goods vector的智能指针 类型 
        typedef std::unique_ptr<std::vector<std::shared_ptr<Goods>>> pGoodsVec;

        /// goods的构造函数 函数指针
        typedef std::shared_ptr<Goods> (*p_goods_construct)(const GoodsData&data);

        static GoodsRecord& get_record(){
            static GoodsRecord record;///> 单例模式
            return record;
        }

        GoodsRecord(const GoodsRecord&)=delete;//禁止拷贝构造
        
        /**
         * @brief 获取指定id的商品
         * 
         * @param id 商品id
         * @return 指向商品的智能指针
         */
        std::shared_ptr<Goods> get(int id);

        /**
         * @brief 更新商品
         * @details 根据商品数据中的商品id更改数据，如果id小于0，那么更改会被取消
         * @param g 商品数据
         * @return 是否更改成功
         */
        bool update(const GoodsData&g);
        // bool update(map<string,>)

        /**
         * @brief 获取用户创建的商品
         * 
         * @param 用户id
         * @return 商品序列的智能指针
         */
        pGoodsVec get_user_goods(int seller_id);

        /**
         * @brief 获取所有商品
         * 
         * @return 商品序列的智能指针
         */
        pGoodsVec get_all_goods();

        /**
         * @brief 按输入的字符串搜索.采用模糊搜索
         * @details 匹配名字，描述中包含该字符串的商品
         * @param search_for_name 字符串
         * @return pGoodsVec 商品序列的智能指针
         */
        pGoodsVec get_goods_by_name(const string& search_for_name);

        /**
         * @brief 清空商品数据
         * @details 这个函数仅用于测试
         */
        void clear(){
            static const char sql[] = "DELETE FROM %s";
            static char buffer[128];
            sprintf(buffer,sql,this->TABLE_NAME);
            Database::exec(db,buffer,nullptr,nullptr);
        }
        
        /**
         * @brief 动态注册商品类，只在程序初始化的时候运行
         * @details 把商品类的构造函数保存到一个数组里；此处为了方便起见，用的是256单位的静态函数指针数组 
         * @param id 商品类id
         * @param factoryMethod 工厂函数
         * @see UserTemplate::instance
         * @return unsigned short 
         */
        unsigned short static register_type(unsigned short id, p_goods_construct factoryMethod){
            GoodsRecord::register_types[id] =factoryMethod;
            return id;
        }

        /**
         * @brief 移除一个商品
         * 
         * @param id 商品id
         */
        void remove(int id)override;        
    protected:
        // using _remove= MetaRecord<Goods,GoodsData>::remove;
        GoodsRecord();
        
        /**
         * @brief 把Goodsdata数据打印到字符串中
         * @details 这个函数被基类调用 @ref MetaRecord::set 
         * 
         * @param buffer 字符指针，执向字符数组
         * @param data 商品数据
         */
        void insert_data_to_string(char buffer[],const GoodsData& data){
            static const char sql[] =  "INSERT INTO %s (NAME,PRICE,SELLER,TYPE,REMAIN,DESCRIPTION) VALUES ('%s', %f, %d ,%d,%d,'%s'); ";
            snprintf(buffer,512,sql,TABLE_NAME,data.name.c_str(),data.price,data.seller,data.type,data.remain,data.description.c_str());
        }

        /**
         * @brief sqlite调用的回调函数。用于把从数据库获取到的字符串数据转化成相应类型，然后放进GoodsData中
         * 
         * @param _data 指向GoodsData的指针
         * @param argc sqlite返回了多少个数据
         * @param argv sqlite返回数据的字符数组
         * @param azColName 列名
         * @return int 
         */
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

        /**
         * @brief sqlite调用的回调函数。这个函数用于把字符串数据转化后放进vector中
         * 
         * @param _data 指向 @ref GoodsRecord::pGoodVec "商品vector的指针"
         * @param argc sqlite返回了多少个数据
         * @param argv sqlite返回数据的字符数组
         * @param azColName 列名
         * @return int 
         */
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

/// 动态为所有继承的商品子类注册类型
template <int TYPE, typename IMPL>
const unsigned short GoodsTemplate<TYPE, IMPL>::GOODS_TYPE_ID = GoodsRecord::register_type(
    GoodsTemplate<TYPE, IMPL >::_GOODS_TYPE_ID, &GoodsTemplate<TYPE, IMPL >::instance);


/**
 * @brief 订单数据
 */
class TransactionData{
    public:
        TransactionData(){}
        TransactionData(int id,int from,int to,string goodsName,int goodsID,double volume,double price,int count,time_t timestamp):
                        id(0),from(from),to(to),goodsName(goodsName),goodsID(goodsID),volume(volume),price(price),count(count),timestamp(timestamp){}
        TransactionData(int from,int to,string goodsName,int goodsID,double volume,double price,int count):
                        TransactionData(0,from,to,goodsName,goodsID,volume,price,count,0){}
        int id;///> 订单id
        std::string  goodsName;///> 商品名
        double price;///> 交易单价
        double volume;///> 总价
        time_t timestamp;///> 时间戳
        int from;///> 卖方id
        int to;///> 买方id
        int count;///> 交易量
        int goodsID;///> 商品id
};

/**
 * @brief 订单类
 * @details 提供购买信息的简单记录
 */
class Transaction{
    public:
        Transaction(int id,int from,int to,string goodsName,int goodsID,double volume,double price,int count,time_t timestamp):
                    data(id,from,to,goodsName,goodsID,volume,price,count,timestamp){}
        explicit Transaction(const TransactionData& data):data(data){}
        static int make_transaction(int from,int to,string goodsName,int goodsID,double volume,double price,int count);
        Transaction(){}
        const int id()const{return data.id;}

        /**
         * @brief 获取卖方用户类
         * 
         * @return 卖方用户智能指针
         */
        std::shared_ptr<User> from()const{
            auto& record = UserRecord::get_record();
            return record.create_user(record.get(data.from));
        }
        /**
         * @brief 获取买方用户类
         * 
         * @return 买方用户智能指针 
         */
       std::shared_ptr<User> to()const{
            auto& record = UserRecord::get_record();
            return record.create_user(record.get(data.to));
        }
        /// 商品名
        const string& goodsName()const{return data.goodsName;}

        /// 获取商品智能指针
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
        TransactionData data;// 订单数据
};

template <>
constexpr char MetaRecord<Transaction,TransactionData>::TABLE_NAME[];

/**
 * @brief 交易订单数据库接口
 * 
 */
class TransactionRecord: public MetaRecord<Transaction,TransactionData>{
    public:
        /// 指向订单对象的vector的智能指针 类型 
        typedef std::unique_ptr<std::vector<std::shared_ptr<Transaction>>> pTransVec;
        static TransactionRecord& get_record(){
            static TransactionRecord record;
            return record;
        }
        TransactionRecord(const TransactionRecord&)=delete;//禁止拷贝构造
        
        /**
         * @brief 获取指定id的订单
         * 
         * @param id 订单id
         * @return 指向订单的智能指针
         */
        std::shared_ptr<Transaction> get(int id);

        /**
         * @brief 获取指定用户的订单
         * 
         * @param user_id 用户id
         * @return pTransVec 指向订单vector的智能指针
         */
        pTransVec get_user_transactions(int user_id);

    protected:
        TransactionRecord();

        /**
         * @brief 把transaction数据打印到字符串中
         * @details 这个函数被基类  调用
         * @see MetaRecord::set
         * @param buffer 字符指针，执向字符数组
         * @param data 商品数据
         */        
        void insert_data_to_string(char buffer[],const TransactionData& data){
            static const char sql[] =  "INSERT INTO %s (_FROM,_TO,GOODSNAME,GOODSID,VOLUME,PRICE,COUNT,TIMESTAMP) VALUES"\
                                                      "(%d, %d ,'%s',%d, %f, %f ,%d,%d); ";
            snprintf(buffer,512,sql,TABLE_NAME,data.from,data.to,data.goodsName.c_str(),data.goodsID,data.volume,data.price,data.count,(int)time(nullptr));

        }

        /**
         * @brief sqlite调用的回调函数。用于把从数据库获取到的字符串数据转化成相应类型，然后放进TransactionData中
         * 
         * @param _data 指向transaction的指针
         * @param argc sqlite返回了多少个数据
         * @param argv sqlite返回数据的字符数组
         * @param azColName 列名
         * @return int 
         */
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

        /**
         * @brief sqlite调用的回调函数。这个函数用于把字符串数据转化后放进vector中
         * 
         * @param _data 指向transaction的vector的指针
         * @param argc sqlite返回了多少个数据
         * @param argv sqlite返回数据的字符数组
         * @param azColName 列名
         * @return int 
         */
        static int fetch_in_vector(void*_data, int argc, char **argv, char **azColName){
            auto& vec = *(std::vector<std::shared_ptr<Transaction>>*)_data;
            vec.emplace_back(std::make_shared<Transaction>(atoi(argv[0]),atoi(argv[2]),atoi(argv[3]),argv[1],atoi(argv[4]),atof(argv[5]),atof(argv[6]),atoi(argv[7]),atoi(argv[8])));
            return 0;
        }
};

/**
 * @brief 购买时上下文
 * 
 */
class GoodsContext{
    public:
        GoodsContext(Goods* g,int num,User* u):
            _num(num),p_goods(g),p_user(u){ }
        
        /// 返回买方指针
        User& user()const{return *p_user;}
        /// 返回商品指针
        Goods& goods()const{return *p_goods;}
        
        /// 返回购买数量
        int num()const{return _num;}
        int&num(){return _num;}
        
        /// 未用，可以用来统计订单价格
        void add_price(double p){
            total_price += p;
        }
    private:
        // std::map<int,std::pair<int,double>> price;
        double total_price;
        int _num;
        Goods* p_goods;
        User* p_user;
};

/**
 * @brief 折扣类
 * @details 折扣的格式类似指令，由 类型，操作数，效果，阈值三个部分组成
 * 例如，单品打折 商品1 7折 无阈值可以写作：
 * DISCOUNT 1 0.7 -1
 */
class Discount{
    public:
        enum{
            DISCOUNT, /**单品打折*/
            OFF, /**单品满减，未用*/
            /*预留*/

            TYPE_DISCOUNT = 16, /**同品打折*/
            TYPE_OFF, /**同品满减，未用*/
            /*预留*/

            TOTAL_DISCOUNT = 32,/**全单打折*/
            TOTAL_OFF /**全单满减，未用*/
        };
        Discount(int id,unsigned char type,int operand,double discount,double threshold=-1):
            _id(id),_type(type),_operand(operand),_discount(discount),_threshold(threshold){}
    virtual double price(const GoodsContext&context)=0;//鉴定客户的折扣价格
        /// id
        int id(){return _id;}
        
        /// 折扣
        double discount(){return _discount;}

        /// 操作数
        int operand(){return _operand;}

        /// 阈值 -1表示无限大
        double threshold(){return _threshold;}

        /// 类型        
        unsigned char type(){return _type;}
    protected:
        int _id;///> id
        unsigned char _type;///> 类型
        int _operand;///> 操作数
        double _discount;///> 打折数或者满减数
        double _threshold;///> 阈值
};

/// 指向discount vector的智能指针
typedef std::unique_ptr<std::vector<std::shared_ptr<Discount>>> pDisVec;

/**
 * @brief 单品打折类
 * 
 */
class DiscountSimple:public Discount{
    public:
        DiscountSimple(int id,unsigned char type,int operand,double discount,double threshold=-1):
            Discount(id,type,operand,discount,threshold){}
        /**
         * @brief 计算单品打折价格
         * 
         * @param context 上下文
         * @return double 价格
         */
        double price(const GoodsContext&context)override{
            if ( (_threshold<=0) || (_threshold>0&& context.goods().price() * context.num() >_threshold)){
                return context.goods().price() *context.num()*  _discount;
            }else{
                return context.goods().price()*context.num();
            }
        }
};

/**
 * @brief 品类打折类
 * 
 */
class DiscountCategory:public Discount{
    public:
        DiscountCategory(int id,unsigned char type,int operand,double discount,double threshold=-1):
            Discount(id,type,operand,discount,threshold){}
        
        /**
         * @brief 计算品类打折价格
         * 
         * @param context 上下文
         * @return double 价格
         */
        double price(const GoodsContext&context)override{
            if ((_threshold>0&&context.goods().price()*context.num()>_threshold) || (_threshold<=0)){
                return context.goods().price() *context.num()* _discount;
            }else{
                return context.goods().price()*context.num();
            }
        }
};

/**
 * @brief Discount数据库接口
 * @details 这个类不再继承自 @ref MetaRecord "MetaRecord"，因为此时发现原本的 @ref MetaRecord "MetaRecord" 设计得并不好，倒不如直接写
 */
class DiscountRecord{
    public:
        static DiscountRecord& get_record(){
            static DiscountRecord record;
            return record;
        }
        DiscountRecord(const DiscountRecord&)=delete;//禁止拷贝造
        
        /**
         * @brief 新建一条折扣
         * 
         * @param type 类型
         * @param operand 操作数
         * @param discount 折扣
         * @param threshold 阈值
         * @return 是否创建成功
         */
        bool make_discount(unsigned char type,int operand,double discount,double threshold=-1);

        /**
         * @brief 更新折扣
         * 
         * @param id 折扣id
         * @param discount 折扣数
         * @param thresold 阈值
         */
        void update_discount(int id,double discount,double thresold);

        /**
         * @brief 获取一个品类的折扣
         * 
         * @param category 品类id
         * @return 指向折扣的智能指针
         */
        std::shared_ptr<Discount> get_category_discount(int category);

        /**
         * @brief 获取一个商品的折扣
         * 
         * @param goods_id 商品id
         * @return 指向商品折扣的智能指针
         */
        std::shared_ptr<Discount> get_goods_discount(int goods_id);

        /**
         * @brief 获取所有品类的折扣
         * 
         * @return 指向折扣数组的智能指针
         */
        pDisVec get_all_category_discount();

        /**
         * @brief 根据商品id移除
         * @details 这里数据库用的不是外键类型，而是普通的类型，因此商品删除时，这里不会跟着删除，需要手动删除它
         * @param goods_id 商品id
         */
        void remove_by_goods(int goods_id);

        /**
         * @brief 移除一条折扣
         * 
         * @param id 
         */
        void remove(int id);
    protected:
        DiscountRecord();

        /**
         * @brief sqlite调用的回调函数。用于把从数据库获取到的字符串数据转化成相应类型，然后放进Discount中
         * 
         * @param _data Discount指针
         * @param argc sqlite返回了多少个数据
         * @param argv sqlite返回数据的字符数组
         * @param azColName 列名
         * @return int 
         */
        static int fetch_to_object(void*_data, int argc, char **argv, char **azColName);

        /**
         * @brief sqlite调用的回调函数。用于把从数据库获取到的字符串数据转化成相应类型，然后放进Discount 数组中
         * 
         * @param _data Discount数组的智能指针
         * @param argc sqlite返回了多少个数据
         * @param argv sqlite返回数据的字符数组
         * @param azColName 列名
         * @return int 
         */
        static int fetch_to_vector(void*_data, int argc, char **argv, char **azColName);
    private:
        sqlite3 *db;
};