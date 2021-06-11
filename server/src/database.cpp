#include"database.h"
extern "C" {
#include"log.h"
}
#include<sstream>
#include<unistd.h>
#include"sqlite3.h"
using std::string;
using std::stringstream;


/********************
 *                  *
 * UserRecordWriter *
 *                  *  
 ********************/
string UserRecordWriter::login(const protoData::UserForm & form){
    auto it = name_to_data.find(form.username());
    if(it!=name_to_data.end()){
        log_info("找到名字");
        if (it->second.password()==form.password()){
            auto iit = id_to_token.find(it->second.id());
            if(iit!=id_to_token.end()){
                //已经登录
                return iit->second;
            }else{
                string s = generate_token();
                token_to_id[s] = it->second.id();
                id_to_token[it->second.id()] = s;
                return s;
            }
        }
    }
    return string();
}
bool UserRecordWriter::logout(protoData::User*user){
    if(user!=nullptr){
        auto it = id_to_token.find(user->id());
        if(it!=id_to_token.end()){
            token_to_id.erase(it->second);
            id_to_token.erase(it);
            return true;
        }
    }
    return false;
}

bool UserRecordWriter::_register(protoData::UserForm & form){
    /**
     * @brief 只检查是否存在，用户名和密码是否带空白符
     * 
     */
    log_info("注册账号，用户名为%s,密码为%s",form.username().c_str(),form.password().c_str());
    auto it = name_to_data.find(form.username());
    if(it != name_to_data.end()){
        // last_error = "account has been registered";
        log_debug("账号已存在");
        return false;
    } 
    char*c= const_cast<char*>(form.username().c_str());
    int i=0;
    for(auto &c:form.username()){
        i++;
        if(isspace(c)){
            log_error("不允许空格");
            // last_error = "disallow space character";
            return false;
        }
    }
    if(i>32){
        log_error("用户名过长");
        // last_error = "username too long";
        return false;
    }
    for(auto&c:form.password()){
        i++;
        if(isspace(c)){
            log_error("不允许空格");
            // last_error = "disallow space character";
            return false;
        }
    }  
    if(i>16){
        log_error("密码过长");
        // last_error = "password too long";
        return false;
    }
    insert_data(form);
    protoData::User u;
    u.set_id(max_pk);
    u.set_username(form.username());
    u.set_password(form.password());
    u.set_type(form.type());
    name_to_data[form.username()] = u;
    id_to_name[max_pk] = form.username();
    log_info("注册账号成功");
    return true;
}
protoData::User* UserRecordWriter::get_user_by_token(const char*token){
    char tmp[TOKEN_SIZE+2];
    memcpy(tmp,token,16);
    tmp[TOKEN_SIZE] = 0;
    auto it =  token_to_id.find(tmp);
    if(it!=token_to_id.end()){
        auto iit = id_to_name.find(it->second);
        log_info("id=%d",it->second);
        if(iit!=id_to_name.end()){
            log_info("name=%s",iit->second.c_str());
            auto iiit = name_to_data.find(iit->second);
            if(iiit!=name_to_data.end()){
                return &iiit->second;
            }
        }
        
    }
    // for(auto&it:token_to_id){
    //     std::cout<<"token = "<<it.first<<" id="<<it.second<<std::endl;
    // }
    log_error("无此用户,token=%.16s",tmp);
    return nullptr;
}
protoData::User*  UserRecordWriter::get_user_by_username(const string&username){
    auto it =  name_to_data.find(username);
    if(it!=name_to_data.end()){
        return &it->second;
    }else{
        return nullptr;
    }
}
bool UserRecordWriter::check_password(protoData::User* user,const string& password){
    return user->password() == password;
}
UserRecordWriter::UserRecordWriter(){
    if (exist(USER_FILE_NAME)){
        load();
        database.open(USER_FILE_NAME,std::ios::in|std::ios::out);
    //不会覆盖原来的内容
    }else{
        database.open(USER_FILE_NAME,std::ios::out);
    }
    if (!database.is_open()){
        throw "can't open user record file.";
    }
    log_debug("init user-record done");
}
int UserRecordWriter::load(){
    int count=0,i=0;
    std::ifstream f(USER_FILE_NAME);
    if (!name_to_data.empty())name_to_data.clear();
    if (!id_to_name.empty())id_to_name.clear();
    protoData::User tmpdata;
    max_pk=0;
    double tmpfloat;
    int tmpint;
    string tmpstr;
    while(f>>tmpint){
        tmpdata.set_id(tmpint);
        if(tmpdata.id()!=0){
            f>>tmpstr;
            tmpdata.set_username(tmpstr);
            f>>tmpstr;
            tmpdata.set_password(tmpstr);
            f>>tmpfloat;
            tmpdata.set_balance(tmpfloat);
            f>>tmpint;
            name_to_data.emplace(tmpdata.username(),tmpdata);
            id_to_name.emplace(tmpdata.id(),tmpdata.username());
            max_pk = std::max(max_pk,tmpdata.id());
            count++;
        }
        f.seekg(i+MAX_LINE);
        i+=MAX_LINE;
    }
    return count;
}
void UserRecordWriter::insert_data(protoData::UserForm& data){
    max_pk++;
    int id = max_pk;
    set_write_cursor_to_nth_line(id);
    database<<id<<" "<<data.username()<<" "<<data.password()<<" "<<0<<" "<<data.type()<<"\0\0\0";
    write_LF_nth_line(id);
}
void UserRecordWriter::update(protoData::User& data){
    if(data.id()<=0){
        //这段不会执行
        max_pk++;
        data.set_id(max_pk);
        data.set_balance(0);
    }
    // log_info("id=%d,username=%s,password=%s",data.id(),data.username().c_str(),data.password().c_str());
    set_write_cursor_to_nth_line(data.id());
    // std::cout<<data.password()<<std::endl;
    std::cout<<data.id()<<" "<<data.username()<<" "<<data.password()<<" "<<data.balance()<<" "<<data.type()<<std::endl;
    database<<data.id()<<" "<<data.username()<<" "<<data.password()<<" "<<data.balance()<<" "<<data.type();
    database.put('\0');database.put('\0');
    write_LF_nth_line(data.id());
    name_to_data[data.username()] = data;
}
void UserRecordWriter::remove_data(int id){
    if(id<=0)return;
    //不判断是否存在了.
    set_write_cursor_to_nth_line(id);
    database<<"0\0";database.flush();
    // write_LF_nth_line(id);
}
string UserRecordWriter::generate_token(){
    char tmp_s[TOKEN_SIZE+2];
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    srand( (unsigned) time(NULL) * getpid());
    for (int i = 0; i < TOKEN_SIZE; ++i) 
        tmp_s[i] =  alphanum[rand() % (sizeof(alphanum) - 1)];
    tmp_s[TOKEN_SIZE] = 0;
    return string(tmp_s);
}
void UserRecordWriter::write_LF_nth_line(int id){
    #if WIN32
    database.seekp(id*MAX_LINE -2);
    #else
    database.seekp(id*MAX_LINE -1);
    #endif
    database<<'\n';
}
void UserRecordWriter::set_write_cursor_to_nth_line(int id){
    database.seekp((id-1)*(MAX_LINE));
}
void UserRecordWriter::set_read_cursor_to_nth_line(int  id){
    database.seekg((id-1)*(MAX_LINE));
}

