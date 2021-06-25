/**
 * @file database.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief socket RPC接口
 */
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

/**
 * @brief 与服务器交互接口
 * 
 */
class Database{
    public:
        static Database* get_database(){
            static Database db;
            return &db;
        }
        

        static bool retry();

        /**
         * @brief 重复连接，直到连接成功
         * @details 使用指数回退
         */
        static void retry_until_connected();

        /**
         * @brief 初始化，配置socket，尝试连接
         * @details 如果连接失败，不会继续尝试，后面可以再尝试连接
         * @return int socket 文件描述符
         */
        static int init();

        /**
         * @brief 发送数据
         */
        int send(ProtocolWriter& w,ProtocolReader&r);
        
        /// 获取文件描述符
        int sock()const{return sockfd;}

        /// 获取token
        char* token(){return _token;}
        /// 获取token
        const char* token()const{return _token;}
        /// 覆盖token
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