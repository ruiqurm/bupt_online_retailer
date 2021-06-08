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
bool UserRecordWriter::logout(const string& token){
    auto it =  token_to_id.find(token);
    if(it!=token_to_id.end()){
        int id = it->second;
        token_to_id.erase(it);
        id_to_token.erase(id);
        return true;
    }
    return false;
}

bool UserRecordWriter::_register(protoData::UserForm & form){
    /**
     * @brief 只检查是否存在，用户名和密码是否带空白符
     * 
     */
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
    log_info("注册账号成功");
    return true;
}
protoData::User* UserRecordWriter::get_user_by_token(const string& token){
    auto it =  token_to_id.find(token);
    if(it!=token_to_id.end()){
        auto iit = id_to_name.find(it->second);
        if(iit!=id_to_name.end()){
            auto iiit = name_to_data.find(iit->second);
            if(iiit!=name_to_data.end()){
                return &iiit->second;
            }
        }
    }
    log_error("无此用户");
    return nullptr;
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
    set_write_cursor_to_nth_line(data.id());
    database<<data.id()<<" "<<data.username()<<" "<<data.password()<<" "<<data.balance()<<" "<<data.type()<<"\0\0\0";
    write_LF_nth_line(data.id());
}
void UserRecordWriter::remove_data(int id){
    if(id<=0)return;
    //不判断是否存在了.
    set_write_cursor_to_nth_line(id);
    database<<"0\0";database.flush();
    // write_LF_nth_line(id);
}
string UserRecordWriter::generate_token(){
    char tmp_s[TOKEN_SIZE+1];
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    srand( (unsigned) time(NULL) * getpid());


    for (int i = 0; i < TOKEN_SIZE; ++i) 
        tmp_s[i] =  alphanum[rand() % (sizeof(alphanum) - 1)];
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
Executor::Executor(char* buffer,char* write_buf):
    read_buf(read_buf),write_buf(write_buf),data((char*)buffer + sizeof(Protocol)),_is_finished(false){
    memcpy(write_buf,read_buf,8);
    ((Protocol*)write_buf)->is_response = 1;
     ((Protocol*)write_buf)->status = Protocol::OK;
      _size = ((Protocol*)write_buf)->length = 0;
    if(permission == PERMISSION_USER){
        auto& record = UserRecordWriter::get_record();
        user = record.get_user_by_token(((Protocol*)read_buf)->token);
        if(user==nullptr){
            last_error = "authenticate failed";
            set_error(Protocol::AUTHENTICATE_FAILED);
            _is_finished = true;
        }
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
        strcpy(data,last_error.c_str());
        _size = ((Protocol*)write_buf)->length = n;
        last_error.clear();
    }
    
}
void UserLoginExecutor::execImp(){
    auto& record = UserRecordWriter::get_record();
    protoData::UserForm form;
    form.ParseFromArray(read_buf,((Protocol*)read_buf)->length);
    string token = record.login(form);
    if(token.size()>0){
        sprintf(data,"%s",token.c_str());
    }else{
        set_error(Protocol::RUN_FAILED);
    }
}

void UserLogoutExecutor::execImp(){
    auto& record = UserRecordWriter::get_record();
    if (! record.logout(((Protocol*)read_buf)->token)){
       set_error(Protocol::AUTHENTICATE_FAILED);
    }
}

void UserUpdateExecutor::execImp(){
    auto& record = UserRecordWriter::get_record();
    protoData::User update_data;
    update_data.ParseFromArray(read_buf,((Protocol*)read_buf)->length);
    if(user->id()==update_data.id()){
        record.update(update_data);
    }else{
        set_error(Protocol::PERMISSION_DENIED);
    }
    
}
void UserRegisterExecutor::execImp(){
    auto& record = UserRecordWriter::get_record();
    protoData::UserForm form;
    form.ParseFromArray(read_buf,((Protocol*)read_buf)->length);
    if (!record._register(form)){
        set_error(Protocol::RUN_FAILED);
    }
}
void UserAuthenticatePasswordExecutor::execImp(){
    auto& record = UserRecordWriter::get_record();
    protoData::UserForm form;
    form.ParseFromArray(read_buf,((Protocol*)read_buf)->length);
    if (!record._register(form)){
        set_error(Protocol::RUN_FAILED);
    }
}