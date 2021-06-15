#include"goods.h"

/***
 * 
 * Goods
 * 
 */
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

std::shared_ptr<Goods> GoodsRecord::get(int id){
    ProtocolWriter writer(send_buf,Protocol::GOOOS_GET_BY_ID,base->token());
    protoData::Goods goods;
    goods.set_id(id);
    writer.load(goods);
    ProtocolReader reader(recv_buf);
    if(base->send(writer,reader) > 1){
        reader.get(goods);
        GoodsData data(goods.id(),goods.name().c_str(),goods.price(),goods.seller(),
        goods.type(),goods.remain(),goods.description().c_str());
        return register_types[goods.type()](data);
    }else{
        return nullptr;
    }
}

GoodsRecord::pGoodsVec GoodsRecord::get(const std::vector<int>&l){
    if(l.size()==0)return std::make_unique<std::vector<std::shared_ptr<Goods>>>();
    ProtocolWriter writer(send_buf,Protocol::GOODS_GET_BY_ID_MULITIPLE,base->token());
    protoData::GoodsArray array;
    for(auto&t:l){
        (array.add_goods())->set_id(t);
    }
    writer.load(array);
    ProtocolReader reader(recv_buf);
    auto pvec = std::make_unique<std::vector<std::shared_ptr<Goods>>>();
    if(base->send(writer,reader)){
        reader.get(array);
        int n = array.goods_size();
        for(int i=0;i<n;i++){
           GoodsData data(array.goods(i).id(),array.goods(i).name().c_str(),array.goods(i).price(),array.goods(i).seller(),
        array.goods(i).type(),array.goods(i).remain(),array.goods(i).description().c_str());
            pvec->push_back(
                register_types[array.goods(i).type()](data)
            );
        }
        
    }
    return pvec;
}
GoodsRecord::pGoodsVec GoodsRecord::get_user_goods(int seller_id){
    ProtocolWriter writer(send_buf,Protocol::GOODS_GET_BY_SELLER,base->token());
    protoData::Goods goods;
    goods.set_seller(seller_id);
    writer.load(goods);
    ProtocolReader reader(recv_buf);
    auto pvec = std::make_unique<std::vector<std::shared_ptr<Goods>>>();
    if(base->send(writer,reader)){
        protoData::GoodsArray array;
        reader.get(array);
        int n = array.goods_size();
        for(int i=0;i<n;i++){
           GoodsData data(array.goods(i).id(),array.goods(i).name().c_str(),array.goods(i).price(),array.goods(i).seller(),
        array.goods(i).type(),array.goods(i).remain(),array.goods(i).description().c_str());
            pvec->push_back(
                register_types[array.goods(i).type()](data)
            );
        }
        
    }
    return pvec;
}

GoodsRecord::pGoodsVec GoodsRecord::get_all_goods(){
    ProtocolWriter writer(send_buf,Protocol::GOODS_GET_ALL,base->token());
    ProtocolReader reader(recv_buf);
    auto pvec = std::make_unique<std::vector<std::shared_ptr<Goods>>>();
    if(base->send(writer,reader)>1){
        protoData::GoodsArray array;
        reader.get(array);
        int n = array.goods_size();
        printf("n=%d",n);
        for(int i=0;i<n;i++){
           GoodsData data(array.goods(i).id(),array.goods(i).name().c_str(),array.goods(i).price(),array.goods(i).seller(),
        array.goods(i).type(),array.goods(i).remain(),array.goods(i).description().c_str());
            pvec->push_back(
                register_types[array.goods(i).type()](data)
            );
        } 
    }
    return pvec;
}
GoodsRecord::pGoodsVec GoodsRecord::get_goods_by_name(const string& search_for_name){
    ProtocolWriter writer(send_buf,Protocol::GOODS_GET_BY_NAME,base->token());
    protoData::Goods goods;
    goods.set_name(search_for_name);
    writer.load(goods);

    ProtocolReader reader(recv_buf);
    auto pvec = std::make_unique<std::vector<std::shared_ptr<Goods>>>();
    if(base->send(writer,reader)){
        protoData::GoodsArray array;
        reader.get(array);
        int n = array.goods_size();
        for(int i=0;i<n;i++){
           GoodsData data(array.goods(i).id(),array.goods(i).name().c_str(),array.goods(i).price(),array.goods(i).seller(),
        array.goods(i).type(),array.goods(i).remain(),array.goods(i).description().c_str());
            pvec->push_back(
                register_types[array.goods(i).type()](data)
            );
        }   
    }
    return pvec;
}
int GoodsRecord::set(GoodsData& data){
    ProtocolWriter writer(send_buf,Protocol::GOOOS_CREATE,base->token());
    protoData::Goods goods;
    goods.set_id(-12);
    goods.set_name(data.name);
    goods.set_price(data.price);
    goods.set_remain(data.remain);
    goods.set_seller(data.seller);
    goods.set_type(data.type);
    goods.set_description(data.description);

    writer.load(goods);

    ProtocolReader reader(recv_buf);

    if(base->send(writer,reader)>1){
        reader.get(goods);
        // std::cout<<goods.name()<<std::endl;
        return goods.id();
    }else{
        return -1;
    } 
}
bool GoodsRecord::update(const GoodsData& data){
    ProtocolWriter writer(send_buf,Protocol::GOODS_UPDATE,base->token());
    protoData::Goods goods;
    goods.set_id(data.id);
    goods.set_name(data.name);
    goods.set_price(data.price);
    goods.set_remain(data.remain);
    goods.set_seller(data.seller);
    goods.set_type(data.type);
    goods.set_description(data.description);
    writer.load(goods);

    ProtocolReader reader(recv_buf);

    return base->send(writer,reader);
}
void GoodsRecord::remove(int id){
    ProtocolWriter writer(send_buf,Protocol::GOODS_REMOVE,base->token());
    protoData::Goods goods;
    goods.set_id(id);
    writer.load(goods);

    ProtocolReader reader(recv_buf);

    base->send(writer,reader);
}



