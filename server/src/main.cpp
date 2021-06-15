#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// #include"sqlite3.h"
#include <thread>
#include "server.h"
extern "C" {
#include"log.h"
}
#include <mutex>
using std::thread;
std::mutex g_mutex;

int init(){
    
    int sockfd =  socket(AF_INET, SOCK_STREAM, 0);
    // try{
    if (sockfd < 0){
        log_fatal_exit_shortcut("opening socket ");
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = INADDR_ANY;  
    serv_addr.sin_port = htons(PROTOCOL_PORT);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0){
              log_fatal_shortcut("bind_error");
              }
    log_info("listen on 127.0.0.1:8888");
    listen(sockfd,5);
    return sockfd;
}
// bool exec(sqlite3 *db, const char *sql, 
//     int (*callback)(void *, int, char **, char **), 
//     void *arg){
//     char *errmsg=nullptr;
//     int rc = sqlite3_exec(db,sql,callback,arg,&errmsg);
//     if( rc != SQLITE_OK ){
//         fprintf(stderr, "SQL error: %s\n", errmsg);
//         sqlite3_free(errmsg);
//         return false;
//     }
//     return true;
// }
void handle_with_request(int sockfd);

int main(int argc, char *argv[]){
    
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char read_buffer[8192];
     char write_buffer[8192];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
    sockfd = init();
    // sqlite3*db;
    int size;
    Executor* executor=nullptr;
    while(1){
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, 
                    (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
        log_error("ERROR on accept");
        log_info("server: got connection from %s port %d  in sockfd=%d",
            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port),newsockfd);
        thread t(handle_with_request,newsockfd);
        t.detach();
    }
     close(sockfd);
     return 0; 
}

void handle_with_request(int sockfd){
    int size,n;
    char read_buffer[8192];
    char write_buffer[8192];
    Executor* executor;
    protoData::User*user=nullptr;
    bool has_authenticated = false;
    while(true){
        size = 8192;
        
        n = recv(sockfd,read_buffer,size,0);
        if (n <= 0) {
            if(n==0){
                log_info("sockfd=%d close connection",sockfd);
            }else{
                log_error_shortcut("ERROR reading from socket");
            }
            break;
        }
        g_mutex.lock();
        if ((executor = ExecutorFactory::get_executor(read_buffer,write_buffer,user))!=nullptr){
            executor->exec();
            user = executor->get_user();
        }
        g_mutex.unlock();
        n = send(sockfd,write_buffer,executor->size(),0);
        if (n <= 0) {delete executor;executor = nullptr;;log_error("ERROR writing from socket");break;}
        delete executor;
        executor = nullptr;
    }
    g_mutex.unlock();
    close(sockfd);
}