/**
 * 
 * 
 */
sqlite3* origin_db;
bool db_is_init=false;

Database::Database(){
    static bool run_once =false;
    if(!run_once){
        
        run_once = true;
    }else{
        this->db = origin_db;
    }
}
bool Database::exec(sqlite3* db,const char *sql, int (*callback)(void *, int, char **, char **),void *arg){
    char *errmsg=nullptr;
    int rc = sqlite3_exec(db,sql,callback,arg,&errmsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return false;
    }
    return true;
}
bool Database::exec(const char *sql, int (*callback)(void *, int, char **, char **),void *arg){
    char *errmsg=nullptr;
    int rc = sqlite3_exec(origin_db,sql,callback,arg,&errmsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return false;
    }
    return true;
}

/***********************
 *                     *
 *    GoodsDatabase    *
 *                     *
 ***********************/

bool GoodsDatabase::init(){
    if(!db_is_init){
        sqlite3_open("data.db",&origin_db);
        db_is_init = true;
    }
    static const char sql[]= "CREATE TABLE GOODS("  \
                                "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                                "NAME           CHAR(64)    NOT NULL," \
                                "PRICE          FLOAT       NOT NULL," \
                                "SELLER         INT         NOT NULL," \
                                "TYPE           INT         NOT NULL," \
                                "REMAIN         INT         NOT NULL," \
                                "DESCRIPTION    TEXT        NOT NULL);";
    exec(origin_db,sql,nullptr,nullptr);
    return true;
}
const bool GoodsDatabase::is_init = init();

