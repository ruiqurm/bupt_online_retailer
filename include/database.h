#pragma once
#include<fstream>
#include<map>
#include<cstring>
#include<cctype>
#include<string>



template<typename T>
class RecordInterfase{
    public:
        virtual const T* get(int pk)=0;
        virtual int set(const T&)=0;
        virtual void remove(int pk)=0;
        virtual T* update(int pk)=0;
};

class UserData{
    public:
        char username[32];
        char password[16];
        int pk;
        int balance;
        int type;
        UserData(){}
        UserData(int pk,const char*username,
                const char*password,int balance,
                int type):pk(pk),balance(balance),type(type){
                    strcpy(this->username,username);
                    strcpy(this->password,password);
        }
};

class UserRecord:public RecordInterfase<UserData>{
    public:
        static UserRecord& get_record(){
            static UserRecord record;
            return record;
        }
        const UserData* get(int pk)override{
            return update(pk);
        }
        const UserData* get(const std::string& username){
            return update(username);
        }
        int set(const UserData& data)override{
            if (exist(data.username)){
                //不抛出异常
                return -1;
            }
            char*c= const_cast<char*>(data.username);
            for(int i=0;*c!=0&&i<32;i++,c++){
                if(i>=30)throw "username too long";
                if(isspace(*c)) throw "disallow space character in username";
            }
            c= const_cast<char*>(data.password);
            for(int i=0;*c!=0&&i<16;i++,c++){
                if(i>16)throw "password too long";
                if(isspace(*c)) throw "disallow space character in password";
            }
            max_pk++;
            name_to_data.emplace(data.username,data);
            pk_to_name.emplace(max_pk,data.username);
            return max_pk;
        }
        void remove(const std::string& username){
            auto it = name_to_data.find(username);
            if(it==name_to_data.end())return;
            pk_to_name.erase(it->second.pk);
            name_to_data.erase(it);
        }
        void remove(int pk)override{
            auto it = pk_to_name.find(pk);
            if(it==pk_to_name.end())return;
            name_to_data.erase(it->second);
            pk_to_name.erase(it);
        }
        UserData* update(int pk)override{
            auto it = pk_to_name.find(pk);
            if(it==pk_to_name.end()){
                return nullptr;
            }else{
                return &name_to_data.find(it->second)->second;
            }
        }
        UserData* update(const std::string& username){
            auto it = name_to_data.find(username);
            if(it==name_to_data.end()){
                return nullptr;
            }else{
                return &it->second;
            }
        }
        int size(){
            return (name_to_data.size());
        }
        bool exist(const std::string& username){
            return (name_to_data.find(username)!=name_to_data.end());
        }
        int load(){
            int count=0;
            std::ifstream f(USER_FILE_NAME);
            UserData tmpdata;
            max_pk=0;
            while(f>>tmpdata.pk>>tmpdata.username>>tmpdata.password>>tmpdata.balance>>tmpdata.type){
                name_to_data.emplace(tmpdata.username,tmpdata);
                pk_to_name.emplace(tmpdata.pk,tmpdata.username);
                max_pk = std::max(max_pk,tmpdata.pk);
                count++;
            }
            return count;
        }
        void save(){
            for(auto it=name_to_data.begin();it!=name_to_data.end();it++){
                auto& data = it->second;
                database<<data.pk<<" "<<data.username<<" "<<data.password<<" "<<data.balance<<" "<<data.type<<std::endl;
            }
            database.flush();
        }
        ~UserRecord(){
            // database.close();
        }
    protected:
        bool file_exist(const char* file) {
            std::ifstream f(file);
            return f.good();
        }
        UserRecord(){
            if (file_exist(USER_FILE_NAME)){
                load();
            }
            database.open(USER_FILE_NAME,std::ios::out);
        }
    private:
        std::map<std::string,UserData> name_to_data;
        std::map<int,std::string> pk_to_name;
        std::ofstream database;
        int max_pk;
        static constexpr const char* USER_FILE_NAME="user-record.txt";
        static constexpr int USERDATA_SIZE = sizeof(UserData);
};

