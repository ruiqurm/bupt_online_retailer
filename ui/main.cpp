#include "mainwindow.h"
#include<include/concreteUser.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(std::shared_ptr<User>(User::login("ruiqurm","123456")));
    w.show();
    return a.exec();
}
