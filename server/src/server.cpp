#include"server.h"
#include"protocol.h"

extern "C" {
#include"log.h"
}

//简单的异常保存
string last_error;

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
        last_error = "account has been registered";
        log_debug("账号已存在");
        return false;
    } 
    char*c= const_cast<char*>(form.username().c_str());
    int i=0;
    for(auto &c:form.username()){
        i++;
        if(isspace(c)){
            log_error("不允许空格");
            last_error = "disallow space character";
            return false;
        }
    }
    if(i>32){
        log_error("用户名过长");
        last_error = "username too long";
        return false;
    }
    for(auto&c:form.password()){
        i++;
        if(isspace(c)){
            log_error("不允许空格");
            last_error = "disallow space character";
            return false;
        }
    }  
    if(i>16){
        log_error("密码过长");
        last_error = "password too long";
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


/********************
 *                  *
 *     Executor     *
 *                  *  
 ********************/
Executor::Executor(char* read_buffer,char* write_buffer,protoData::User*outer_user,int permission):
    read_buf(read_buffer),write_buf(write_buffer),write_data(write_buf+sizeof(Protocol)),
    read_data(read_buffer + sizeof(Protocol)),_is_finished(false),user(nullptr),
    _is_error(false){
    memcpy(write_buf,read_buf,8);
    ((Protocol*)write_buf)->is_response = 1;
    ((Protocol*)write_buf)->status = Protocol::OK;
    set_length(0);
    // if(outer_user)log_info("有用户");
    // else log_info("无用户");
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
void UserLoginExecutor::execImp(){
    auto& record = UserRecordWriter::get_record();
    protoData::UserForm form;
    form.ParseFromArray(read_data,((Protocol*)read_buf)->length);
    string token = record.login(form);
    if(token.size()>0){
        //成功
        memcpy(((Protocol*)write_buf)->token,token.c_str(),16);
        user = record.get_user_by_username(form.username());
        set_length(user->ByteSizeLong());
        user->SerializeToArray(write_data,user->ByteSizeLong());
        log_debug("token=%.16s",token.c_str());
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
        user = record.get_user_by_username(update_data.username());
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
    set_length(user->ByteSizeLong());
    user->SerializeToArray(write_data,user->ByteSizeLong());
}