/**
 * @file goods.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 商品数据，商品，折扣，交易上下文
 */
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
        /// id
        const int id(){return data.id;}
        
        /// 商品名
        string& name(){return data.name;}

        /// 商品名
        const string& name()const{return data.name;}

        /// 描述
        string& description(){return data.description;}

        /// 描述
        const string& description()const{return data.description;}

        /// 剩余量
        int& remain(){return data.remain;}

        /// 剩余量
        int remain()const{return data.remain;}

        /// 价格
        double& price(){return data.price;}

        /// 价格
        double price()const{return data.price;}

        /// 商家id
        int seller()const{return data.seller;}

        /// 商家名称
        string seller_name()const{
            auto& record = UserRecord::get_record();
            return record.get_username(data.seller);
        }

        /// 商品类型
        int type()const{return data.type;}
        
        /**
         * @brief 保存商品数据
         * @details 禁止修改id
         * @return 是否修改成功
         */
        bool save();
        // bool buy(User*u,int num);

        /**
         * @brief 获取商品价格
         * @note 纯虚
         * @return 返回价格
         */
        virtual double get_price(const GoodsContext&);

        /// 获取商品类型
        virtual int get_goods_type()const=0;

        ~Goods(){}
    protected:
        Goods(const GoodsData&data):data(data){}
        Goods(){}
        ///商品数据 @ref GoodsData "商品数据类"
        GoodsData data;
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



/**
 * @brief 商品数据库操作
 * @details 提供商品CURD的接口
 */
class GoodsRecord{
    public:
        /// 指向goods vector的智能指针 类型
        typedef std::unique_ptr<std::vector<std::shared_ptr<Goods>>> pGoodsVec;

        /// goods的构造函数 函数指针
        typedef std::shared_ptr<Goods> (*p_goods_construct)(const GoodsData&data);

        
        static GoodsRecord& get_record(){
            static GoodsRecord record;
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
         * @brief 获取id列表中的商品
         * 
         * @param id列表
         * @return 指向商品列表的智能指针
         */              
        pGoodsVec get(const std::vector<int>&l);

        /**
         * @brief 创建商品
         * @param g 商品数据
         * @return 商品id
         */        
        int set(GoodsData&data);

        /**
         * @brief 更新商品
         * @details 根据商品数据中的商品id更改数据，如果id小于0，那么更改会被取消
         * @param g 商品数据
         * @return 是否更改成功
         */
        bool update(const GoodsData&);

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
        void remove(int id); 

        
    protected:
        // using _remove= MetaRecord<Goods,GoodsData>::remove;
        /// 初始化，连接socket
        GoodsRecord():base(Database::get_database()),manager(UserRecord::get_record()){}
    
    private:
        Database* base;
        char send_buf[8192];///> 写缓存
        char recv_buf[8192];///> 读缓存，可以和写缓存合并

        const UserRecord& manager;
        static p_goods_construct register_types[256];
};

/// 动态为所有继承的商品子类注册类型
template <int TYPE, typename IMPL>
const unsigned short GoodsTemplate<TYPE, IMPL>::GOODS_TYPE_ID = GoodsRecord::register_type(
    GoodsTemplate<TYPE, IMPL >::_GOODS_TYPE_ID, &GoodsTemplate<TYPE, IMPL >::instance);



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
        /// 返回购买数量
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
};//购买上下文

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
        
        /// 价格
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
        int _id;
        unsigned char _type;///> id
        int _operand;///> 类型
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
        /// 初始化，链接database
        DiscountRecord():base(Database::get_database()){}
        Database* base;
        char send_buf[8192];
        char recv_buf[8192];

        // static int fetch_to_object(void*_data, int argc, char **argv, char **azColName);
        // static int fetch_to_vector(void*_data, int argc, char **argv, char **azColName);
};