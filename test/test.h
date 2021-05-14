#include<iostream>
#include<exception>
#include<fstream>
// #include<random>
using std::ifstream;
using std::cerr;
#define ASSERT(bool_expression,hint) if(!(bool_expression)){throw hint;}



bool exist(const char* file) {
    std::ifstream f(file);
    return f.good();
}
void remove_database(){
    if (exist("user-record.txt")){
        remove("user-record.txt");
    }
}