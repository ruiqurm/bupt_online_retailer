#pragma once
#include<iostream>
#include<exception>
#include<cstring>
// #include<filesystem>
#include<assert.h>
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

bool exist (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}
// class database_clean{
//     public:
//         static void remove_database(){
//             if(!if_clean)return;
//             cout<<"Try to clean...\n";
//             if (exist("user-record.txt")){
//                 cout<<"Clean User-record.txt\n";
//                 remove("user-record.txt");
//             }
//             if (exist("data.db")){
//                 cout<<"Clean data.db"<<endl;
//                 remove("data.db");
//             }
//         }
//         static void parse_argument(int argc,char**argv){
//             if(argc<1){
//                 return;
//             }
//             for(int i=1;i<argc;i++){
//                 if(!strcmp(argv[i],"-pd")){
//                     if_clean=false;
//                 }
//             }
//         }
//         static bool if_clean;
// };
// bool database_clean::if_clean=true;

