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
            static int sockfd = init();
            static Database db(sockfd);
            return &db;
        }
        static int init(){
            int portno, n;
            struct sockaddr_in serv_addr;
            struct hostent *server;
            int size; 
            int retry[] {1,2,4,8,16,32,32,32,32,32};
            portno = 8888;
            int sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) 
                printf("ERROR opening socket");
            server = gethostbyname("127.0.0.1");
            if (server == NULL) {
                fprintf(stderr,"ERROR, no such host\n");
                exit(0);
            }
            memset(&serv_addr,0,sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            memcpy(server->h_addr, 
                &serv_addr.sin_addr.s_addr,
                server->h_length);
            serv_addr.sin_port = htons(portno);
            bool ok = false;
            for(int i =0;i<10;i++){
                if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
                    printf("ERROR connecting");
                    printf("%ds后尝试重连\n",retry[i]);
                    sleep(retry[i]*1000);
                }else{
                    ok = true;
                    break;
                }
            }
            if(!ok){
                printf("连接网络失败...退出\n");
                exit(0);
            }
            return sockfd;  
        }
        int send(ProtocolWriter& w,ProtocolReader&r);
        int sock()const{return sockfd;}
        char* token(){return _token;}
        const char* token()const{return _token;}
        void copy_to_token(const char* s){
            memcpy(_token,s,16);
            _token[16]=0;
        }
        Database(int a):sockfd(a){}
    private:
        int sockfd;
        char _token[18];

};