#include "user.h"

const std::regex User::USERNAME_PATTERN = std::regex("^[\\w,-_,!@#$%^&*()]{4,16}$");
const std::regex User::PASSWORD_PATTERN = std::regex("^[\\w,-_,!@#$%^&*()]{4,16}$");


/*
* 
*/
bool User::register_(int type,const string& username,const string& password){
    auto& record = UserRecord::get_record();
    if (record.get(username)){
        return false;
    }
    char*c= const_cast<char*>(username.c_str());
    int i=0;
    for(auto &c:username){
        i++;
        if(isspace(c)) throw "disallow space character in username";
    }
    if(i>32)throw "username too long";
    for(auto&c:password){
        i++;
        if(isspace(c)) throw "disallow space character in username";
    }
    if(i>16)throw "password too long";

    if(!validate_username(username)){
        throw "username syntax error";
    }
    if(!validate_password(password)){
        throw "password syntax error";
    }
    record.set(type,username,password);
    return true;
}
int UserRecord::load(){
    int count=0,i=0;
    std::ifstream f(USER_FILE_NAME);
    if (!name_to_data.empty())name_to_data.clear();
    if (!pk_to_name.empty())pk_to_name.clear();
    UserData tmpdata;
    max_pk=0;
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
    name_to_data.emplace(username,data);
    pk_to_name.emplace(max_pk,username);
    insert_data(data);
}
bool UserRecord::update(const UserData&data){
    auto it = name_to_data.find(data.username);
    if(it!=name_to_data.end()){
        if(data.id!=it->second.id)return false;//尝试修改id
        it->second = data;
        set_write_cursor_to_nth_line(data.id);
        insert_data(data);
        write_LF_nth_line(data.id);
        return true;
    }
    return false;
}
unsigned short UserRecord::register_type(unsigned short id, p_user_construct factoryMethod){
    printf("Registering constructor for user id %d\n", id);
    register_types[id] = factoryMethod;
    return id;
}

void UserRecord::clear(){
    name_to_data.clear();
    pk_to_name.clear();
    max_pk = 0;
    database.close();
    database.open(USER_FILE_NAME,std::ios::out);
    database.close();
    database.open(USER_FILE_NAME,std::ios::in|std::ios::out);
}
std::unique_ptr<User> User::login(const string& username,const string& password){
    auto& record = UserRecord::get_record();
    UserData* data = record.get(username);
    if(!data){
        throw "no exist";
    }
    if(data->password!=password){
        throw "password error";
    }
    return record.create_user(data->type,*data);
}
bool User::save(){
    auto &record = UserRecord::get_record();
    return record.update(*data);
}
// UserRecord::UserRecord(){
//     if (file_exist(USER_FILE_NAME)){
//         load();
//     }
//     database.open(USER_FILE_NAME,std::ios::out);
// }
// User* UserRecord::get(int id){
//     return update(id);
// }
// const UserData* UserRecord::get(const std::string& username){
//     return update(username);
// }
// int UserRecord::set(UserData data){
//     if (exist(data.username)){
//         //不抛出异常
//         return -1;
//     }
//     char*c= data.username;
//     for(int i=0;*c!=0&&i<32;i++,c++){
//         if(i>=30)throw "username too long";
//         if(isspace(*c)) throw "disallow space character in username";
//     }
//     c= data.password;
//     for(int i=0;*c!=0&&i<16;i++,c++){
//         if(i>16)throw "password too long";
//         if(isspace(*c)) throw "disallow space character in password";
//     }
//     max_pk++;
//     data.id = max_pk;
//     name_to_data.emplace(data.username,data);
//     pk_to_name.emplace(max_pk,data.username);
//     return max_pk;
// }

// void UserRecord::remove(const std::string& username){
//     auto it = name_to_data.find(username);
//     if(it==name_to_data.end())return;
//     pk_to_name.erase(it->second.id);
//     name_to_data.erase(it);
// }
// void UserRecord::remove(int id){
//     auto it = pk_to_name.find(id);
//     if(it==pk_to_name.end())return;
//     name_to_data.erase(it->second);
//     pk_to_name.erase(it);
// }
// UserData* UserRecord::update(int id){
//     auto it = pk_to_name.find(id);
//     if(it==pk_to_name.end()){
//         return nullptr;
//     }else{
//         return &name_to_data.find(it->second)->second;
//     }
// }
// UserData* UserRecord::update(const std::string& username){
//     auto it = name_to_data.find(username);
//     if(it==name_to_data.end()){
//         return nullptr;
//     }else{
//         return &it->second;
//     }
// }
// int UserRecord::load(){
//     int count=0;
//     std::ifstream f(USER_FILE_NAME);
//     UserData tmpdata;
//     max_pk=0;
//     while(f>>tmpdata.id>>tmpdata.username>>tmpdata.password>>tmpdata.balance>>tmpdata.type){
//     name_to_data.emplace(tmpdata.username,tmpdata);
//     pk_to_name.emplace(tmpdata.id,tmpdata.username);
//     max_pk = std::max(max_pk,tmpdata.id);
//     count++;
//     }
//     return count;
// }
// void UserRecord::save(){
//     for(auto it=name_to_data.begin();it!=name_to_data.end();it++){
//         auto& data = it->second;
//         database<<data.id<<" "<<data.username<<" "<<data.password<<" "<<data.balance<<" "<<data.type<<std::endl;
//     }
//     database.flush();
// }

 
