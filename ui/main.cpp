#include "mainwindow.h"
#include<include/concreteUser.h>
#include <QApplication>
#include "include/database.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    User::register_(USER_TYPE::seller,"ruiqurm","123456");

//    MainWindow w(std::shared_ptr<User>(User::login("ruiqurm","123456")));
    if (!Database::is_connected()){
        Database::retry_until_connected();
    }
    MainWindow w;
    w.show();
    return a.exec();
}
