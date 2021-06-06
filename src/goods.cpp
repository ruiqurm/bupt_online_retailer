#include"goods.h"

/***
 * 
 * Goods
 * 
 */
template <>
constexpr char MetaRecord<Goods,GoodsData>::TABLE_NAME[] = "GOODS";
// template <>
// constexpr char MetaRecord<Transaction,TransactionData>::TABLE_NAME[] ="MYTRANSACTION";

bool Goods::save(){
    auto &record = GoodsRecord::get_record();
    return record.update(data);
}
double Goods::get_price(const GoodsContext& context){
    auto&record = DiscountRecord::get_record();
    double min_price=context.goods().price()*context.num();//TODO: magic number;
    auto p1 = record.get_goods_discount(context.goods().id());
    if(p1){
        min_price = std::min(p1->price(context),min_price);
        std::cout<<p1->price(context)<<std::endl;
    }
    auto p2 = record.get_category_discount(context.goods().type());
    if(p2){
        min_price = std::min(p2->price(context),min_price);
    }
    return min_price;
}
// bool Goods::buy(User* u,int num){
//     if(!u)return false;
//     auto &record = UserRecord::get_record();
//     auto seller = record.get(data.id);
//     if(!seller)return false;
//     //商家不存在
//     #if DEBUG==1
//     #include<iostream>
//     std::cout<<"构建上下文"<<endl;
//     #endif
//     GoodsContext context(this,num,u);
//     double volume = get_price(context);
//     if(u->balance()<volume){
//         return false;
//     }
//     if(data.remain<num){
//         return false;
//     }
//     #if DEBUG==1
//     #include<iostream>
//     std::cout<<"尝试写入交易"<<endl;
//     #endif
//     if(Transaction::make_transaction(data.seller,u->id(),data.name,data.id,volume,data.price,num)){
//         data.remain -= num;
//         save();
//         u->balance() -= volume;
//         u->save();
//         seller->balance += volume;
//         seller->save();
//         return true;
//     }
//     return false;
// } 

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
GoodsRecord::pGoodsVec GoodsRecord::get(const std::vector<int>&l){
    if(l.size()==0)return std::make_unique<std::vector<std::shared_ptr<Goods>>>();
    std::stringstream ss;
    ss<<"SELECT * FROM "
     <<TABLE_NAME
     <<" WHERE ID IN ( "
     <<l[0];
    if(l.size()>1)
    for(int i=1;i<l.size();i++){
        ss<<", "<<l[i];
    }
    ss<<" );";
    // puts(ss.str().c_str());
    auto pvec = std::make_unique<std::vector<std::shared_ptr<Goods>>>();
    Database::exec(db,ss.str().c_str(),fetch_in_vector,pvec.get());
    return pvec;
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
void GoodsRecord::remove(int id){
    auto& record = DiscountRecord::get_record();
    record.remove_by_goods(id);
    MetaRecord<Goods,GoodsData>::remove(id);
}



/*
*
* Discount
*
*/
bool DiscountRecord::make_discount(unsigned char type,int operand,double discount,double threshold){
        static char buffer[192];
        static const char sql[] = "INSERT INTO _DISCOUNT (TYPE,OPERAND,DISCOUNT,THRESHOLD) VALUES(%d ,%d,%f, %f); ";
        sprintf(buffer,sql,type,operand,discount,threshold);
        Database::exec(db,buffer,nullptr,nullptr);
        return true;
}
void DiscountRecord::update_discount(int id,double discount,double thresold){
    static char buffer[192];
    
    static const char sql[] = "UPDATE _DISCOUNT SET DISCOUNT = %f,THRESHOLD=%f WHERE ID=%d;";
    sprintf(buffer,sql,discount,thresold,id);
    // puts(buffer);
    Database::exec(db,buffer,nullptr,nullptr);
}

std::shared_ptr<Discount> DiscountRecord::get_category_discount(int category){
    static char buffer[192];
    static const char sql[] = "SELECT * FROM _DISCOUNT WHERE TYPE = %d AND OPERAND = %d;";
    sprintf(buffer,sql,Discount::TYPE_DISCOUNT,category);
    Discount* tmp=nullptr;
    Database::exec(db,buffer,fetch_to_object,&tmp);
    // std::cout<<tmp->id()<<" "<<tmp->discount()<<std::endl;
    return std::shared_ptr<Discount>(tmp);
}
pDisVec DiscountRecord::get_all_category_discount(){
    static char buffer[192];
    static const char sql[] = "SELECT * FROM _DISCOUNT WHERE TYPE = %d;";
    sprintf(buffer,sql,Discount::TYPE_DISCOUNT);
    auto pvec = std::make_unique<std::vector<std::shared_ptr<Discount>>>();
    Database::exec(db,buffer,fetch_to_vector,&pvec);
    // std::cout<<tmp->id()<<" "<<tmp->discount()<<std::endl;
    return pvec;
}
std::shared_ptr<Discount> DiscountRecord::get_goods_discount(int goods_id){
    static char buffer[192];
    static const char sql[] = "SELECT * FROM _DISCOUNT WHERE TYPE = %d AND OPERAND = %d;";
    auto pvec = std::make_unique<std::vector<std::shared_ptr<Discount>>>();
    sprintf(buffer,sql,Discount::DISCOUNT,goods_id);
    // puts(buffer);
    
    Discount* tmp=nullptr;
    Database::exec(db,buffer,fetch_to_object,&tmp);
        // std::cout<<tmp->id()<<" "<<tmp->discount()<<std::endl;


    return std::shared_ptr<Discount>(tmp);
}
DiscountRecord::DiscountRecord():db(Database::get_db()){
    static const char sql[]= "CREATE TABLE _DISCOUNT("  \
                    "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                    "TYPE            INT    NOT NULL," \
                    "OPERAND         INT       NOT NULL," \
                    "DISCOUNT        FLOAT         NOT NULL," \
                    "THRESHOLD       FLOAT         NOT NULL," \
                    "UNIQUE(TYPE,OPERAND) ON CONFLICT REPLACE );";
    Database::exec(db,sql,nullptr,nullptr);
}

