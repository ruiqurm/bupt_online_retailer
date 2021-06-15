#pragma once
#include<fstream>
#include<map>
#include<cstring>
#include<cctype>
#include<string>
#include<cstdlib>
#include<memory>
#include<vector>
#include"protocol.h"
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include<mutex>

class Database{
    public:
        static Database* get_database(){
            static Database db;
            return &db;
        }
        static bool retry();

        // 使用二进制指数回退
        static void retry_until_connected();
        static int init();
        int send(ProtocolWriter& w,ProtocolReader&r);
        int sock()const{return sockfd;}
        char* token(){return _token;}
        const char* token()const{return _token;}
        void copy_to_token(const char* s){
            memcpy(_token,s,16);
            _token[16]=0;
        }
        static bool is_connected(){return _is_connected;}
        // Database(int a):sockfd(a){}
    private:
        static bool _is_connected;
        static int sockfd;
        static struct sockaddr_in server_addr;
        char _token[18];


};