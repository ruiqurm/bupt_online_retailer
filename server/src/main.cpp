#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// #include"sqlite3.h"
#include "server.h"
extern "C" {
#include"log.h"
}

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
    serv_addr.sin_port = htons(8888);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) {}
              log_fatal_exit_shortcut("bind_error");
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

        log_info("server: got connection from %s port %d\n",
            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
        // n = write(newsockfd,buffer,size);
        n = read(newsockfd,read_buffer,size);
        if (n < 0) {log_error("ERROR reading from socket");continue;}
        if ((executor = ExecutorFactory(read_buffer,write_buffer))!=nullptr){
        executor->exec();
        }
        n = write(newsockfd,write_buffer,executor->size());
        if (n < 0) {log_error("ERROR writing from socket");continue;}

        close(newsockfd);
    }
     close(sockfd);
     return 0; 
}

