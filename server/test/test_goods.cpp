#include"protocol.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<cstring>
extern "C" {
#include"log.h"
}
int sockfd;

inline 
bool mysend(ProtocolWriter&w,ProtocolReader&r){
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
int main(){
    int portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int size;
    char send_buf[8192];
    char recv_buf[8192];
    char token[18];
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
    memcpy(server->h_addr, 
         &serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        log_error("ERROR connecting");
    ProtocolReader reader(recv_buf);
    {
         //注册
        {
            ProtocolWriter w1(send_buf,Protocol::USER_REGISTER,nullptr);
            protoData::UserForm form ;
            form.set_username("ruiqurm");
            form.set_password("123456");
            form.set_type(0);
            w1.load(form);
            if(mysend(w1,reader)){
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
            if(mysend(w,reader)){
                reader.log_header();
                memcpy(token,reader.token(),16);
                token[16] = 0;
                log_info("token=%.16s",token);
            }
        }
        int id = 0;
        {
            ProtocolWriter w(send_buf,Protocol::GOOOS_CREATE,token);
            protoData::Goods goods;
            goods.set_name("大西瓜");
            goods.set_price(20);
            goods.set_type(0);
            goods.set_seller(1);
            goods.set_remain(10);
            w.load(goods);
            if(mysend(w,reader)){
                reader.log_header();
                reader.get(goods);
                reader.log_data(goods);
                id = goods.id();
            }
        }
         {
            ProtocolWriter w(send_buf,Protocol::GOOOS_GET_BY_ID,token);
            protoData::Goods goods;
            goods.set_id(id);
            w.load(goods);
            if(mysend(w,reader)){
                reader.log_header();
                reader.get(goods);
                reader.log_data(goods);
                if(goods.name()!="大西瓜"){
                    log_error("获取名称错误");
                }
            }   
        }
        {
            ProtocolWriter w(send_buf,Protocol::GOOOS_CREATE,token);
            protoData::Goods goods;
            goods.set_name("可乐");
            goods.set_price(20);
            goods.set_type(0);
            goods.set_seller(1);
            goods.set_remain(10);
            w.load(goods);
            if(mysend(w,reader)){
                reader.log_header();
                reader.get(goods);
                reader.log_data(goods);
                if(goods.name()!="可乐"){
                    log_error("获取名称错误");
                }
            }   
            goods.set_name("雪碧");
            goods.set_price(20);
            goods.set_type(0);
            goods.set_seller(1);
            goods.set_remain(10);
            w.clear_size();
            w.load(goods);
            if(mysend(w,reader)){
                reader.log_header();
                reader.get(goods);
                reader.log_data(goods);
                if(goods.name()!="雪碧"){
                    log_error("获取名称错误");
                }
            }   
        }
        {
            ProtocolWriter w(send_buf,Protocol::GOODS_GET_ALL,token);
            protoData::GoodsArray array;
            if(mysend(w,reader)){
                reader.log_header();
                reader.get(array);
                reader.log_data(array);
            }   
        }
        {
            ProtocolWriter w(send_buf,Protocol::GOODS_GET_BY_ID_MULITIPLE,token);
            protoData::GoodsArray array;
            array.add_goods()->set_id(1);
            array.add_goods()->set_id(2);
            array.add_goods()->set_id(3);
            w.load(array);
            if(mysend(w,reader)){
                reader.log_header();
                reader.get(array);
                reader.log_data(array);
            }   
        }
        {
            ProtocolWriter w(send_buf,Protocol::GOODS_GET_BY_NAME,token);
            protoData::Goods goods;
            
            goods.set_name("瓜");
            w.load(goods);
            if(mysend(w,reader)){
                reader.log_header();
                protoData::GoodsArray array;
                reader.get(array);
                reader.log_data(array);
            }   
        }
        {
            ProtocolWriter w(send_buf,Protocol::GOODS_UPDATE,token);
            protoData::Goods goods;
            goods.set_id(1);
            goods.set_name("中西瓜");
            goods.set_price(40);
            goods.set_type(0);
            goods.set_seller(1);
            goods.set_remain(40);
            w.load(goods);
            if(mysend(w,reader)){
                reader.log_header();
                reader.get(goods);
                reader.log_data(goods);
            }   
        }
    }   
}