#include<iostream>
#include<exception>
#include<fstream>
// #include<random>
using std::ifstream;
using std::cerr;
#define ASSERT(bool_expression,hint) if(!(bool_expression)){throw hint;}

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
    std::ifstream f(file);
    return f.good();
}
void remove_database(){
    if (exist("user-record.txt")){
        remove("user-record.txt");
    }
    if (exist("data.db")){
        remove("data.db");
    }
}