/**
 * @file transaction.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 交易订单与购物车
 */
#pragma once
#include"user.h"
#include"goods.h"

/**
 * @brief 交易订单
 * 
 */
class Transaction{
    public:
        Transaction(int id,int from,string from_name,int to,string to_name,bool finished,double volume,
                    std::string detail,time_t timestamp):
                    _id(id),_from(from),_to(to),_from_name(from_name),_to_name(to_name),_finished(finished),
                    _volume(volume),_detail(detail),_timestamp(timestamp),_detail_map(nullptr){
                    }
        Transaction(int from,int to,double volume,std::map<int,std::tuple<string,int,double>>&data):
        _id(-1),_from(from),_to(to),_volume(volume),_timestamp(time(NULL)),_detail(write_detail(data)),_finished(false),
        _detail_map(std::make_shared<std::map<int,std::tuple<string,int,double>>>(data))
        {
            auto &record = UserRecord::get_record();
            _from_name = record.get_username(from);
            _to_name = record.get_username(to);
            auto &grecord = GoodsRecord::get_record();
            std::cout<<"本份data有"<<data.size()<<"条"<<std::endl;
            for(auto&it:data){
                std::cout<< "id:"<<it.first<<std::endl;
                auto g = grecord.get(it.first);
                if(g!=nullptr){
                    //不保证正确性
                    std::cout<< "商品名:"<<g->name()<<std::endl;
                    g->remain() -=  std::get<1>(it.second);
                    g->save();
                }
            }
            _id = save();
        }
        // Transaction(){}
        // make_transaction
       
        /// 获取卖方指针
        std::shared_ptr<User> from()const{
            auto& record = UserRecord::get_record();
            return record.create_user(record.get(_from));
        }

        /// 获取买方指针
       std::shared_ptr<User> to()const{
            auto& record = UserRecord::get_record();
            return record.create_user(record.get(_to));
        }

        /// 取消订单
        void cancel();

        /// id 
        const int id()const{return _id;}

        /**
         * @brief 替换详情
         * @details 详情的形式是一个字典，键是商品id，值是商品名，数量，价格tuple
         * @param data 
         * @return string 
         */
       static string write_detail(const std::map<int,std::tuple<string,int,double>>&data);

        /// 详情
        const string& raw_detail()const{
            return _detail;
        }

       /// 获取详情字典
        const std::shared_ptr<std::map<int,std::tuple<string,int,double>>> detail();

       /// 订单是否完成
        bool is_finished()const{return _finished;}

       /// 订单标记完成
        bool set_finished();

       /// 卖方id
        int from_id()const{return _from;}

       /// 买方id
        int to_id()const{return _to;}

       /// 卖方用户名
        const string& from_name()const{return _from_name;} 
    
       /// 买方用户名
        const string& to_name()const{return _to_name;} 
        const char* c_from_name()const{return _from_name.c_str();}
        const char* c_to_name()const{return _to_name.c_str();}
        
        /// 时间戳
        time_t timestamp()const{return _timestamp;}
        double volume()const{return _volume;}
    protected:
        int save();
    private:
        int _id,_from,_to;
        string _from_name,_to_name;
        bool _finished;
        double _volume;
        string _detail;
        time_t _timestamp;
        std::shared_ptr<std::map<int,std::tuple<string,int,double>>> _detail_map;
};
class TransactionRecord{
    public:
        /// 指向订单对象的vector的智能指针 类型 
        typedef std::unique_ptr<std::vector<std::shared_ptr<Transaction>>> pTransVec;
        static TransactionRecord& get_record(){
            static TransactionRecord record;
            return record;
        }
        TransactionRecord(const DiscountRecord&)=delete;//禁止拷贝构造
        
        /**
         * @brief 获取指定用户的订单
         * 
         * @param user_id 用户id
         * @return pTransVec 指向订单vector的智能指针
         */    
        pTransVec get_transaction(int user_id,bool is_finished);
        
        /**
         * @brief 生成订单
         * @param t 订单对象
         * @return int 
         */
        int set(Transaction&t);
        
        /**
         * @brief 获取指定id的订单
         * 
         * @param id 订单id
         * @return 指向订单的智能指针
         */      
        std::shared_ptr<Transaction> get(int id);

        /**
         * @brief 取消订单
         * @details 取消订单。如果未完成会释放资源，否则跳过
         * @param id 
         */
        void cancel(int id);

        /**
         * @brief 订单完成
         * 
         * @param id 订单id
         */
        void set_finished(int id);
        
    protected:
        TransactionRecord():base(Database::get_database()){}
        // static int fetch_to_object(void*_data, int argc, char **argv, char **azColName);
        // static int fetch_to_vector(void*_data, int argc, char **argv, char **azColName);
    
    private:
        Database* base;
        char send_buf[8192];
        char recv_buf[8192];
};

/**
 * @brief 购物车数据库接口
 * 
 */
class CartRecord{
    public:
        static CartRecord& get_record(){
            static CartRecord record;
            return record;
        }
        /// 获取购物车
        std::map<int,int> get(int user_id);

        /// 添加购物车项
        int set(int user_id,int goods_id,int num);

        /// 移除一个购物车项
        void remove(int user_id,int goods_id);
        
    protected:
        CartRecord():base(Database::get_database()){}
        // static int fetch_to_map(void*_data, int argc, char **argv, char **azColName);
    private:
        Database* base;
        char send_buf[8192];
        char recv_buf[8192];
};

/**
 * @brief 
 * 
 */
class Cart{
    //购物车
    public:
        Cart(int user_id);
        
        /// 判断是否存在
        bool exist(int goods_id);

        /// 添加
        bool add(int goods_id,int delta,int max_count);

        /// 清空
        void clear();
        
        ///添加一个商品的数量
        void add(int goods_id,int delta);

        /// 减少一个商品的数量        
        void reduce(int goods_id,int delta);
        
        //设置id为更新的数值
        bool set(int goods_id,int num,int max_count=-1);
        
        //移除
        void remove(int goods_id);
        const std::map<int,int>& data()const;
        std::map<int,int>&data();
        int operator [](int goods_id)const;
    private:
        std::map<int,int>_data;
        int user;
};