#include "database.h"
std::mutex g_lock;
bool Database::_is_connected = false;
sockaddr_in Database::server_addr;
int Database::init(){
	int portno, n;
	struct hostent *server;
	int size; 

	portno = PROTOCOL_PORT;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	printf("ERROR opening socket");
	server = gethostbyname("127.0.0.1");
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	memcpy(server->h_addr, 
	&server_addr.sin_addr.s_addr,
	server->h_length);
	server_addr.sin_port = htons(portno);
	Database::_is_connected = connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr))==0;
	return sockfd;
}
int Database::sockfd=Database::init();

int Database::send(ProtocolWriter& w,ProtocolReader&r){
	g_lock.lock();
	int n;
	if ((n = ::send(sockfd,w.buf(),w.size(),0))<=0){
	printf("ERROR writing to socket,n=%d",n);
	g_lock.unlock();
	return false;
	}
	// log_info("recv_buf=%ld,r.buf()=%ld",recv_buf,r.buf());
//     printf("buf_size=%d\n",r.buf_size());
    n=0;
	while( (n = recv(sockfd,r.buf(),r.buf_size(),0))==0){
        printf("接收失败\n");
    }
	if(n<0){
        printf("ERROR writing to socket,n=%d",n);
        g_lock.unlock();
        return false;
	}
	if (r.status() != Protocol::OK && r.status() != Protocol::OK_LOGOUT){
	printf("r.status =%d, %s\n",r.status(),protocol_status_to_str(r.status()));
	g_lock.unlock();
	return false;
	}
	// std::cout<<r.id()<<" "<<r.type()<<" "<<std::endl;
	// std::cout<<"length="<<r.length()<<" "<<n-28<<std::endl;
	if(r.length()>0){
	g_lock.unlock();
	return 2;
	}
	g_lock.unlock();
	return 1;
}
void Database::retry_until_connected(){
    if(_is_connected){
        return;
    }
    int retry[] {1,2,4,8,16,32,32,32,32,32,32,32,32,32,32,32,32};
    for(int i =0;i<10;i++){
        if ( connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) !=0){
            std::cout<<"连接失败"<<retry[i]<<"s后尝试重连"<<std::endl;
            sleep(retry[i]);
        }else{
            _is_connected = true;
            break;
        }
    }
    if(!_is_connected){
        printf("连接网络失败...退出\n");
        exit(0);
    }
}
bool Database::retry(){
    return (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) ==0);
}