int GoodsDatabase::create(protoData::Goods& data){
   char buffer[512]; 
   static const char sql[] =  "INSERT INTO GOODS (NAME,PRICE,SELLER,TYPE,REMAIN,DESCRIPTION) VALUES ('%s', %f, %d ,%d,%d,'%s'); ";
   snprintf(buffer,512,sql,data.name().c_str(),data.price(),data.seller(),data.type(),data.remain(),data.description().c_str());
   exec(buffer,nullptr,nullptr);
   return sqlite3_last_insert_rowid(origin_db);//越界
}
void GoodsDatabase::get(protoData::Goods& goods){
    static const char sql[] = "SELECT * FROM GOODS WHERE ID=%d";
    static char buffer[48]; 
    snprintf(buffer,48,sql,goods.id());
    exec(buffer,fetch_to_object,&goods);
}

void GoodsDatabase::get(protoData::GoodsArray&array){
    int n = array.goods_size();
    if(n==0)return;
    std::stringstream ss;
    ss<<"SELECT * FROM GOODS"
     <<" WHERE ID IN ( "
     <<array.goods()[0].id();
    if(n>1)
    for(int i=1;i<n;i++){
        ss<<", "<<array.goods()[i].id();
    }
    ss<<" );";
    array.clear_goods();
    // puts(ss.str().c_str());
    exec(ss.str().c_str(),fetch_to_vector,&array);
}
void GoodsDatabase::get_user_goods(int seller_id,protoData::GoodsArray&array){
    static const char sql[] =  "SELECT * FROM GOODS WHERE SELLER=%d";
    static char buffer[48];
    snprintf(buffer,48,sql,seller_id);
    exec(buffer,fetch_to_vector,&array);
}

