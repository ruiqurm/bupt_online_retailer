#include"protocol.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<cstring>
extern "C" {
#include"log.h"
}
#include"testfunction.h"



int main(){
    char send_buf[8192];
    char recv_buf[8192];
    char token[18];
    int sockfd = init();
    setup_user(sockfd,send_buf,recv_buf,token);
    ProtocolReader reader(recv_buf);
//     int id = 0;
	{
		ProtocolWriter w(send_buf,Protocol::CART_SET,token);
		protoData::CartItem item;
		item.set_goods(1);
		item.set_user(1);
		item.set_count(5);
		w.load(item);
		if(mysend(sockfd,w,reader)){
		reader.log_header();
		reader.get(item);
		reader.log_data(item);
		// id = goods.id();
		}
		item.set_goods(2);
		item.set_user(1);
		item.set_count(10);
		w.load(item);
		if(mysend(sockfd,w,reader)){
		reader.log_header();
		reader.get(item);
		reader.log_data(item);
		// id = goods.id();
		}
	}
	{
		ProtocolWriter w(send_buf,Protocol::CART_GET,token);
		protoData::CartItem item;
		item.set_user(1);
		w.load(item);
		if(mysend(sockfd,w,reader)){
		protoData::Cart cart;
		reader.log_header();
		reader.get(cart);
		reader.log_data(cart);
		// id = goods.id();
		}
	}
	{
		ProtocolWriter w(send_buf,Protocol::CART_REMOVE,token);
		protoData::CartItem item;
		item.set_user(1);
		item.set_goods(2);
		w.load(item);
		mysend(sockfd,w,reader);
		item.set_user(1);
		w.type() = Protocol::CART_GET;
		w.load(item);
		if(mysend(sockfd,w,reader)){
			protoData::Cart cart;
			reader.log_header();
			reader.get(cart);
			reader.log_data(cart);
		// id = goods.id();
		}
	}
}