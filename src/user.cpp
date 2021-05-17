#include "user.h"

const std::regex UserManager::USERNAME_PATTERN = std::regex("^[\\w,-_,!@#$%^&*()]{4,16}$");
const std::regex UserManager::PASSWORD_PATTERN = std::regex("^[\\w,-_,!@#$%^&*()]{4,16}$");


/*
* 
*/
bool UserManager::register_(USER_TYPE type,const string& username,const string& password){
    if (get_userdata(username)){
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
    max_pk++;
    UserData data(max_pk,username.c_str(),password.c_str(),0,type);
    name_to_data.emplace(username,data);
    pk_to_name.emplace(max_pk,username);
    insert_data(data);
    return true;
}
int UserManager::load(){
    int count=0,i=0;
    std::ifstream f(USER_FILE_NAME);
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

 
