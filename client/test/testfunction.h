#pragma once
inline 
bool mysend(int sockfd,ProtocolWriter&w,ProtocolReader&r){
    int n;
    if ((n = send(sockfd,w.buf(),w.size(),0))<=0){
        log_error("ERROR writing to socket,n=%d",n);
        return false;
    }
    // log_info("recv_buf=%ld,r.buf()=%ld",recv_buf,r.buf());
    if ((n = recv(sockfd,r.buf(),8192,0))<=0){
        log_error("ERROR writing to socket,n=%d",n);
        return false;
    }else{
        log_debug("receive %d bytes",n);
    }
    return true;
}
inline
int init(){
    int portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int size;
    
   
    portno = 8888;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        log_error("ERROR opening socket");
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
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        log_error("ERROR connecting");
    return sockfd;
}
inline
void setup_user(int sockfd,char*send_buf,char*recv_buf,char*token){
	ProtocolReader reader(recv_buf);
         //注册
        {
            ProtocolWriter w1(send_buf,Protocol::USER_REGISTER,nullptr);
            protoData::UserForm form ;
            form.set_username("ruiqurm");
            form.set_password("123456");
            form.set_type(0);
            w1.load(form);
            if(mysend(sockfd,w1,reader)){
                reader.log_header();
            }
        }
        //登录
        {
            ProtocolWriter w(send_buf,Protocol::USER_LOGIN,nullptr);
            protoData::UserForm form ;
            form.set_username("ruiqurm");
            form.set_password("123456");
            w.load(form);
            if(mysend(sockfd,w,reader)){
                reader.log_header();
                memcpy(token,reader.token(),16);
                token[16] = 0;
                log_info("token=%.16s",token);
            }
        }
}