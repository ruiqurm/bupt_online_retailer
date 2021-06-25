#include"server.h"
#include"protocol.h"
#include"database.h"
extern "C" {
#include"log.h"
}


string last_error;/// 记录运行中的错误信息

/********************
 *                  *
 *     Executor     *
 *                  *  
 ********************/
Executor::Executor(char* read_buffer,char* write_buffer,protoData::User*outer_user,int permission):
    read_buf(read_buffer),write_buf(write_buffer),write_data(write_buf+sizeof(Protocol)),
    read_data(read_buffer + sizeof(Protocol)),_is_finished(false),user(nullptr),
    _is_error(false){
    memcpy(write_buf,read_buf,8);// 复制id
    ((Protocol*)write_buf)->is_response = 1; // 标记报文为回复
    ((Protocol*)write_buf)->status = Protocol::OK; // 标记报文正常
    set_length(0);// 设置回复报文长度为0
    // if(outer_user)log_info("有用户");
    // else log_info("无用户");
    
    // 传入用户指针主要是为了完成这样的功能：对于每个连接，只要登录一次或者检验一次token；后面重新连接重新登录或者校验
    
    // 如果需要登录，并且没有传入用户指针
    if(!outer_user && permission == PERMISSION_USER){
        auto& record = UserRecordWriter::get_record();
        user = record.get_user_by_token(((Protocol*)read_buf)->token);
        if(user==nullptr){
            last_error = "authenticate failed";
            log_error("Authenticate error");
            set_error(Protocol::AUTHENTICATE_FAILED);
            _is_finished = true;
            _is_error = true;
        }
    }else if(outer_user!=nullptr){
        user = outer_user;
    }
}
void Executor::exec(){
    if(!_is_finished){
        execImp();
        _is_finished = true;
    }
}

void Executor::set_error(int type){
    ((Protocol*)write_buf)->status = type;
    int n = last_error.size();
    if(n){
        strcpy(read_data,last_error.c_str());
        set_length(n);
        _is_error = true;
        last_error.clear();
    }
}
void Executor::set_length(int length){
    if(length>=0){
        _size = ((Protocol*)write_buf)->length = length;
    }
}
void Executor::set_status(int type){
    ((Protocol*)write_buf)->status = type;
}

/********************
 *                  *
 *  UserExecutor    *
 *                  *  
 ********************/
void UserLoginExecutor::execImp(){
    auto& record = UserRecordWriter::get_record();
    protoData::UserForm form;
    form.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    string token = record.login(form);// 登录，并获取token
    if(token.size()>0){
        //成功
        memcpy(((Protocol*)write_buf)->token,token.c_str(),16); // 复制token到报文
        user = record.get_user(form.username());
        set_length(user->ByteSizeLong());
        user->SerializeToArray(write_data,user->ByteSizeLong()); // 写入用户信息到报文
        log_debug("token=%.16s username=%s,type=%d",token.c_str(),user->username().c_str(),user->type());
    }else{
        set_error(Protocol::RUN_FAILED);
        log_debug("登录失败. username=%s,password=%s",form.username().c_str(),form.password().c_str());
    }
}

void UserLogoutExecutor::execImp(){
    auto& record = UserRecordWriter::get_record();
    if (! record.logout(user)){
       set_error(Protocol::AUTHENTICATE_FAILED);
    }else{
        //成功
        set_status(Protocol::OK_LOGOUT);
        user =nullptr;
        memset(((Protocol*)write_buf)->token,0,16);
    }
}

