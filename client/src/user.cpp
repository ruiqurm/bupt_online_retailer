#include "user.h"
#include "protoData.pb.h"
#include"protocol.h"
const std::regex User::USERNAME_PATTERN = std::regex("^[\\w,-_,!@#$%^&*()]{4,16}$");
const std::regex User::PASSWORD_PATTERN = std::regex("^[\\w,-_,!@#$%^&*()]{4,16}$");


/*
* 
*/
bool User::register_(int type,const string& username,const string& password){
    
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

    if(!validate_username_format(username)){
        throw "username syntax error";
    }
    if(!validate_password_format(password)){
        throw "password syntax error";
    }
    auto& record = UserRecord::get_record();
    record.set(type,username,password);
    return true;
}
unsigned short UserRecord::register_type(unsigned short id, p_user_construct factoryMethod){
    #if DEBUG==1
        printf("Registering constructor for user id %d\n", id);
    #endif
    register_types[id] = factoryMethod;
    return id;
}

// void UserRecord::clear(){
//     name_to_data.clear();
//     pk_to_name.clear();
//     max_pk = 0;
//     database.close();
//     database.open(USER_FILE_NAME,std::ios::out);
//     database.close();
//     database.open(USER_FILE_NAME,std::ios::in|std::ios::out);
// }
bool UserData::save(){
    auto &record = UserRecord::get_record();
    return record.update(*this);
}
std::shared_ptr<User> User::login(const string& username,const string& password){
    auto& record = UserRecord::get_record();
    UserData* data = record.login(username,password);
    if(!data){
        throw "no exist";
    }
    if(data->id<0){
        delete data;
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
 

/*
*
* UserRecord
*
*/

// void UserRecord::remove(int id){
//     ProtocolWriter writer(send_buf,Protocol::USER)
//     auto it = pk_to_name.find(id);
//     if(it==pk_to_name.end())return;
//     name_to_data.erase(it->second);
//     pk_to_name.erase(id);
//     remove_data(id);
// }
// void UserRecord::remove(const std::string& username){
//     auto it = name_to_data.find(username);
//     if(it==name_to_data.end())return;
//     int id = it->second.id;
//     pk_to_name.erase(it->second.id);
//     name_to_data.erase(it);
//     remove_data(id);
// }
// UserData* UserRecord::get(const string&username){
//     ProtocolWriter writer(send_buf,Protocol::USER_INFO,base->token());
//     protoData::User u;
//     u.username()
//     writer.load(u);
//     ProtocolReader reader(recv_buf);
//     if(base->send(writer,reader)){
//        reader.get(u);
//        return u.username();
//     }else{
//         return string();
//     }
// }
void UserRecord::logout(){
    ProtocolWriter writer(send_buf,Protocol::USER_LOGOUT,base->token());
    ProtocolReader reader(recv_buf);
    base->send(writer,reader);
}

UserData* UserRecord::login(const string&username,const string&password){
    ProtocolWriter writer(send_buf,Protocol::USER_LOGIN,base->token());
    protoData::UserForm form;
    form.set_username(username);
    form.set_password(password);
    writer.load(form);
    ProtocolReader reader(recv_buf);
    if(base->send(writer,reader)){
        protoData::User user;
        reader.get(user);
        base->copy_to_token(reader.token());
        return new UserData(user.id(),user.username(),user.password(),user.balance(),(int)user.type());
    }else{
        return nullptr;
    }
}


UserData* UserRecord::get(int id){
    ProtocolWriter writer(send_buf,Protocol::USER_INFO,base->token());
    protoData::User u;
    u.set_id(id);
    writer.load(u);
    ProtocolReader reader(recv_buf);
    if(base->send(writer,reader)){
        if(reader.length()>0){
            reader.get(u);
            return new UserData(u.id(),u.username(),u.password(),u.balance(),u.type());
        }
    }
    return nullptr;
}
string UserRecord::get_username(int id){
    ProtocolWriter writer(send_buf,Protocol::USER_INFO,base->token());
    protoData::User u;
    u.set_id(id);
    writer.load(u);
    ProtocolReader reader(recv_buf);
    if(base->send(writer,reader)){
       if(reader.length()>0){
        reader.get(u);
        return u.username();
       }
    }
    return string();
}

void UserRecord::set(int type,const string& username,const string& password){
    ProtocolWriter writer(send_buf,Protocol::USER_REGISTER,nullptr);
    protoData::UserForm u;
    u.set_username(username);
    u.set_password(password);
    u.set_type(type);
    writer.load(u);
    ProtocolReader reader(recv_buf);
    base->send(writer,reader);
}
bool UserRecord::update(const UserData&data){
    ProtocolWriter writer(send_buf,Protocol::USER_UPDATE,base->token());
    protoData::User u;
    u.set_username(data.username);
    u.set_password(data.password);
    u.set_type(data.type);
    u.set_balance(data.balance);
    writer.load(u);
    ProtocolReader reader(recv_buf);
    base->send(writer,reader);
    return true;
}