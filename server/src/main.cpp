/**
 * @file main.cpp
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 服务端主函数
 */
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

/**
 * @brief 建立连接，绑定0.0.0.0:端口上。
 * 
 * @return int 
 */
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
    log_info("listen on 0.0.0.0:12345");
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

/**
 * @brief 处理请求的函数
 * 
 * @param sockfd 分配的文件流
 */
void handle_with_request(int sockfd);

int main(int argc, char *argv[]){
    
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char read_buffer[8192];// 实际使用可以改大一点
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
        thread t(handle_with_request,newsockfd);// 创建线程
        t.detach();// 分离
    }
     close(sockfd);
     return 0; 
}

void handle_with_request(int sockfd){
    /**
     * @brief 处理的流程：
     * 1. 接收数据
     * 2. 加锁
     * 3. 调用对应类型的执行器，执行器除了执行外，还会校验token，并生成出user对象（除了登录和注册）
     * 4. 保存user对象的指针（不会被析构，因为全局只有一份user对象，见 @ref UserRecordWriter "UserRecordWriter"）,在下一轮可以使用
     * 5. 解锁，回到1
     */
    int size,n;
    char read_buffer[8192];
    char write_buffer[8192];
    Executor* executor;
    protoData::User*user=nullptr; // 执行用户数据的指针
    bool has_authenticated = false;// 是否登录
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
        g_mutex.lock();// 锁
        if ((executor = ExecutorFactory::get_executor(read_buffer,write_buffer,user))!=nullptr){
            executor->exec();
            user = executor->get_user(); // 获取登录后的用户，并将在
        }
        g_mutex.unlock();// 解锁
        n = send(sockfd,write_buffer,executor->size(),0);
        if (n <= 0) {delete executor;executor = nullptr;;log_error("ERROR writing from socket");break;}
        delete executor;
        executor = nullptr;
    }
    g_mutex.unlock();
    close(sockfd);
}