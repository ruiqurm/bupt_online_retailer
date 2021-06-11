#include "protocol.h"
#include<ctime>
#include<cstring>
extern "C" {
#include"log.h"
}
ProtocolWriter::ProtocolWriter(char *_buffer,int type,const char* token):
    _size(0),buffer(_buffer){
    ((Protocol*)buffer)->transaction_id = time(NULL) % 65536;
    ((Protocol*)buffer)->type = type;
    ((Protocol*)buffer)->is_response = 0;
    ((Protocol*)buffer)->length = 0;//数据长度
    if(token!=nullptr)memcpy(((Protocol*)buffer)->token,token,16);

}
int ProtocolWriter::load(const protoData::Goods&goods){
    goods.SerializeToArray(buffer+sizeof(Protocol),goods.ByteSizeLong());
    add_length(goods.ByteSizeLong());
    
    return _size;
}
int ProtocolWriter::load(const protoData::GoodsArray& array){
    array.SerializeToArray(buffer+sizeof(Protocol),array.ByteSizeLong());
    add_length(array.ByteSizeLong());
    return _size;
}
int ProtocolWriter::load(const protoData::UserForm &form){
    form.SerializeToArray(buffer+sizeof(Protocol),form.ByteSizeLong());
    add_length(form.ByteSizeLong());
    return _size;
}
int ProtocolWriter::load(const protoData::User& user){
    user.SerializeToArray(buffer+sizeof(Protocol),user.ByteSizeLong());
    add_length(user.ByteSizeLong());
    return _size;
}
int ProtocolWriter::load(const protoData::CartItem&item){
    item.SerializeToArray(buffer+sizeof(Protocol),item.ByteSizeLong());
    add_length(item.ByteSizeLong());
    return _size;
}
int ProtocolWriter::load(const protoData::Transaction&transcation){
    transcation.SerializeToArray(buffer+sizeof(Protocol),transcation.ByteSizeLong());
    add_length(transcation.ByteSizeLong());
    return _size;
}
int ProtocolWriter::load(const protoData::Discount&discount){
    discount.SerializeToArray(buffer+sizeof(Protocol),discount.ByteSizeLong());
    add_length(discount.ByteSizeLong());
    return _size;
}

void ProtocolReader::log_header(){
    if(((Protocol*)buffer)->is_response){
    log_info("header 类型:回复\ntransaction id= %d\ntype=%s\nstatus=%s\nlength=%d\ntoken=%.16s",
        ((Protocol*)buffer)->transaction_id,protocol_type_to_str(((Protocol*)buffer)->type),protocol_status_to_str(((Protocol*)buffer)->status),
        ((Protocol*)buffer)->length,((Protocol*)buffer)->token);
    }else{
        log_info("header 类型:请求\ntransaction id= %d\ntype=%s\nlength=%d\ntoken=%.16s",
            ((Protocol*)buffer)->transaction_id,protocol_type_to_str(((Protocol*)buffer)->type),((Protocol*)buffer)->length,((Protocol*)buffer)->token);
    }
}
void ProtocolReader::get(protoData::Goods&goods){
    goods.ParseFromArray(buffer+sizeof(Protocol),((Protocol*)buffer)->length);
}
void ProtocolReader::get(protoData::GoodsArray& array){
    array.ParseFromArray(buffer+sizeof(Protocol),((Protocol*)buffer)->length);
}
void ProtocolReader::get(protoData::UserForm &form){
    form.ParseFromArray(buffer+sizeof(Protocol),((Protocol*)buffer)->length);
}
void ProtocolReader::get(protoData::User& user){
    user.ParseFromArray(buffer+sizeof(Protocol),((Protocol*)buffer)->length);
}
void ProtocolReader::get(protoData::CartItem&item){
    item.ParseFromArray(buffer+sizeof(Protocol),((Protocol*)buffer)->length);
}
void ProtocolReader::get(protoData::Cart&cart){
    cart.ParseFromArray(buffer+sizeof(Protocol),((Protocol*)buffer)->length);
}
void ProtocolReader::get(protoData::Transaction&transcation){
    transcation.ParseFromArray(buffer+sizeof(Protocol),((Protocol*)buffer)->length);
}
void ProtocolReader::get(protoData::TransactionArray&array){
    array.ParseFromArray(buffer+sizeof(Protocol),((Protocol*)buffer)->length);
}
void ProtocolReader::get(protoData::Discount&discount){
    discount.ParseFromArray(buffer+sizeof(Protocol),((Protocol*)buffer)->length);
}
void ProtocolReader::get(protoData::DiscountArray&array){
    array.ParseFromArray(buffer+sizeof(Protocol),((Protocol*)buffer)->length);
}