void GoodsDatabase::get_all_goods(protoData::GoodsArray&array){
    static const char sql[] =  "SELECT * FROM GOODS";
    static char buffer[48];
    snprintf(buffer,48,sql);
    exec(buffer,fetch_to_vector,&array);
}
void GoodsDatabase::get_goods_by_name(const string& search_for_name,protoData::GoodsArray&array){
    static const char sql[] =  "SELECT * FROM GOODS WHERE NAME LIKE '%%%s%%' OR DESCRIPTION LIKE '%%%s%%' COLLATE NOCASE";
    //两个%%代表一个%,不区分大小写
    static char buffer[256];
    snprintf(buffer,256,sql,search_for_name.c_str(),search_for_name.c_str());
    exec(buffer,fetch_to_vector,&array);
}
bool GoodsDatabase::update(const protoData::Goods& data){
    static const char sql[] =  "UPDATE GOODS "\
                                "SET NAME = '%s'," \
                                "PRICE = %f," \
                                "SELLER = %d," \
                                "REMAIN = %d ," \
                                "DESCRIPTION = '%s'"\
                                "WHERE ID=%d";
    static char buffer[512];
    snprintf(buffer,512,sql,data.name().c_str(),data.price(),data.seller(),data.remain(),data.description().c_str(),data.id());
    exec(buffer,nullptr,nullptr);
    return true;
}
void GoodsDatabase::remove(const protoData::Goods& data){
    static const char sql[] = "DELETE FROM GOODS WHERE ID = %d;";
    static char buffer[256]; 
    
    sprintf(buffer,sql,data.id());
    puts(buffer);
    exec(buffer,nullptr,nullptr);
}
int GoodsDatabase::fetch_to_object(void*_data, int argc, char **argv, char **azColName){
    protoData::Goods* data = (protoData::Goods*)_data;
    data->set_id(            atoi(argv[0]) );
    data->set_name(          argv[1]       );
    data->set_price(         atof(argv[2]) );
    data->set_seller(        atoi(argv[3]) );
    data->set_type(          atoi(argv[4]) );
    data->set_remain(        atoi(argv[5]) );
    data->set_description(   argv[6]       );
    return 0;
}
int GoodsDatabase::fetch_to_vector(void*_data, int argc, char **argv, char **azColName){
    protoData::Goods* data = ((protoData::GoodsArray*)_data)->add_goods();
    data->set_id(            atoi(argv[0]) );
    data->set_name(          argv[1]       );
    data->set_price(         atof(argv[2]) );
    data->set_seller(        atoi(argv[3]) );
    data->set_type(          atoi(argv[4]) );
    data->set_remain(        atoi(argv[5]) );
    data->set_description(   argv[6]       );
    return 0;
}

/***********************
 *                     *
 *  DiscountDatabase   *
 *                     *
 ***********************/

int DiscountDatabase::create(protoData::Discount& discount){
    static char buffer[192];
    static const char sql[] = "INSERT INTO _DISCOUNT (TYPE,OPERAND,DISCOUNT,THRESHOLD) VALUES(%d ,%d,%f, %f); ";
    sprintf(buffer,sql,discount.type(),discount.operand(),discount.discount(),discount.threshold());
    exec(buffer,nullptr,nullptr);
    return sqlite3_last_insert_rowid(origin_db);//越界
}
bool DiscountDatabase::init(){
    static const char sql[]= "CREATE TABLE _DISCOUNT("  \
                    "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                    "TYPE            INT    NOT NULL," \
                    "OPERAND         INT       NOT NULL," \
                    "DISCOUNT        FLOAT         NOT NULL," \
                    "THRESHOLD       FLOAT         NOT NULL," \
                    "UNIQUE(TYPE,OPERAND) ON CONFLICT REPLACE );";
    exec(origin_db,sql,nullptr,nullptr);
    return true;
}
const bool DiscountDatabase::is_init = init();

void DiscountDatabase::update(protoData::Discount& discount){
    static char buffer[192];
    
    static const char sql[] = "UPDATE _DISCOUNT SET DISCOUNT = %f,THRESHOLD=%f WHERE ID=%d;";
    sprintf(buffer,sql,discount.discount(),discount.threshold(),discount.id());
    // puts(buffer);
    exec(buffer,nullptr,nullptr);
}

void DiscountDatabase::get_category_discount(protoData::Discount& discount){
    static char buffer[192];
    static const char sql[] = "SELECT * FROM _DISCOUNT WHERE TYPE = %d AND OPERAND = %d;";
    sprintf(buffer,sql,discount.type(),discount.operand());
    exec(buffer,fetch_to_object,&discount);
}
void DiscountDatabase::get_all_category_discount(protoData::Discount&discount,protoData::DiscountArray& array){
    static char buffer[192];
    static const char sql[] = "SELECT * FROM _DISCOUNT WHERE TYPE = %d;";
    sprintf(buffer,sql,discount.type());
    exec(buffer,fetch_to_vector,&array);
    // std::cout<<tmp->id()<<" "<<tmp->discount()<<std::endl;
}
void DiscountDatabase::get_goods_discount(protoData::Discount& discount){
    static char buffer[192];
    static const char sql[] = "SELECT * FROM _DISCOUNT WHERE TYPE = %d AND OPERAND = %d;";
    sprintf(buffer,sql,discount.type(),discount.operand());
    exec(buffer,fetch_to_object,&discount);
}


