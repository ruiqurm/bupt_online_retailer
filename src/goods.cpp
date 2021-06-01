#include"goods.h"

/***
 * 
 * Goods
 * 
 */
template <>
constexpr char MetaRecord<Goods,GoodsData>::TABLE_NAME[] = "GOODS";
template <>
constexpr char MetaRecord<Transaction,TransactionData>::TABLE_NAME[] ="MYTRANSACTION";

bool Goods::save(){
    auto &record = GoodsRecord::get_record();
    return record.update(data);
}
bool Goods::buy(User* u,int num){
    if(!u)return false;
    auto &record = UserRecord::get_record();
    auto seller = record.get(data.id);
    if(!seller)return false;
    //商家不存在
    #if DEBUG==1
    #include<iostream>
    std::cout<<"构建上下文"<<endl;
    #endif
    GoodsContext context(this,num,u);
    double volume = get_price(context);
    if(u->balance()<volume){
        return false;
    }
    if(data.remain<num){
        return false;
    }
    #if DEBUG==1
    #include<iostream>
    std::cout<<"尝试写入交易"<<endl;
    #endif
    if(Transaction::make_transaction(data.seller,u->id(),data.name,data.id,volume,data.price,num)){
        data.remain -= num;
        save();
        u->balance() -= volume;
        u->save();
        seller->balance += volume;
        seller->save();
        return true;
    }
    return false;
} 

/**
 * GoodsRecord
 */
GoodsRecord::p_goods_construct GoodsRecord::register_types[256] {nullptr};
GoodsRecord::GoodsRecord():manager(UserRecord::get_record()){
    static const char sql[]= "CREATE TABLE GOODS("  \
                                "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                                "NAME           CHAR(64)    NOT NULL," \
                                "PRICE          FLOAT       NOT NULL," \
                                "SELLER         INT         NOT NULL," \
                                "TYPE           INT         NOT NULL," \
                                "REMAIN         INT         NOT NULL," \
                                "DESCRIPTION    TEXT        NOT NULL);";
    Database::exec(db,sql,nullptr,nullptr);
}
std::shared_ptr<Goods> GoodsRecord::get(int id){
    static const char sql[] = "SELECT * FROM %s WHERE ID=%d";
    static char buffer[48]; 
    GoodsData goods;

    snprintf(buffer,48,sql,TABLE_NAME,id);
    Database::exec(db,buffer,fetch_in_struct,&goods);
    return register_types[goods.type](goods);
}
GoodsRecord::pGoodsVec GoodsRecord::get_user_goods(int seller_id){
    static const char sql[] =  "SELECT * FROM %s WHERE SELLER=%d";
    static char buffer[48];
    auto pvec = std::make_unique<std::vector<std::shared_ptr<Goods>>>();
    snprintf(buffer,48,sql,TABLE_NAME,seller_id);
    Database::exec(db,buffer,fetch_in_vector,pvec.get());
    return pvec;
}
GoodsRecord::pGoodsVec GoodsRecord::get_all_goods(){
    static const char sql[] =  "SELECT * FROM %s";
    static char buffer[48];
    auto pvec = std::make_unique<std::vector<std::shared_ptr<Goods>>>();
    snprintf(buffer,48,sql,TABLE_NAME);
    Database::exec(db,buffer,fetch_in_vector,pvec.get());
    return pvec;
}
GoodsRecord::pGoodsVec GoodsRecord::get_goods_by_name(const string& search_for_name){
    static const char sql[] =  "SELECT * FROM %s WHERE NAME LIKE '%%%s%%' OR DESCRIPTION LIKE '%%%s%%' COLLATE NOCASE";
    //两个%%代表一个%,不区分大小写
    static char buffer[256];
    auto pvec = std::make_unique<std::vector<std::shared_ptr<Goods>>>();
    snprintf(buffer,256,sql,TABLE_NAME,search_for_name.c_str(),search_for_name.c_str());
    Database::exec(db,buffer,fetch_in_vector,pvec.get());
    return pvec;
}
bool GoodsRecord::update(const GoodsData& data){
    static const char sql[] =  "UPDATE %s "\
                                "SET NAME = '%s'," \
                                "PRICE = %f," \
                                "SELLER = %d," \
                                "REMAIN = %d ," \
                                "DESCRIPTION = '%s'"\
                                "WHERE ID=%d";
    static char buffer[512];
    snprintf(buffer,512,sql,TABLE_NAME,data.name.c_str(),data.price,data.seller,data.remain,data.description.c_str(),data.id);
    return Database::exec(db,buffer,nullptr,nullptr);
}

/**
 * 
 * 
 * 
 */

TransactionRecord::TransactionRecord(){
    static const char sql[]= "CREATE TABLE MYTRANSACTION("  \
                                "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                                "GOODSNAME      CHAR(64)    NOT NULL," \
                                "_FROM           INT       NOT NULL," \
                                "_TO             INT         NOT NULL," \
                                "GOODSID        INT         NOT NULL," \
                                "VOLUME         FLOAT         NOT NULL," \
                                "PRICE          FLOAT         NOT NULL," \
                                "COUNT          INT         NOT NULL," \
                                "TIMESTAMP      INT         NOT NULL);" ;
    Database::exec(db,sql,nullptr,nullptr);
}
std::shared_ptr<Transaction> TransactionRecord::get(int id){
    static const char sql[] = "SELECT * FROM %s WHERE ID=%d";
    static char buffer[48]; 
    TransactionData d;

    snprintf(buffer,48,sql,TABLE_NAME,id);
    Database::exec(db,buffer,fetch_in_struct,&d);
    return std::make_shared<Transaction>(d);
}
TransactionRecord::pTransVec TransactionRecord::get_user_transactions(int user_id){
    static const char sql[] =  "SELECT * FROM %s WHERE FROM =%d OR TO=%d";
    static char buffer[64];
    auto pvec = std::make_unique<std::vector<std::shared_ptr<Transaction>>>();
    snprintf(buffer,64,sql,TABLE_NAME,user_id,user_id);
    Database::exec(db,buffer,fetch_in_vector,pvec.get());
    return pvec;
}
/**
 * 
 * Transaction
 * 
 */
int Transaction::make_transaction(int from,int to,string goodsName,int goodsID,double volume,double price,int count){
    auto &record = TransactionRecord::get_record();
    TransactionData data(from,to,goodsName,goodsID,volume,price,count);
    return record.set(data);
}
std::shared_ptr<Goods> Transaction::goods()const{
    auto& record = GoodsRecord::get_record();
    return record.get(data.goodsID);
}