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
    int id;
	setup_user(sockfd,send_buf,recv_buf,token);
	ProtocolReader reader(recv_buf);
	ProtocolWriter w(send_buf,Protocol::DISCOUNT_CREATE,token);
	protoData::Discount discount;
	discount.set_operand(1);
	discount.set_type(0);
	discount.set_discount(0.9);
	discount.set_threshold(-1);
	w.load(discount);
	if(mysend(sockfd,w,reader)){
	reader.log_header();
	reader.get(discount);
	reader.log_data(discount);
	// id = goods.id();
	}
	discount.set_operand(2);
	discount.set_discount(0.8);
	w.load(discount);
	if(mysend(sockfd,w,reader)){
	reader.log_header();
	reader.get(discount);
	reader.log_data(discount);
	}
	discount.set_type(1);//假设1是category打折
	discount.set_operand(1);
	discount.set_discount(0.6);
	w.load(discount);
	if(mysend(sockfd,w,reader)){
		reader.log_header();
		reader.get(discount);
		reader.log_data(discount);
        id = discount.id();
	}
    discount.set_type(1);//假设1是category打折
	discount.set_operand(2);
	discount.set_discount(0.5);
	w.load(discount);
	if(mysend(sockfd,w,reader)){
		reader.log_header();
		reader.get(discount);
		reader.log_data(discount);
        
	}
	w.type() = Protocol::DISCOUNT_GET_GOODS_DISCOUNT;
    discount.set_type(0);
    discount.set_operand(2);
    w.load(discount);
	if(mysend(sockfd,w,reader)){
		reader.log_header();
		reader.get(discount);
        log_info("获取goods=2的折扣");
		reader.log_data(discount);
	}
    w.type() = Protocol::DISCOUNT_GET_CATEGORY_DISCOUNT;
    discount.set_type(1);
    discount.set_operand(1);
    w.load(discount);
	if(mysend(sockfd,w,reader)){
		reader.log_header();
		reader.get(discount);
        log_info("获取种类为=1的折扣");
		reader.log_data(discount);
	}
    w.type() = Protocol::DISOCUNT_UPDATE;
    discount.set_id(id);
    discount.set_type(1);
    discount.set_operand(1);
	discount.set_discount(0.4);
    w.load(discount);
	if(mysend(sockfd,w,reader)){
		reader.log_header();
		reader.get(discount);
        log_info("修改种类为=1的折扣");
		reader.log_data(discount);
	}
    w.type() = Protocol::DISCOUNT_GET_ALL_CATEGORY;
    discount.set_type(1);
    w.load(discount);
	if(mysend(sockfd,w,reader)){
        protoData::DiscountArray array;
		reader.log_header();
		reader.get(array);
        log_info("获取type=1的折扣");
		reader.log_data(array);
	}
    
}