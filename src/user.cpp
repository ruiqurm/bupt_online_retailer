#include "user.h"

const std::regex User::USERNAME_PATTERN = std::regex("^[\\w,-_,!@#$%^&*()]{4,16}$");
const std::regex User::PASSWORD_PATTERN = std::regex("^[\\w,-_,!@#$%^&*()]{4,16}$");


bool User::register_(int type,const string& username,const string& password){
    auto& record = UserRecord::get_record();
    /// 若用户名存在，则返回失败
    if (record.get(username)){
        return false;
    }
    /// 若用户名中有空格，返回失败
    char*c= const_cast<char*>(username.c_str());
    int i=0;
    for(auto &c:username){
        i++;
        if(isspace(c)) throw "disallow space character in username";
    }
    /// 用户名长度大于32，返回失败
    if(i>32)throw "username too long";
    /// 若密码中有空格，返回失败
    for(auto&c:password){
        i++;
        if(isspace(c)) throw "disallow space character in username";
    }
    /// 密码长度大于16返回失败
    if(i>16)throw "password too long";

    // 检查用户名格式
    if(!validate_username_format(username)){
        throw "username syntax error";
    }
    //检查密码格式
    if(!validate_password_format(password)){
        throw "password syntax error";
    }
    record.set(type,username,password);
    return true;
}
int UserRecord::load(){
    int count=0,i=0;
    std::ifstream f(USER_FILE_NAME);
    /// 若map非空，则清空map内容
    if (!name_to_data.empty())name_to_data.clear();
    if (!pk_to_name.empty())pk_to_name.clear();
    UserData tmpdata;
    max_pk=0;
    // 读取文件内容
    while(f>>tmpdata.id){
        if(tmpdata.id!=0){
            f>>tmpdata.username>>tmpdata.password>>tmpdata.balance>>tmpdata.type;
            name_to_data.emplace(tmpdata.username,tmpdata);
            pk_to_name.emplace(tmpdata.id,tmpdata.username);
            max_pk = std::max(max_pk,tmpdata.id);
            count++;
        }
        f.seekg(i+MAX_LINE);
        i+=MAX_LINE;
    }
    return count;
}
void UserRecord::set(int type,const string& username,const string& password){
    max_pk++;
    UserData data(max_pk,username.c_str(),password.c_str(),0,type);
    name_to_data.emplace(username,data);// 保存 名字-数据 映射
    pk_to_name.emplace(max_pk,username);// 保存 id-名字   映射
    insert_data(data);
}
bool UserRecord::update(const UserData&data){
    auto it = name_to_data.find(data.username);
    if(it!=name_to_data.end()){
        if(data.id!=it->second.id)return false;///> 更改id则返回失败
        
        // 文件流操作
        memcpy(&(it->second),&data,sizeof(UserData));
        set_write_cursor_to_nth_line(data.id);
        insert_data(data);
        write_LF_nth_line(data.id);
        
        return true;
    }
    return false;
}
unsigned short UserRecord::register_type(unsigned short id, p_user_construct factoryMethod){
    #if DEBUG==1
        printf("Registering constructor for user id %d\n", id);
    #endif
    register_types[id] = factoryMethod;
    return id;
}

void UserRecord::clear(){
    //清空数据
    name_to_data.clear();
    pk_to_name.clear();
    max_pk = 0;
    database.close();
    database.open(USER_FILE_NAME,std::ios::out);
    database.close();
    database.open(USER_FILE_NAME,std::ios::in|std::ios::out);
}
bool UserData::save(){
    auto &record = UserRecord::get_record();
    return record.update(*this);
}
std::shared_ptr<User> User::login(const string& username,const string& password){
    auto& record = UserRecord::get_record();
    UserData* data = record.get(username);

    // data为空指针，说明用户不存在
    if(!data){
        throw "no exist";
    }
    
    // data的password错误
    if(data->password!=password){
        throw "password error";
    }
    return record.create_user(data);
}
bool User::save(){
    auto &record = UserRecord::get_record();
    #if DEBUG==1
    #include<iostream>
    cout<<"try to update user"<<endl;
    #endif
    return record.update(*data);
}
 