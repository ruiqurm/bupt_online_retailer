#include"transaction.h"

/**
 * 
 * 
 * 
 */

TransactionRecord::TransactionRecord():db(Database::get_db()){
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
    Database::exec(db,sql,nullptr,nullptr);
}
std::shared_ptr<Transaction> TransactionRecord::get(int id){
    static const char sql[] = "SELECT * FROM _TRANSACTION WHERE ID=%d";
    static char buffer[64]; 
    Transaction*d;

    sprintf(buffer,sql,id);
    Database::exec(db,buffer,fetch_to_object,&d);
    return std::shared_ptr<Transaction>(d);
}
int TransactionRecord::set(Transaction&data){
    static char buffer[1024];
    static const char sql[] = "INSERT INTO _TRANSACTION (_FROM,_FROM_NAME,_TO,_TO_NAME,FINISHED,VOLUME,DETAIL,TIMESTAMP)"\
                              "VALUES(%d,'%s',%d,'%s',%d,%f,'%s',%ld); ";
    sprintf(buffer,sql,data.from_id(),data.c_from_name(),data.to_id(),data.c_to_name(),((data.is_finished())?1:0),
    data.volume(),data.raw_detail().c_str(),data.timestamp());
    Database::exec(db,buffer,nullptr,nullptr);
    return sqlite3_last_insert_rowid(db);
}
int TransactionRecord::fetch_to_object(void*_data, int argc, char **argv, char **azColName){
    if(argc!=9){(*((Transaction**)_data))=nullptr;return -1;}
    Transaction* &data = (*(Transaction**)_data);
    data = new Transaction( atoi(argv[0]),
                            atoi(argv[1]),
                            argv[2],
                            atoi(argv[3]),
                            argv[4],
                            atoi(argv[5]),
                            atof(argv[6]),
                            argv[7],
                            atoll(argv[8]));
    // std::cout<<data->from_id()<<std::endl;
    return 0;
}
int TransactionRecord::fetch_to_vector(void*_data, int argc, char **argv, char **azColName){
    if(argc!=9){ *( (std::unique_ptr<std::vector<std::shared_ptr<Transaction>>>*)_data)=nullptr;return -1;}
    std::unique_ptr<std::vector<std::shared_ptr<Transaction>>>&data = *( (std::unique_ptr<std::vector<std::shared_ptr<Transaction>>>*)_data);
    data->push_back(std::make_shared<Transaction>(  atoi(argv[0]),
                                                    atoi(argv[1]),
                                                    argv[2],
                                                    atoi(argv[3]),
                                                    argv[4],
                                                    atoi(argv[5]),
                                                    atof(argv[6]),
                                                    argv[7],
                                                    atoll(argv[8])));
    return 0;
}
void TransactionRecord::cancel(int id){
    static const char sql[] = "DELETE FROM _TRANSACTION WHERE ID=%d AND FINISHED=0;";
    static char buffer[64];
    sprintf(buffer,sql,id);
    Database::exec(db,buffer,nullptr,nullptr);
}
void TransactionRecord::set_finished(int id){
    static const char sql[] = "UPDATE _TRANSACTION SET FINISHED=1 WHERE ID=%d";
    static char buffer[64];
    sprintf(buffer,sql,id);
    Database::exec(db,buffer,nullptr,nullptr);
}
TransactionRecord::pTransVec TransactionRecord::get_transaction(int user_id,bool is_finished){
    static char buffer[96];
    static const char sql[] = "SELECT * FROM _TRANSACTION WHERE FINISHED = %d AND (_FROM = %d OR _TO = %d);";
    auto pvec = std::make_unique<std::vector<std::shared_ptr<Transaction>>>();
    sprintf(buffer,sql,is_finished?1:0,user_id,user_id);
    Database::exec(db,buffer,fetch_to_vector,&pvec);
    return pvec;
}

/**
 * 
 * Transaction
 * 
 */
void Transaction::cancel(){
    if(!_finished&&_id>0){
        auto &record = GoodsRecord::get_record();
        auto m = detail();
        for(auto&it:*m){
            auto g = record.get(it.first);
            if(g!=nullptr){
                g->remain() +=  std::get<1>(it.second);
                g->save();
            }
        }
        auto&record2 = TransactionRecord::get_record();
        record2.cancel(_id);
    }
}
int Transaction::save(){
    auto&record = TransactionRecord::get_record();
    return record.set(*this);
}
const std::shared_ptr<std::map<int,std::tuple<string,int,double>>> Transaction::detail(){
    if(!_detail_map){
        // std::cout<<_detail<<std::endl;
        std::stringstream ss(_detail);
        int n;
        ss>>n;
        // std::cout<<n<<std::endl;
        _detail_map = std::make_shared<std::map<int,std::tuple<string,int,double>>>();
        int tmp1;
        string tmp2;
        int tmp3;
        double tmp4;
        for(int i=0;i<n;i++){
            ss>>tmp1;ss.ignore(1);
            getline(ss,tmp2);
            ss>>tmp3>>tmp4;
            // std::cout<<"id="<<tmp1<<" s="<<tmp2<<" num="<<tmp3<<" price="<<tmp4<<std::endl;
            (*_detail_map)[tmp1] = std::make_tuple(tmp2,tmp3,tmp4);
        }
    }
    return _detail_map;
    
}
string Transaction::write_detail(const std::map<int,std::tuple<string,int,double>>&data){
    std::stringstream ss;
    ss<<data.size()<<'\n';
    for(auto&it:data){
        ss<<it.first<<'\n'<<std::get<0>(it.second)<<'\n'<<std::get<1>(it.second)<<'\n'<<std::get<2>(it.second)<<'\n';
    }
    return ss.str();
}