int DiscountDatabase::fetch_to_object(void*_data, int argc, char **argv, char **azColName){
    // if(argc!=5){
    //     (*(Discount**)_data) = nullptr;
    //     return -1;
    // }
    protoData::Discount* data = (protoData::Discount*)_data;
    data->set_id(            atoi(argv[0])        );
    data->set_type(          atoi(argv[1])        );
    data->set_operand(       atoi(argv[2])        );
    data->set_discount(      atof( argv[3])       );
    data->set_threshold(     atof(argv[4])        );
    return 0;
}
int DiscountDatabase::fetch_to_vector(void*_data, int argc, char **argv, char **azColName){
    protoData::Discount* data = ((protoData::DiscountArray*)_data)->add_discount();
    data->set_id(            atoi(argv[0])        );
    data->set_type(          atoi(argv[1])        );
    data->set_operand(       atoi(argv[2])        );
    data->set_discount(      atof( argv[3])       );
    data->set_threshold(     atof(argv[4])        );
    return 0;
}
void DiscountDatabase::remove(protoData::Discount& discount){
    static char buffer[192];
    static const char sql[] = "DELETE FROM _DISCOUNT WHERE ID=%d;";
    sprintf(buffer,sql,discount.id());
    exec(buffer,nullptr,nullptr);
}
void DiscountDatabase::remove_by_goods(protoData::Discount& discount){
    static char buffer[192];
    static const char sql[] = "DELETE FROM _DISCOUNT WHERE TYPE=%d AND OPERAND=%d;";
    sprintf(buffer,sql,discount.type(),discount.operand());
    exec(buffer,nullptr,nullptr);
}



/***********************
 *                     *
 * TransactionDatabase *
 *                     *
 ***********************/
bool TransactionDatabase::init(){
    static const char sql[]= "CREATE TABLE _TRANSACTION("  \
                                "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                                "_FROM           INT       NOT NULL," \
                                "_FROM_NAME      CHAR(64)    NOT NULL," \
                                "_TO             INT         NOT NULL," \
                                "_TO_NAME      CHAR(64)    NOT NULL," \
                                "FINISHED       INT         NOT NULL," \
                                "VOLUME         FLOAT         NOT NULL," \
                                "DETAIL         TEXT        NOT NULL," \
                                "TIMESTAMP      INT         NOT NULL);" ;
    exec(origin_db,sql,nullptr,nullptr);
    return true;
}

const bool TransactionDatabase::is_init = init();

