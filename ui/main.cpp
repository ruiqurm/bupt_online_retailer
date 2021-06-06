#include "mainwindow.h"
#include<include/concreteUser.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    User::register_(USER_TYPE::seller,"ruiqurm","123456");

//    MainWindow w(std::shared_ptr<User>(User::login("ruiqurm","123456")));
    MainWindow w;
    w.show();
    return a.exec();
}