int DiscountRecord::fetch_to_object(void*_data, int argc, char **argv, char **azColName){
    if(argc!=5){
        (*(Discount**)_data) = nullptr;
        return -1;
    }
    
    int type = atoi(argv[1]);
    switch (type){
    case Discount::DISCOUNT:
        (*(Discount**)_data) = new DiscountSimple(atoi(argv[0]),atoi(argv[1]),atoi(argv[2]),atof(argv[3]),atof(argv[4]));
        break;
    case Discount::TYPE_DISCOUNT:
        (*(Discount**)_data)  =  new DiscountCategory(atoi(argv[0]),atoi(argv[1]),atoi(argv[2]),atof(argv[3]),atof(argv[4]));
        break;
    default:
        (*(Discount**)_data) = nullptr;
        break;
    }
    return 0;
}
int DiscountRecord::fetch_to_vector(void*_data, int argc, char **argv, char **azColName){
        if(argc!=5){
        (*(Discount**)_data) = nullptr;
        return -1;
    }
    auto& vec = *(std::unique_ptr<std::vector<std::shared_ptr<Discount>>>*)_data;
    int type = atoi(argv[1]);
    switch (type){
    case Discount::DISCOUNT:

        vec->emplace_back(std::make_shared<DiscountSimple>(atoi(argv[0]),atoi(argv[1]),atoi(argv[2]),atof(argv[3]),atof(argv[4])));
        break;
    case Discount::TYPE_DISCOUNT:
        vec->emplace_back(std::make_shared<DiscountCategory>(atoi(argv[0]),atoi(argv[1]),atoi(argv[2]),atof(argv[3]),atof(argv[4])));
        break;
    default:


        break;
    }
    return 0;
}
void DiscountRecord::remove(int id){
    static char buffer[192];
    static const char sql[] = "DELETE FROM _DISCOUNT WHERE ID=%d;";
    sprintf(buffer,sql,id);
    Database::exec(db,buffer,nullptr,nullptr);
}
void DiscountRecord::remove_by_goods(int goods_id){
    static char buffer[192];
    static const char sql[] = "DELETE FROM _DISCOUNT WHERE TYPE=%d AND OPERAND=%d;";
    sprintf(buffer,sql,Discount::DISCOUNT,goods_id);
    Database::exec(db,buffer,nullptr,nullptr);
}