void ProtocolReader::log_data(const protoData::Goods&goods){
    log_debug("\nid=%d\nname:%s\nprice=%f\ndescription=%s\nremain=%f\nseller=%d",
    goods.id(),
    goods.name().c_str(),
    goods.price(),
    goods.description().c_str(),
    goods.remain(),
    goods.seller()
    );
}
void ProtocolReader::log_data(protoData::GoodsArray& goods){
    int n = goods.goods_size();
    for(int i =0;i<n;i++){
        log_debug("\nid=%d\nname:%s\nprice=%f\ndescription=%s\nremain=%f\nseller=%d",
        goods.goods()[i].id(),
        goods.goods()[i].name().c_str(),
        goods.goods()[i].price(),
        goods.goods()[i].description().c_str(),
        goods.goods()[i].remain(),
        goods.goods()[i].seller()
       );
    }   
}

void ProtocolReader::log_data(protoData::Transaction&transcation){
    log_debug("transaction\n"\
              "id=%d\n"\
              "from=%d\n"\
              "from_name=%s\n"\
              "to=%d\n"\
              "to_name=%s\n"\
              "finished=%d\n"\
              "volume=%f\n"\
              "detail=%s\n"\
              "timestamp=%ld",
     transcation.id(),
     transcation.from(),transcation.from_name().c_str(),transcation.to(),transcation.to_name().c_str(),
     transcation.finished(),transcation.volume(),transcation.detail().c_str(),
     transcation.timestamp());
}
void ProtocolReader::log_data(protoData::TransactionArray&array){
    int n = array.transaction_size();
    for(int i =0;i<n;i++){
    log_debug("transaction\n"\
              "id=%d\n"\
              "from=%d\n"\
              "from_name=%s\n"\
              "to=%d\n"\
              "to_name=%s\n"\
              "finished=%d\n"\
              "volume=%f\n"\
              "detail=%s\n"\
              "timestamp=%ld",
     array.transaction(i).id(),
     array.transaction(i).from(),array.transaction(i).from_name().c_str(),array.transaction(i).to(),array.transaction(i).to_name().c_str(),
     array.transaction(i).finished(),array.transaction(i).volume(),array.transaction(i).detail().c_str(),
     array.transaction(i).timestamp());
    }   
}
void ProtocolReader::log_data(protoData::Discount&discount){
    log_debug("discount\n"\
             "id=%d\n"\
             "type=%d\n"\
             "operand=%d\n"\
             "discount=%f\n"\
             "threshold=%f\n",
    discount.id(),
    discount.type(),discount.operand(),discount.discount(),discount.threshold());
}
void ProtocolReader::log_data(protoData::DiscountArray&array){
    int n = array.discount_size();
    for(int i =0;i<n;i++){
    log_debug("discount\n"\
             "id=%d\n"\
             "type=%d\n"\
             "operand=%d\n"\
             "discount=%f\n"\
             "threshold=%f\n",
            array.discount(i).id(),
            array.discount(i).type(),
            array.discount(i).operand(),
            array.discount(i).discount(),
            array.discount(i).threshold());
    }   
}
void ProtocolReader::log_data(protoData::Cart&cart){
    int n = cart.item_size();
    for(int i =0;i<n;i++){
    log_debug("购物车数据\n"\
              "id=%d\n"\
              "user=%d\n"\
              "goods=%d\n"\
              "count=%d",
    cart.item(i).id(),cart.item(i).user(),cart.item(i).goods(),cart.item(i).count());
    }   
}
void ProtocolReader::log_data(protoData::CartItem&item){
    log_debug("购物车数据\n"\
              "id=%d\n"\
              "user=%d\n"\
              "goods=%d\n"\
              "count=%d",
    item.id(),item.user(),item.goods(),item.count());
}

// void log_data(protoData::UserForm &form){
//     log_debug("username:%s\nprice=%f\ndescription=%s\nremain=%s\nseller=%d",
//     goods.name().c_str(),
//     goods.price(),
//     goods.description().c_str(),
//     goods.remain(),
//     goods.seller()
//     );
// }
// void log_data(protoData::User& user);