#include "database.h"

// UserRecord::UserRecord(){
//     if (file_exist(USER_FILE_NAME)){
//         load();
//     }
//     database.open(USER_FILE_NAME,std::ios::out);
// }
// const UserData* UserRecord::get(int id){
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




/********************
 * 
 * GoodsRecord
 * 
 * ****************/




// Goods GoodsRecord::get(int id){
//     static const char sql[] = "SELECT * FROM GOODS WHERE ID=%d";
//     static char buffer[48];
//     GoodsData goods;

//     sprintf(buffer,sql,id);
//     Database::exec(db,buffer,fetch_in_struct,&goods);
//     return Goods(goods.id,goods.name,goods.price,goods.seller);
// }
// Goods GoodsRecord::get(const std::string& name){
//     static const char sql[] = "SELECT * FROM GOODS WHERE NAME=%s";
//     static char buffer[256];
//     GoodsData goods;

//     sprintf(buffer,sql,name.c_str());
//     Database::exec(db,buffer,fetch_in_struct,&goods);
//     return Goods(goods.id,goods.name,goods.price,goods.seller);
// }
// void GoodsRecord::set(GoodsData data){
//     static const char sql[] = "INSERT INTO GOODS (NAME,PRICE,SELLER) VALUES ('%s', %d, %d ); ";
//     static char buffer[256];

//     sprintf(buffer,sql,data.name,data.price,data.seller);
//     Database::exec(db,buffer,nullptr,nullptr);
// }
// void GoodsRecord::remove(int id){
//     static const char sql[] = "DELETE FROM GOODS WHERE ID = %d; ";
//     static char buffer[48]; // 34+14

//     sprintf(buffer,sql,id);
//     Database::exec(db,buffer,nullptr,nullptr);
// }

// int GoodsRecord::fetch_in_struct(void*_data, int argc, char **argv, char **azColName){
//     GoodsData* data = (GoodsData*)_data;
//     data->id = atoi(argv[0]);
//     strcpy(data->name,argv[1]);
//     data->price = atof(argv[2]);
//     data->seller = atoi(argv[3]);
//     return 0;
// }