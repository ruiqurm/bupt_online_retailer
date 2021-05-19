#include"goods.h"

/***
 * 
 * Goods
 * 
 */
//  void Goods::save(){
//     auto& record = GoodsRecord::get_record();
//     record.set()
//  }
 

/**
 * GoodsRecord
 */
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
    return GoodsFactory::get_goods(goods);
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
