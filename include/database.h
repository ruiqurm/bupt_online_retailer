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
        virtual int set(T)=0;
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
class GoodsData{
    public:
        char name[64];
        double price;
        int seller;//primary key of seller

};
class UserRecord:public RecordInterfase<UserData>{
    public:
        UserRecord(const UserRecord&) = delete;
        UserRecord &operator=(const UserRecord&) = delete;
        //阻止拷贝构造

        static UserRecord& get_record(){
            static UserRecord record;
            return record;
        }
        const UserData* get(int pk)override;
        const UserData* get(const std::string& username);
        int set(UserData data)override;
        void remove(const std::string& username);
        void remove(int pk)override;
        UserData* update(int pk)override;
        UserData* update(const std::string& username);
        int size(){return (name_to_data.size());}
        bool exist(const std::string& username){return (name_to_data.find(username)!=name_to_data.end());}
        int load();
        void save();
        ~UserRecord(){}
    protected:
        bool file_exist(const char* file) {
            std::ifstream f(file);
            return f.good();
        }
        UserRecord();
    private:
        std::map<std::string,UserData> name_to_data;
        std::map<int,std::string> pk_to_name;
        std::ofstream database;
        int max_pk;
        static constexpr const char* USER_FILE_NAME="user-record.txt";
        static constexpr int USERDATA_SIZE = sizeof(UserData);
};


class GoodsRecord:public RecordInterfase<GoodsData>{
    
};