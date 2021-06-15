#include"transaction.h"

/**
 * 
 * 
 * 
 */


std::shared_ptr<Transaction> TransactionRecord::get(int id){
    ProtocolWriter writer(send_buf,Protocol::TRANSACTION_GET,base->token());
    protoData::Transaction tran;
    tran.set_id(id);
    writer.load(tran);
    ProtocolReader reader(recv_buf);
    if(base->send(writer,reader)>1){
       reader.get(tran);
       return std::make_shared<Transaction>(tran.id(),tran.from(),tran.from_name(),tran.to(),tran.to_name(),tran.finished(),
       tran.volume(),tran.detail(),tran.timestamp());
    }else{
        return nullptr;
    }
}
int TransactionRecord::set(Transaction&data){
    ProtocolWriter writer(send_buf,Protocol::TRANSACTION_SET,base->token());
    protoData::Transaction tran;
    tran.set_id(data.id());
    tran.set_from(data.from_id());
    tran.set_from_name(data.from_name());
    tran.set_to(data.to_id());
    tran.set_to_name(data.to_name());
    tran.set_finished(data.is_finished());
    tran.set_volume(data.volume());
    tran.set_detail(data.raw_detail());
    tran.set_timestamp(data.timestamp());
    writer.load(tran);
    ProtocolReader reader(recv_buf);
    if(base->send(writer,reader)){
       reader.get(tran);
       return tran.id();
    }else{
        return -1;
    }
}
void TransactionRecord::cancel(int id){   
    ProtocolWriter writer(send_buf,Protocol::TRANSACTION_CANCEL,base->token());
    protoData::Transaction tran;
    tran.set_finished(false);
    tran.set_id(id);
    writer.load(tran);
    ProtocolReader reader(recv_buf);
    base->send(writer,reader);
}
void TransactionRecord::set_finished(int id){
    ProtocolWriter writer(send_buf,Protocol::TRANSACTION_SET_FINISHED,base->token());
    protoData::Transaction tran;
    tran.set_finished(true);
    tran.set_id(id);
    writer.load(tran);
    ProtocolReader reader(recv_buf);
    base->send(writer,reader);
}
TransactionRecord::pTransVec TransactionRecord::get_transaction(int user_id,bool is_finished){
    ProtocolWriter writer(send_buf,Protocol::TRANSACTION_GET_BY_USER,base->token());
    protoData::Transaction tran;
    tran.set_finished(is_finished);
    tran.set_from(user_id);
    writer.load(tran);
    ProtocolReader reader(recv_buf);
    pTransVec vec = std::make_unique<std::vector<std::shared_ptr<Transaction>>>();
    if(base->send(writer,reader)){
        protoData::TransactionArray array;
        reader.get(array);
        int n =array.transaction_size();
        
        for(int i =0;i<n;i++){
            vec->push_back(
               std::make_shared<Transaction>(array.transaction(i).id(),array.transaction(i).from(),array.transaction(i).from_name(),array.transaction(i).to(),
               array.transaction(i).to_name(),array.transaction(i).finished(),
               array.transaction(i).volume(),array.transaction(i).detail(),array.transaction(i).timestamp())
            );
        }
        
    }
    return vec;
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
    std::cout<<"transaction_id="<<_id<<" is_fin="<<_finished<<std::endl;
    if(_finished==false&&_id>0){
        auto to_u = to();
        // std::cout<<to_u->id();
        if(to_u->balance() < _volume){
            std::cout<<"没有足够的钱";
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
std::map<int,int> CartRecord::get(int user_id){
    ProtocolWriter writer(send_buf,Protocol::CART_GET,base->token());
    protoData::CartItem item;
    item.set_user(user_id);
    writer.load(item);
    ProtocolReader reader(recv_buf);
    std::map<int,int> m;
    if(base->send(writer,reader)){
        protoData::Cart cart;
        reader.get(cart);
        int n = cart.item_size();
        for(int i=0;i<n;i++){
            m[cart.item(i).goods()] = cart.item(i).count();
        }
        
    }
    return m;
}

// void CartRecord::set(const std::map<int,int>&m){

// }
int CartRecord::set(int user_id,int goods_id,int num){
    ProtocolWriter writer(send_buf,Protocol::CART_SET,base->token());
    protoData::CartItem item;
    item.set_user(user_id);
    item.set_goods(goods_id);
    item.set_count(num);
    writer.load(item);
    ProtocolReader reader(recv_buf);
    if(base->send(writer,reader)){
        reader.get(item);
        return item.id();
    }else{
        return -1;
    }
}
void CartRecord::remove(int user_id,int goods_id){
    ProtocolWriter writer(send_buf,Protocol::CART_REMOVE,base->token());
    protoData::CartItem item;
    item.set_user(user_id);
    item.set_goods(goods_id);
    writer.load(item);
    ProtocolReader reader(recv_buf);
    base->send(writer,reader);
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