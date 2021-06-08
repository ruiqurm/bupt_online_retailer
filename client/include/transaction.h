#pragma once
#include"user.h"
#include"goods.h"

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
            for(auto&it:data){
                auto g = grecord.get(it.first);
                if(g!=nullptr){
                    //不保证正确性
                    g->remain() -=  std::get<1>(it.second);
                    g->save();
                }
            }
            _id = save();
        }
        // Transaction(){}
        // make_transaction
       
        std::shared_ptr<User> from()const{
            auto& record = UserRecord::get_record();
            return record.create_user(record.get(_from));
        }
       std::shared_ptr<User> to()const{
            auto& record = UserRecord::get_record();
            return record.create_user(record.get(_to));
        }
        void cancel();
        const int id()const{return _id;}
       static string write_detail(const std::map<int,std::tuple<string,int,double>>&data);
       const string& raw_detail()const{
           return _detail;
       }
       const std::shared_ptr<std::map<int,std::tuple<string,int,double>>> detail();
       bool is_finished()const{return _finished;}
       bool set_finished();
       int from_id()const{return _from;}
       int to_id()const{return _to;}
       const string& from_name()const{return _from_name;} 
       const string& to_name()const{return _to_name;} 
       const char* c_from_name()const{return _from_name.c_str();}
       const char* c_to_name()const{return _to_name.c_str();}

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
        typedef std::unique_ptr<std::vector<std::shared_ptr<Transaction>>> pTransVec;
        static TransactionRecord& get_record(){
            static TransactionRecord record;
            return record;
        }
        TransactionRecord(const DiscountRecord&)=delete;//禁止拷贝造
        
        //获取订单
        pTransVec get_transaction(int user_id,bool is_finished);
        
        //生成订单
        int set(Transaction&);
        
        //获取订单
        std::shared_ptr<Transaction> get(int id);

        //移除订单。如果未完成会释放资源，否则跳过
        void cancel(int id);

        //确认订单
        void set_finished(int id);
        
    protected:
        TransactionRecord();
        static int fetch_to_object(void*_data, int argc, char **argv, char **azColName);
        static int fetch_to_vector(void*_data, int argc, char **argv, char **azColName);

    private:
        sqlite3 *db;
};
class CartRecord{
    public:
        static CartRecord& get_record(){
            static CartRecord record;
            return record;
        }
        std::map<int,int> get(int user_id);
        // void set(const std::map<int,int>&m);
        int set(int user_id,int goods_id,int num);
        void remove(int user_id,int goods_id);

    protected:
        CartRecord();
        static int fetch_to_map(void*_data, int argc, char **argv, char **azColName);
    private:
        sqlite3 *db;
};
class Cart{
    //购物车
    public:
        Cart(int user_id);
        
        //判断是否存在
        bool exist(int goods_id);

        //添加
        bool add(int goods_id,int delta,int max_count);
        void clear();
        //添加
        void add(int goods_id,int delta);
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