bool Transaction::set_finished(){
    if(_finished==false&&_id>0){
        auto to_u = to();
        // std::cout<<to_u->id();
        if(to_u->balance() < _volume){
;
            return false;
        }
        _finished=true;
        auto&record = TransactionRecord::get_record();
        record.set_finished(_id);
        auto from_u = from();
        from_u->balance() += _volume;
        to_u ->balance()  -= _volume;
        from_u->save();
        to_u->save();
        return true;
    }
    return false;
}

/**
 * @brief CartRecord implemention
 * 
 */
CartRecord::CartRecord():db(Database::get_db()){
    static const char sql[]= "CREATE TABLE CART("\
                            "ID INTEGER PRIMARY KEY AUTOINCREMENT,"\
                            "USER           INT       NOT NULL,"\
                            "GOODS          INT       NOT NULL,"\
                            "COUNT          INT       NOT NULL,"\
                            "UNIQUE(USER,GOODS) ON CONFLICT REPLACE);";
    Database::exec(db,sql,nullptr,nullptr);
}
std::map<int,int> CartRecord::get(int user_id){
    static const char sql[] = "SELECT GOODS,COUNT FROM CART WHERE USER=%d";
    static char buffer[64];
    std::map<int,int> m;
    sprintf(buffer,sql,user_id);
    Database::exec(db,buffer,fetch_to_map,&m);
    return m;
}

// void CartRecord::set(const std::map<int,int>&m){

// }
int CartRecord::set(int user_id,int goods_id,int num){
    static char buffer[128];
    static const char sql[] = "INSERT INTO CART (USER,GOODS,COUNT)"\
                              "VALUES(%d,%d,%d); ";
    sprintf(buffer,sql,user_id,goods_id,num);
    Database::exec(db,buffer,nullptr,nullptr);
    return sqlite3_last_insert_rowid(db);
}
void CartRecord::remove(int user_id,int goods_id){
    static char buffer[128];
    static const char sql[] = "DELETE FROM CART WHERE USER=%d AND GOODS=%d;";
    sprintf(buffer,sql,user_id,goods_id);
    Database::exec(db,buffer,nullptr,nullptr);
}

int CartRecord::fetch_to_map(void*_data, int argc, char **argv, char **azColName){
    if(argc!=2){return -1;}
    std::map<int,int>& data = *static_cast<std::map<int,int>*>(_data);
    data[atoi(argv[0])] += atoi(argv[1]);//map默认值为0,虽然不允许重复，但是这样写也没错
    //argv[0]是id
    //argv[1]是用户
    //argv[2]是商品
    //argv[3]是数量
    return 0;
}
/**
 * @brief Cart implemention
 * 
 */
Cart::Cart(int user_id){
    user = user_id;
    // auto & record = UserRecord::get_record();
    // user = record.create_user(record.get(user_id));
    auto& crecord = CartRecord::get_record();
    _data = crecord.get(user_id);
}

bool Cart::exist(int goods_id){
    auto it = _data.find(goods_id);
    return (it!=_data.end());
}
//添加
bool Cart::add(int goods_id,int delta,int max_count){
    auto it = _data.find(goods_id);
    auto& crecord = CartRecord::get_record();
    bool ok = true;
    if(it!=_data.end()){
        if(max_count>0&&it->second+delta>max_count){
             crecord.set(user,goods_id,max_count);
            _data[goods_id] = max_count;
            ok=false;
        }else{
            crecord.set(user,goods_id,it->second+delta);
            _data[goods_id] = it->second+delta;
        }
                
    }else{
       crecord.set(user,goods_id,delta);
       _data[goods_id] = delta;
    }
    return ok;
}
void Cart::add(int goods_id,int delta){
    add(goods_id,delta,-1);
}
int Cart::operator [](int goods_id)const{
    auto it = _data.find(goods_id);
    if(it!=_data.end()){
        return it->second;
    }else{
        return -1;
    }
}
void Cart::reduce(int goods_id,int delta){
    auto it = _data.find(goods_id);
    if(it!=_data.end()){
        auto& crecord = CartRecord::get_record();
        int v = it->second - delta;
        // printf("goods_id = %d , %d\n",goods_id,v);
        if(v <=0){
            crecord.remove(user,goods_id);
            _data.erase(goods_id);
        }else{
            crecord.set(user,goods_id,v);
            _data[goods_id] = v;
        }    
    }
}
void Cart::clear(){
    for(auto&i:_data){
        auto& crecord = CartRecord::get_record();
        crecord.remove(user,i.first);
    }
    _data.clear();
}
void Cart::remove(int goods_id){
    auto& crecord = CartRecord::get_record();
   crecord.remove(user,goods_id); 
}
const std::map<int,int>& Cart::data()const{
    return _data;
}
std::map<int,int>& Cart::data(){
    return _data;
}
bool Cart::set(int goods_id,int num,int max_count){
    auto it = _data.find(goods_id);
    auto& crecord = CartRecord::get_record();
    if(it!=_data.end()){
        if(max_count>0&&num>max_count){
             crecord.set(user,goods_id,max_count);
            _data[goods_id] = max_count;
        }else if(num<=0){
            crecord.remove(user,goods_id);
            _data.erase(goods_id);
            return false;
        }else{
            crecord.set(user,goods_id,num);
            _data[goods_id] = num;
        }
                
    }else{
       crecord.set(user,goods_id,num);
       _data[goods_id] = num;
    }
    return true;
}