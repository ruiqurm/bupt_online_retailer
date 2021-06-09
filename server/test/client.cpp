#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "protoData.pb.h"
#include "protocol.h"
extern "C" {
#include"log.h"
}

void write_request(char*buffer,int type,const char* token,int size){
    ((Protocol*)buffer)->transaction_id = getpid();
    ((Protocol*)buffer)->type = type;
    ((Protocol*)buffer)->is_response = 0;
    if(token!=nullptr)memcpy(((Protocol*)buffer)->token,token,16);
    ((Protocol*)buffer)->length = size;
}
inline
void read_header(char*buffer){
    if(((Protocol*)buffer)->is_response){
        log_info("header 类型:回复\ntransaction id= %d\ntype=%s\nstatus=%s\nlength=%d\ntoken=%.16s",
            ((Protocol*)buffer)->transaction_id,protocol_type_to_str(((Protocol*)buffer)->type),protocol_status_to_str(((Protocol*)buffer)->status),
            ((Protocol*)buffer)->length,((Protocol*)buffer)->token);
    }else{
        log_info("header 类型:请求\ntransaction id= %d\ntype=%s\nlength=%d\ntoken=%.16s",
            ((Protocol*)buffer)->transaction_id,protocol_type_to_str(((Protocol*)buffer)->type),((Protocol*)buffer)->length,((Protocol*)buffer)->token);
    }
    
}
void read_userdata(char*buffer){
    if(((Protocol*)buffer)->length==0){
        log_info("数据:\n空");
        return;
    }
    protoData::User user;
    user.ParseFromArray(buffer+sizeof(Protocol),((Protocol*)buffer)->length);
    log_info("数据\nusername=%s\npassword=%s\nbalance=%.2f",user.username().c_str(),user.password().c_str(),user.balance());
}
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int size;
    char buffer[8192];
    char recv_buf[8192];
    // if (argc < 2) {
    //    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    //    exit(0);
    // }
    portno = atoi("8888");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        log_error("ERROR opening socket");
    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    protoData::UserForm form;
    form.set_username("ruiqurm");
    form.set_password("123456");
    form.set_type(0);
    string token;
    {
        if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
            log_error("ERROR connecting");
        write_request(buffer,Protocol::USER_REGISTER,nullptr,form.ByteSizeLong());
        form.SerializeToArray(buffer+sizeof(Protocol),form.ByteSizeLong());
        size = sizeof(Protocol) + form.ByteSizeLong();
        read_header(buffer);
        if ((n = send(sockfd,buffer,size,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
            return 0;
        }
        if ((n = recv(sockfd,recv_buf,8192,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
            return 0;
        }
        read_header(recv_buf);
        write_request(buffer,Protocol::USER_LOGIN,nullptr,form.ByteSizeLong());
        read_header(buffer);
        if ((n = send(sockfd,buffer,size,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
        }
        if ((n = recv(sockfd,recv_buf,8192,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
            return 0;
        }
        log_info("n = %d",n);
        read_header(recv_buf);
        read_userdata(recv_buf);
        log_info("再试一次登录");
        if ((n = send(sockfd,buffer,size,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
        }
        if ((n = recv(sockfd,recv_buf,8192,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
            return 0;
        }
        read_header(recv_buf);
        log_info("尝试校验密码:错误密码");
        write_request(buffer,Protocol::USER_AUTHENTICATE_PASSWORD,nullptr,form.ByteSizeLong());
        form.set_password("1234567");
        form.SerializeToArray(buffer+sizeof(Protocol),form.ByteSizeLong());
        if ((n = send(sockfd,buffer,size,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
        }
        if ((n = recv(sockfd,recv_buf,8192,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
            return 0;
        }
        read_header(recv_buf);
        if(((Protocol*)recv_buf)->status==Protocol::OK){
            log_error("校验密码失败");
        }
        log_info("尝试校验密码:正确密码");
        write_request(buffer,Protocol::USER_AUTHENTICATE_PASSWORD,nullptr,form.ByteSizeLong());
        form.set_password("123456");
        form.SerializeToArray(buffer+sizeof(Protocol),form.ByteSizeLong());
        if ((n = send(sockfd,buffer,size,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
        }
        if ((n = recv(sockfd,recv_buf,8192,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
            return 0;
        }
        read_header(recv_buf);
        if(((Protocol*)recv_buf)->status!=Protocol::OK){
            log_error("校验密码失败");
        }
        log_info("获取信息");
        write_request(buffer,Protocol::USER_INFO,nullptr,0);
        if ((n = send(sockfd,buffer,size,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
        }
        if ((n = recv(sockfd,recv_buf,8192,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
            return 0;
        }
        read_userdata(recv_buf);
        log_info("注销");
        write_request(buffer,Protocol::USER_LOGOUT,nullptr,0);
        if ((n = send(sockfd,buffer,size,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
        }
        if ((n = recv(sockfd,recv_buf,8192,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
            return 0;
        }
        read_header(recv_buf);
        log_info("更新信息");
        write_request(buffer,Protocol::USER_LOGIN,nullptr,form.ByteSizeLong());
        if ((n = send(sockfd,buffer,size,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
        }
        if ((n = recv(sockfd,recv_buf,8192,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
            return 0;
        }
        read_header(recv_buf);
        {
            protoData::User userdata;
            userdata.set_username("ruiqurm");
            userdata.set_password("123456");
            userdata.set_type(0);
            userdata.set_balance(20.2);
            userdata.SerializeToArray(buffer+sizeof(Protocol),userdata.ByteSizeLong());
            write_request(buffer,Protocol::USER_UPDATE,nullptr,userdata.ByteSizeLong());
            size = userdata.ByteSizeLong() + sizeof(Protocol);
            if ((n = send(sockfd,buffer,size,0))<=0){
                log_error("ERROR writing to socket,n=%d",n);
            }
            if ((n = recv(sockfd,recv_buf,8192,0))<=0){
                log_error("ERROR writing to socket,n=%d",n);
                return 0;
            }
            read_header(recv_buf);
        }
        {
            protoData::User userdata;
            userdata.set_username("ruiqurm");
            userdata.set_password("12345678");
            userdata.set_type(0);
            userdata.set_balance(20.2);
            userdata.SerializeToArray(buffer+sizeof(Protocol),userdata.ByteSizeLong());
            write_request(buffer,Protocol::USER_UPDATE,nullptr,userdata.ByteSizeLong());
            size = userdata.ByteSizeLong() + sizeof(Protocol);
            if ((n = send(sockfd,buffer,size,0))<=0){
                log_error("ERROR writing to socket,n=%d",n);
            }
            if ((n = recv(sockfd,recv_buf,8192,0))<=0){
                log_error("ERROR writing to socket,n=%d",n);
                return 0;
            }
            read_header(recv_buf);
        }
        form.set_password("12345678");
        write_request(buffer,Protocol::USER_LOGIN,nullptr,form.ByteSizeLong());
        form.SerializeToArray(buffer+sizeof(Protocol),form.ByteSizeLong());
        size = form.ByteSizeLong()+sizeof(Protocol);
        if ((n = send(sockfd,buffer,size,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
        }
        if ((n = recv(sockfd,recv_buf,8192,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
            return 0;
        }
        {
            protoData::User userdata;
            userdata.set_username("ruiqurm");
            userdata.set_password("123456");
            userdata.set_type(0);
            userdata.set_balance(40.2);
            userdata.SerializeToArray(buffer+sizeof(Protocol),userdata.ByteSizeLong());
            write_request(buffer,Protocol::USER_UPDATE,nullptr,userdata.ByteSizeLong());
            size = userdata.ByteSizeLong() + sizeof(Protocol);
            if ((n = send(sockfd,buffer,size,0))<=0){
                log_error("ERROR writing to socket,n=%d",n);
            }
            if ((n = recv(sockfd,recv_buf,8192,0))<=0){
                log_error("ERROR writing to socket,n=%d",n);
                return 0;
            }
            read_header(recv_buf);
        }
        form.set_password("123456");
        write_request(buffer,Protocol::USER_LOGIN,nullptr,form.ByteSizeLong());
        form.SerializeToArray(buffer+sizeof(Protocol),form.ByteSizeLong());
        size = form.ByteSizeLong()+sizeof(Protocol);
        if ((n = send(sockfd,buffer,size,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
        }
        if ((n = recv(sockfd,recv_buf,8192,0))<=0){
            log_error("ERROR writing to socket,n=%d",n);
            return 0;
        }
        read_header(recv_buf);
        close(sockfd);
    }
    
    int sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sockfd2, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        log_error("ERROR connecting");
    write_request(buffer,Protocol::USER_INFO,((Protocol*)recv_buf)->token,0);
    read_header(buffer);
    size = sizeof(Protocol);
    if ((n = send(sockfd,buffer,size,0))<=0){
        log_error("ERROR writing to socket,n=%d",n);
    }
    if ((n = recv(sockfd,recv_buf,8192,0))<=0){
        log_error("ERROR writing to socket,n=%d",n);
        return 0;
    }
    read_header(recv_buf);
    read_userdata(recv_buf);
    return 0;
}