/*
*
* Discount
*
*/
bool DiscountRecord::make_discount(unsigned char type,int operand,double discount,double threshold){
    ProtocolWriter writer(send_buf,Protocol::DISCOUNT_CREATE,base->token());
    protoData::Discount dis;
    dis.set_discount(discount);
    dis.set_type(type);
    dis.set_operand(operand);
    dis.set_threshold(threshold);

    writer.load(dis);
    ProtocolReader reader(recv_buf);
    return base->send(writer,reader);
}
void DiscountRecord::update_discount(int id,double discount,double threshold){
    ProtocolWriter writer(send_buf,Protocol::DISOCUNT_UPDATE,base->token());
    protoData::Discount dis;
    dis.set_id(id);
    dis.set_discount(discount);
    dis.set_threshold(threshold);
    writer.load(dis);
    ProtocolReader reader(recv_buf);
    base->send(writer,reader);
}

std::shared_ptr<Discount> DiscountRecord::get_category_discount(int category){
     ProtocolWriter writer(send_buf,Protocol::DISCOUNT_GET_CATEGORY_DISCOUNT,base->token());
    protoData::Discount dis;
    dis.set_type(Discount::TYPE_DISCOUNT);
    dis.set_operand(category);

    writer.load(dis);
    ProtocolReader reader(recv_buf);
    if(base->send(writer,reader)>1){
        reader.get(dis);
        switch (dis.type()){
        case Discount::DISCOUNT:
            return std::make_shared<DiscountSimple>(dis.id(),dis.type(),dis.operand(),dis.discount(),dis.threshold());
        case Discount::TYPE_DISCOUNT:
            return std::make_shared<DiscountCategory>(dis.id(),dis.type(),dis.operand(),dis.discount(),dis.threshold());
        default:
            break;
        }
    }
    return nullptr;
}
pDisVec DiscountRecord::get_all_category_discount(){
    ProtocolWriter writer(send_buf,Protocol::DISCOUNT_GET_ALL_CATEGORY,base->token());
    protoData::Discount dis;
    dis.set_type(Discount::TYPE_DISCOUNT);

    writer.load(dis);
    ProtocolReader reader(recv_buf);
    auto pvec = std::make_unique<std::vector<std::shared_ptr<Discount>>>();
    if(base->send(writer,reader)){
        protoData::DiscountArray array;
        reader.get(array);
        int n = array.discount_size();
        for(int i =0;i<n;i++){
            switch (array.discount(i).type()){
            case Discount::DISCOUNT:
                pvec->push_back(std::make_shared<DiscountSimple>(array.discount(i).id(),array.discount(i).type(),array.discount(i).operand(),array.discount(i).discount(),array.discount(i).threshold()));
            case Discount::TYPE_DISCOUNT:
                pvec->push_back(std::make_shared<DiscountCategory>(array.discount(i).id(),array.discount(i).type(),array.discount(i).operand(),array.discount(i).discount(),array.discount(i).threshold()));
            }
        }
    }
    return pvec;
}
std::shared_ptr<Discount> DiscountRecord::get_goods_discount(int goods_id){
     ProtocolWriter writer(send_buf,Protocol::DISCOUNT_GET_GOODS_DISCOUNT,base->token());
    protoData::Discount dis;
    dis.set_type(Discount::DISCOUNT);
    dis.set_operand(goods_id);

    writer.load(dis);
    ProtocolReader reader(recv_buf);
    if(base->send(writer,reader)>1){
        reader.get(dis);
        switch (dis.type()){
        case Discount::DISCOUNT:
            return std::make_shared<DiscountSimple>(dis.id(),dis.type(),dis.operand(),dis.discount(),dis.threshold());
        case Discount::TYPE_DISCOUNT:
            return std::make_shared<DiscountCategory>(dis.id(),dis.type(),dis.operand(),dis.discount(),dis.threshold());
        default:
            break;
        }
    }
    return nullptr;
}

void DiscountRecord::remove(int id){
    ProtocolWriter writer(send_buf,Protocol::DISCOUNT_REMOVE,base->token());
    protoData::Discount dis;
    dis.set_id(id);
    // dis.set_operand(goods_id);

    writer.load(dis);
    ProtocolReader reader(recv_buf);
    base->send(writer,reader);
}
void DiscountRecord::remove_by_goods(int goods_id){
    ProtocolWriter writer(send_buf,Protocol::DISCOUNT_REMOVE_BY_GOODS,base->token());
    protoData::Discount dis;
    dis.set_operand(goods_id);
    // dis.set_operand(goods_id);

    writer.load(dis);
    ProtocolReader reader(recv_buf);
    base->send(writer,reader);
}
