#pragma once
#include<iostream>
#include<exception>
#include<fstream>
#include<map>
#include<cstring>
#include<filesystem>
using namespace std;
#define ASSERT(bool_expression,hint) if(!(bool_expression)){throw hint;}


// hacking

// class TestException:public std::exception{
//      explicit TestException(const char* message)
//         : msg_(message) {}

//     explicit TestException(const std::string& message)
//         : msg_(message) {
//             string
//         }

//     virtual ~TestException() noexcept {}

//     virtual const char* what() const noexcept {
//        return msg_.c_str();
//     }

// protected:
//     /** Error message.
//      */
//     std::string msg_;
// };
bool exist(const char* file) {
    return std::filesystem::exists(file);
}
class database_clean{
    public:
        static void remove_database(){
            if(!if_clean)return;
            if (exist("user-record.txt")){
                remove("user-record.txt");
            }
            if (exist("data.db")){
                remove("data.db");
            }
        }
        static void parse_argument(int argc,char**argv){
            if(argc<1){
                return;
            }
            for(int i=1;i<argc;i++){
                if(!strcmp(argv[i],"-pd")){
                    if_clean=false;
                }
            }
        }
        static bool if_clean;
};
bool database_clean::if_clean=true;
