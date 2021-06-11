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
    {
	int id =0;
	{
            ProtocolWriter w(send_buf,Protocol::TRANSACTION_SET,token);
            protoData::Transaction trans;
            trans.set_from(1);
	    trans.set_from_name("ruiqurm");
            trans.set_to(2);
	    trans.set_to_name("testuser");
	    trans.set_finished(false);
            trans.set_volume(20.0);
            trans.set_detail("aaa");
            trans.set_timestamp(1000);
            w.load(trans);
            if(mysend(sockfd,w,reader)){
                reader.log_header();
                reader.get(trans);
                reader.log_data(trans);
		id = trans.id();
            }
	    trans.set_id(id);
	    w.type() = Protocol::TRANSACTION_SET_FINISHED;
	    w.load(trans);
	    if(mysend(sockfd,w,reader)){
                reader.log_header();
            }
	    trans.set_from(1);
	    trans.set_from_name("ruiqurm");
            trans.set_to(3);
	    trans.set_to_name("testuser2");
	    trans.set_finished(false);
            trans.set_volume(40.0);
            trans.set_detail("bbbb");
            trans.set_timestamp(2000);
	    w.type() = Protocol::TRANSACTION_SET;
	    w.load(trans);
	    if(mysend(sockfd,w,reader)){
                reader.log_header();
                reader.get(trans);
                reader.log_data(trans);
		id = trans.id();
            }
	    w.type() = Protocol::TRANSACTION_GET_BY_USER;
	    trans.set_from(1);
	    trans.set_finished(false);
	    w.load(trans);
	    if(mysend(sockfd,w,reader)){
                reader.log_header();
		protoData::TransactionArray array;
                reader.get(array);
		log_info("未完成的:");
                reader.log_data(array);
            }    
	    trans.set_finished(true);
	    w.load(trans);
	    if(mysend(sockfd,w,reader)){
                reader.log_header();
		protoData::TransactionArray array;
                reader.get(array);
		log_info("已完成的:");
                reader.log_data(array);
            }
	    w.type() = Protocol::TRANSACTION_CANCEL;
	    w.load(trans);
	    mysend(sockfd,w,reader);
	    trans.set_from(1);
	    trans.set_finished(false);
	    w.load(trans);
	    if(mysend(sockfd,w,reader)){
                reader.log_header();
		protoData::TransactionArray array;
                reader.get(array);
		log_info("未完成的:");
                reader.log_data(array);
            }    
        }  
    }
//     int id = 0;
}