#pragma once
#include<string>
#include<vector>
#include<memory>
#include"database.h"
using std::string;
class GoodsData{
    public:
        char name[64];
        // string description;
        double price;
        int seller;// primary key of seller
        int id;
        // int remain;
};
class Goods{
    public:
        Goods();
        Goods(int id,const char name[],double price,int seller):
            id(id),name(name),price(price),seller(seller){
                #if debug==1
                std::cout<<"create goods name="<<name<<std::endl;
                #endif
            }
        string get_name(){return name;}
        // string get_descr(){return description;}
        double get_price(){return price;}
        // int get_remain(){return remain;}
        virtual int get_goods_type()const=0;
        virtual double get_price()const=0;
        // void set_remain()const;

        int get_id(){return id;}
        // void save();

    private:
        string name;
        // string description;
        // int remain;
        double price;
        int seller;
        // User* user;
        int id;
};
template <>
const char MetaRecord<Goods,GoodsData>::TABLE_NAME[] ="GOODS";

class GoodsRecord: public MetaRecord<Goods,GoodsData>{
    public:
        GoodsRecord(){
            static constexpr char sql[]= "CREATE TABLE GOODS("  \
                                        "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                                        "NAME           CHAR(64)    NOT NULL," \
                                        "PRICE          FLOAT       NOT NULL," \
                                        "SELLER         INT         NOT NULL);"; 
                                        // "DESCRIPTION    TEXT        NOT NULL"  \
                                        
                                        //"REMAIN         INT         NOT NULL);";
            Database::exec(db,sql,nullptr,nullptr);
        }
        std::shared_ptr<Goods> get(int id){
            static const char sql[] = "SELECT * FROM %s WHERE ID=%d";
            static char buffer[48]; 
            GoodsData goods;

            sprintf(buffer,sql,TABLE_NAME,id);
            Database::exec(db,buffer,fetch_in_struct,&goods);
            return std::make_shared<Goods>(goods.id,goods.name,goods.price,goods.seller);
        }
        std::vector<std::shared_ptr<Goods>> get_user_goods(int seller_id){
            static char sql[] =  "SELECT * FROM %s WHERE SELLER=%d";
            static char buffer[48];
            std::vector<std::shared_ptr<Goods>>vec;
            sprintf(buffer,sql,TABLE_NAME,seller_id);
            Database::exec(db,buffer,fetch_in_vector,&vec);
        }
        
    protected:
        void _data_to_string(char buffer[],const GoodsData& data){
            static char sql[] =  "INSERT INTO %s (NAME,PRICE,SELLER) VALUES ('%s', %d, %d ); ";
            sprintf(buffer,sql,TABLE_NAME,data.name,data.price,data.seller);
        }
        static int fetch_in_struct(void*_data, int argc, char **argv, char **azColName){
            GoodsData* data = (GoodsData*)_data;
            data->id = atoi(argv[0]);
            strcpy(data->name,argv[1]);
            data->price = atof(argv[2]);
            data->seller = atoi(argv[3]);
            return 0;
        }
        static int fetch_in_vector(void*_data, int argc, char **argv, char **azColName){
            typedef std::vector<std::shared_ptr<Goods>> data_type;
            data_type*pvec = (data_type*)_data;
            pvec->push_back(std::make_shared<Goods>(atoi(argv[0]),argv[1],atof(argv[2]),atoi(argv[3])));
            return 0;
        }

};


class Transaction{
    private:
        int from;
        int to;
        double volume;
        time_t timestamp;
};