void UserUpdateExecutor::execImp(){
    protoData::User update_data;
    update_data.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    // log_info("length=%d,username=%s,password=%s",((Protocol*)read_buf)->length,update_data.username().c_str(),update_data.password().c_str());
    update_data.set_id(user->id());
    update_data.set_username(user->username());
    auto& record = UserRecordWriter::get_record();
    log_info("length=%d,username=%s,password=%s",((Protocol*)read_buf)->length,update_data.username().c_str(),update_data.password().c_str());
    record.update(update_data);
    if(user->type()!=update_data.type() || user->password()!=update_data.password()){
        set_status(Protocol::OK_LOGOUT);
        record.logout(user);
        user = record.get_user(update_data.username());
        memset(((Protocol*)write_buf)->token,0,16);
    }
    user->SerializeToArray(write_data,user->ByteSizeLong());
    set_length((int)user->ByteSizeLong());
    
}
void UserRegisterExecutor::execImp(){
    auto& record = UserRecordWriter::get_record();
    log_info("register");
    protoData::UserForm form;
    form.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    if (!record._register(form)){
        set_error(Protocol::RUN_FAILED);
    }
}
void UserAuthenticatePasswordExecutor::execImp(){
    auto& record = UserRecordWriter::get_record();
    protoData::UserForm form;
    form.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    if (!record.check_password(user,form.password())){
        set_error(Protocol::AUTHENTICATE_FAILED);
    }
}
void UserInfoExecutor::execImp(){
    if(((Protocol*)read_buf)->length == 0){
        set_length(user->ByteSizeLong());
        user->SerializeToArray(write_data,user->ByteSizeLong());
        return;
    }
    auto& record = UserRecordWriter::get_record();
    protoData::User userdata;
    userdata.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    auto tmp = record.get_user(userdata.id());
    if(tmp!=nullptr){
        set_length(tmp->ByteSizeLong());
        tmp->SerializeToArray(write_data,tmp->ByteSizeLong());
    }else{
        set_length(0);
    }
    
    
}


/*
 *
 * Goods Executor 
 * 
 */
void GoodsCreateExecutor::execImp(){
    protoData::Goods goods;
    goods.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    GoodsDatabase database;
    int id=  database.create(goods);
    goods.set_id(id);
    log_debug("创建商品;报文长度%d\nid=%d\nname=%s\nprice=%f\ndescription=%s\ncount=%d",goods.ByteSizeLong(),goods.id(),goods.name().c_str(),goods.price(),goods.description().c_str(),goods.remain());
    goods.SerializeToArray(write_data,goods.ByteSizeLong());
    set_length(goods.ByteSizeLong());
}
void GoodsGetByIdExecutor::execImp(){
    protoData::Goods goods;
    goods.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    goods.set_remain(-1);
    if(goods.id()>0){
        GoodsDatabase database;
        goods.set_type(-1);
        database.get(goods);
        if (goods.remain() != -1){
            log_debug("获取到商品id=%d",goods.id());
            goods.SerializeToArray(write_data,goods.ByteSizeLong());
            set_length(goods.ByteSizeLong());
        }else{
            log_debug("未获取到商品id=%d",goods.id());
            set_length(0);
        }
    }else{
        set_error(Protocol::PARAMETER_ERROR);
    }
}
void GoodsGetByIDMultipleExecutor::execImp(){
    protoData::GoodsArray array;
    array.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    log_debug("获取多个商品");
    if(array.goods_size()>=0){
        GoodsDatabase database;
        database.get(array);
        array.SerializeToArray(write_data,array.ByteSizeLong());
        set_length(array.ByteSizeLong());
        log_debug("返回了%d个商品",array.goods_size());
    }else{
        set_error(Protocol::PARAMETER_ERROR);
    }
}
void GoodsGetBySellerExecutor::execImp(){
    protoData::Goods goods;
    protoData::GoodsArray array;
    goods.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    log_debug("获取商家id=%d的商品",goods.seller());
    if(goods.seller()>0){
        GoodsDatabase database;
        database.get_user_goods(goods.seller(),array);
        array.SerializeToArray(write_data,array.ByteSizeLong());
        set_length(array.ByteSizeLong());
        log_debug("返回了%d个商品",array.goods_size());
    }else{
        set_error(Protocol::PARAMETER_ERROR);
    }
}
void GoodsGetAllExecutor::execImp(){
    protoData::GoodsArray array;
    log_debug("获取全部商品");
    GoodsDatabase database;
    database.get_all_goods(array);
    array.SerializeToArray(write_data,array.ByteSizeLong());
    set_length(array.ByteSizeLong());
    log_debug("返回了%d个商品",array.goods_size());
}
void GoodsGetByNameExecutor::execImp(){
    protoData::Goods goods;
    protoData::GoodsArray array;
    goods.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    log_debug("获取名字和' %s '相近的商品",goods.name().c_str());
    if(goods.name().size()>=0){
        GoodsDatabase database;
        database.get_goods_by_name(goods.name(),array);
        array.SerializeToArray(write_data,array.ByteSizeLong());
        set_length(array.ByteSizeLong());
        log_debug("返回了%d个商品",array.goods_size());
    }else{
        set_error(Protocol::PARAMETER_ERROR);
    }
}
void GoodsUpdateExecutor::execImp(){
    protoData::Goods goods;
    goods.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    log_debug("更新商品\nid=%d\nname=%s\nprice=%f\ndescription=%s\ncount=%d",goods.id(),goods.name().c_str(),goods.price(),goods.description().c_str(),goods.remain());
    if(goods.id()>0){
        GoodsDatabase database;
        database.update(goods);
        goods.SerializeToArray(write_data,goods.ByteSizeLong());
        set_length(goods.ByteSizeLong());
    }else{
        set_error(Protocol::PARAMETER_ERROR);
    }
}
void GoodsRemoveExecutor::execImp(){
    protoData::Goods goods;
    goods.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    log_debug("删除商品");
    if(goods.id()>0){
        GoodsDatabase database;
        database.remove(goods);
    }else{
        set_error(Protocol::PARAMETER_ERROR);
    }
}