void TransactionDatabase::get(protoData::Transaction&transaction){
    static const char sql[] = "SELECT * FROM _TRANSACTION WHERE ID=%d";
    static char buffer[64]; 

    sprintf(buffer,sql,transaction.id());
    exec(buffer,fetch_to_object,&transaction);

}
int TransactionDatabase::set(protoData::Transaction&transaction){
    static char buffer[1024];
    static const char sql[] = "INSERT INTO _TRANSACTION (_FROM,_FROM_NAME,_TO,_TO_NAME,FINISHED,VOLUME,DETAIL,TIMESTAMP)"\
                              "VALUES(%d,'%s',%d,'%s',%d,%f,'%s',%ld); ";
    
    sprintf(buffer,sql,transaction.from(),transaction.from_name().c_str()
    ,transaction.to(),transaction.to_name().c_str(),transaction.finished(),
    transaction.volume(),transaction.detail().c_str(),transaction.timestamp());
    exec(buffer,nullptr,nullptr);
    return sqlite3_last_insert_rowid(origin_db);
}
int TransactionDatabase::fetch_to_object(void*_data, int argc, char **argv, char **azColName){
    protoData::Transaction* data = (protoData::Transaction*)_data;
    data->set_id (          atoi(argv[0])      );
    data->set_from(         atoi(argv[1])      );
    data->set_from_name(    argv[2]            );
    data->set_to(           atoi(argv[3])      );
    data->set_to_name(      argv[4]            );
    data->set_finished(     atoi(argv[5])      );
    data->set_volume(       atof(argv[6])      );  
    data->set_detail(       argv[7]            );
    data->set_timestamp(    atoll(argv[8])     );
    return 0;
}
int TransactionDatabase::fetch_to_vector(void*_data, int argc, char **argv, char **azColName){
    protoData::Transaction* data = ((protoData::TransactionArray*)_data)->add_transaction();
    data->set_id (          atoi(argv[0])      );
    data->set_from(         atoi(argv[1])      );
    data->set_from_name(    argv[2]            );
    data->set_to(           atoi(argv[3])      );
    data->set_to_name(      argv[4]            );
    data->set_finished(     atoi(argv[5])      );
    data->set_volume(       atof(argv[6])      );  
    data->set_detail(       argv[7]            );
    data->set_timestamp(    atoll(argv[8])     );
    return 0;
}
void TransactionDatabase::cancel(protoData::Transaction&transaction){
    static const char sql[] = "DELETE FROM _TRANSACTION WHERE ID=%d AND FINISHED=0;";
    static char buffer[64];
    sprintf(buffer,sql,transaction.id());
    exec(buffer,nullptr,nullptr);
}
void TransactionDatabase::set_finished(protoData::Transaction&transaction){
    static const char sql[] = "UPDATE _TRANSACTION SET FINISHED=1 WHERE ID=%d";
    static char buffer[64];
    sprintf(buffer,sql,transaction.id());
    exec(buffer,nullptr,nullptr);
}
void TransactionDatabase::get_transaction(protoData::Transaction&transaction,protoData::TransactionArray &array){
    static char buffer[96];
    static const char sql[] = "SELECT * FROM _TRANSACTION WHERE FINISHED = %d AND (_FROM = %d OR _TO = %d);";
    sprintf(buffer,sql,transaction.finished(),transaction.from(),transaction.to());
    exec(buffer,fetch_to_vector,&array);
}

/***********************
 *                     *
 *    CartDatabase     *
 *                     *
 ***********************/

bool CartDatabase::init(){
    static const char sql[]= "CREATE TABLE CART("\
                            "ID INTEGER PRIMARY KEY AUTOINCREMENT,"\
                            "USER           INT       NOT NULL,"\
                            "GOODS          INT       NOT NULL,"\
                            "COUNT          INT       NOT NULL,"\
                            "UNIQUE(USER,GOODS) ON CONFLICT REPLACE);";
    exec(origin_db,sql,nullptr,nullptr);
    return true;
}
const bool CartDatabase::is_init = init();

void CartDatabase::get(protoData::CartItem&item, protoData::Cart&cart){
    static const char sql[] = "SELECT * FROM CART WHERE USER=%d";
    static char buffer[64];
    sprintf(buffer,sql,item.user());
    exec(buffer,fetch_to_vector,&cart);
}
int CartDatabase::set(protoData::CartItem&item){
    static char buffer[128];
    static const char sql[] = "INSERT INTO CART (USER,GOODS,COUNT)"\
                            "VALUES(%d,%d,%d); ";
    sprintf(buffer,sql,item.user(),item.goods(),item.count());
    exec(buffer,nullptr,nullptr);
    return sqlite3_last_insert_rowid(origin_db);
}
void CartDatabase::remove(protoData::CartItem&item){
    static char buffer[128];
    static const char sql[] = "DELETE FROM CART WHERE USER=%d AND GOODS=%d;";
    sprintf(buffer,sql,item.user(),item.goods());
    exec(buffer,nullptr,nullptr);
}
int CartDatabase::fetch_to_vector(void*_data, int argc, char **argv, char **azColName){
    protoData::CartItem * data = ((protoData::Cart*)_data)->add_item();
    data->set_id (          atoi(argv[0])      );
    data->set_user(         atoi(argv[1])      );
    data->set_goods(        atoi(argv[2])      );
    data->set_count(        atoi(argv[3])      );
    return 0;
}
