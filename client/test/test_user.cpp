// #include"testfunction.h"
#include"user.h"

int main(){
    // User::register_(0,"aaaaa","123456");
    
    try{
        auto p = User::login("aaaaa","123456");
    if(p!=nullptr){
        std::cout<<p->username();
    }}
    catch(const char* s){
        std::cout<<s;
    }
}