/*
 *
 * Discount Executor 
 * 
 */
void DiscountCreateExecutor::execImp(){
    protoData::Discount discount;
    discount.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    DiscountDatabase database;
    int id = database.create(discount);
    discount.set_id(id);
    log_debug("创建discount\n"\
             "id=%d\n"\
             "type=%d\n"\
             "operand=%d\n"\
             "discount=%f\n"\
             "threshold=%f\n",
    discount.id(),discount.type(),discount.operand(),discount.discount(),discount.threshold());
    discount.SerializeToArray(write_data,discount.ByteSizeLong());
    set_length(discount.ByteSizeLong());
}

void DisocuntUpdateExecutor::execImp(){
    protoData::Discount discount;
    discount.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    log_debug("更新discount\n"\
            "id=%d\n"\
            "type=%d\n"\
            "operand=%d\n"\
            "discount=%f\n"\
            "threshold=%f\n",
    discount.id(),discount.type(),discount.operand(),discount.discount(),discount.threshold());
    if(discount.id()>0){
        DiscountDatabase database;
        database.update(discount);
        discount.SerializeToArray(write_data,discount.ByteSizeLong());
        set_length(discount.ByteSizeLong());
    }else{
        set_error(Protocol::PARAMETER_ERROR);
    }
}
void DiscountGetAllCategoryExecutor::execImp(){
    protoData::Discount discount;
    discount.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    protoData::DiscountArray array;
    DiscountDatabase database;
    database.get_all_category_discount(discount,array);
    log_debug("获取类别为type=%d的所有记录共%d条",discount.type(),array.discount_size());
    array.SerializeToArray(write_data,array.ByteSizeLong());
    set_length(array.ByteSizeLong());
}
void DiscountGetGoodsDiscountExecutor::execImp(){
    protoData::Discount discount;
    discount.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    log_debug("获取关于商品%d的打折记录",discount.operand());
    DiscountDatabase database;
    discount.set_discount(-1);
    database.get_goods_discount(discount);
    if(discount.discount()>0){
        discount.SerializeToArray(write_data,discount.ByteSizeLong());
        set_length(discount.ByteSizeLong());
    }else{
        set_length(0);
    }
    
}
void DiscountGetCategoryDiscountExecutor::execImp(){
    protoData::Discount discount;
    discount.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    log_debug("获取关于商品类别%d的打折记录",discount.operand());
    DiscountDatabase database;
    discount.set_discount(-1);
    database.get_category_discount(discount);
    // discount
    if(discount.discount()>0){
        discount.SerializeToArray(write_data,discount.ByteSizeLong());
        set_length(discount.ByteSizeLong());
    }else{
        set_length(0);
    }
}
void DiscountRemoveByGoodsExecutor::execImp(){
    protoData::Discount discount;
    discount.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    log_debug("删除关于商品%d的打折记录",discount.operand());
    DiscountDatabase database;
    database.remove_by_goods(discount);
    // discount.SerializeToArray(write_data,discount.ByteSizeLong());
    // set_length(discount.ByteSizeLong());
}
void DiscountRemoveExecutor::execImp(){
    protoData::Discount discount;
    discount.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    log_debug("删除id为%d的打折记录",discount.id());
    DiscountDatabase database;
    database.remove(discount);
}
/*
 *
 * Transaction Executor 
 * 
 */
void TransactionGetExecutor::execImp(){
    protoData::Transaction transcation;
    transcation.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    if(transcation.id()>0){
        TransactionDatabase database;
        transcation.set_volume(-1);
        database.get(transcation);
        log_debug("获取id=%d的记录",transcation.id());
        if(transcation.volume()>0){
            transcation.SerializeToArray(write_data,transcation.ByteSizeLong());
            set_length(transcation.ByteSizeLong());
        }else{
            set_length(0);
        }
    }else{
        set_error(Protocol::PARAMETER_ERROR);
    }
}
void TransactionGetByUserExecutor::execImp(){
    protoData::Transaction transcation;
    protoData::TransactionArray array;
    transcation.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    TransactionDatabase database;
    database.get_transaction(transcation,array);
    log_debug("获取到和%d有关的记录共%d条",transcation.from(),array.transaction_size());
    array.SerializeToArray(write_data,array.ByteSizeLong());
    set_length(array.ByteSizeLong());
}
void TransactionSetExecutor::execImp(){
    protoData::Transaction transcation;
    transcation.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    TransactionDatabase database;
    int id = database.set(transcation);
    transcation.set_id(id);
    log_debug("创建新的transaction\n"\
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
    transcation.SerializeToArray(write_data,transcation.ByteSizeLong());
    set_length(transcation.ByteSizeLong());
}
void TransactionCancelExecutor::execImp(){
    protoData::Transaction transcation;
    transcation.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    TransactionDatabase database;
    log_debug("id=%d 的transaction取消订单",transcation.id());
    database.cancel(transcation);
}
void TransactionSetFinishedExecutor::execImp(){
    protoData::Transaction transcation;
    transcation.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    TransactionDatabase database;
    log_debug("id=%d 的transaction完成订单",transcation.id());
    database.set_finished(transcation);
}
/*
 *
 * Cart Executor 
 * 
 */
void CartGetExecutor::execImp(){
    protoData::CartItem item;
    item.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    CartDatabase database;
    protoData::Cart cart;
    database.get(item,cart);
    log_debug("获取用户%d的购物车数据共%d条",item.user(),cart.item_size());
    cart.SerializeToArray(write_data,cart.ByteSizeLong());
    set_length(cart.ByteSizeLong());
}
void CartSetExecutor::execImp(){
    protoData::CartItem item;
    item.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    CartDatabase database;
    int id = database.set(item);
    item.set_id(id);
    log_debug("添加购物车数据一条\n"\
              "id=%d\n"\
              "user=%d\n"\
              "goods=%d\n"\
              "count=%d",
    item.id(),item.user(),item.goods(),item.count());
    item.SerializeToArray(write_data,item.ByteSizeLong());
    set_length(item.ByteSizeLong());
}
void CartRemoveExecutor::execImp(){
    protoData::CartItem item;
    item.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    CartDatabase database;
    database.remove(item);
    log_debug("删除一条item id=%d",item.id());
}