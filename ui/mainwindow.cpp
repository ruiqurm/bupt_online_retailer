#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "include/concreteGoods.h"
#include "logindialog.h"
#include "registerdialog.h"
#include "market.h"
#include "profile.h"
#include "mainwindow.h"
#include "addgoodsdialog.h"
#include "usergoodsmanagement.h"
#include "categorydiscount.h"
#include<cstring>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow()),
    user(nullptr),
    mainframe(nullptr),
    market(nullptr),
    profile(nullptr),
    sellerMenu(nullptr)
{
    ui->setupUi(this);
    open_market();
//    menu->
//    ui->menubar[1].addAction("add_item");
//    connect()
//    QAction *login= this->menuBar()->
//    connect(login,SIGNAL(triggered()),this,SLOT();

}
MainWindow::MainWindow(std::shared_ptr<User>u):MainWindow(nullptr){
   user = u;
//   u.reset();
   on_login(u);
}
MainWindow::~MainWindow()
{
    mainframe->close();
    if(market)delete market;
    if(profile)delete profile;
    delete ui;
}


void MainWindow::on_action_login_triggered()
{
    qDebug("login");
    if(!user){
        loginDialog *dlg = new loginDialog(this);
        dlg->show();
    }else{
        QMessageBox box;
        box.setText("已经登陆");
        box.exec();
    }
}

void MainWindow::on_action_register_triggered()
{
    qDebug("register");
    registerDialog *dlg = new registerDialog(this);
    dlg->show();
}

void MainWindow::on_login(std::shared_ptr<User> user){
    qDebug("receive :\nusername:%s",
           user->username().c_str());
    this->user = user;
    if(user->get_user_type()==USER_TYPE::seller&&sellerMenu==nullptr){
        sellerMenu = ui->menubar->addMenu("商品管理");
        connect(sellerMenu->addAction("快速添加商品"), SIGNAL(triggered()), this, SLOT(my_on_action_add_item_triggered()));
        connect(sellerMenu->addAction("管理商品"), SIGNAL(triggered()), this, SLOT(my_on_action_manage_item_triggered()));
        connect(sellerMenu->addAction("管理品类打折"), SIGNAL(triggered()), this, SLOT(my_on_action_discount()));
    }
//    delete user;
}
void MainWindow::my_on_receive_logout(){
    if(!strcmp(mainframe->metaObject()->className(),"Profile")){
        open_market();
    }
    this->user = nullptr;
    delete this->profile;
    this->profile = nullptr;
    delete sellerMenu;
//    removeAction(sellerMenu);
    sellerMenu =nullptr;
}
void MainWindow::on_action_logout_triggered(){
    my_on_receive_logout();
}
void MainWindow::my_on_action_add_item_triggered(){
    //添加商品
    (new AddGoodsDialog(user.get()))->exec();

}
void MainWindow::my_on_action_manage_item_triggered(){
    (new userGoodsManagement(user.get()))->exec();
}

void MainWindow::open_market(){
    if(mainframe  && market){
        if(!strcmp(mainframe->metaObject()->className(),"Market")){
            return;//如果还是原来的
        }
        mainframe->close();
        mainframe = market;
        mainframe->show();
    }else{
        if(market){
            mainframe = market;
        }else{
            if(mainframe)mainframe->close();
            market  = mainframe = new Market(user.get(),this);
        }
        ui->horizontalLayout->addWidget(mainframe,12);
        mainframe->show();
    }
}
void MainWindow::open_profile(){
    if(!user){
        //还没登录
        loginDialog *dlg = new loginDialog(this);
        dlg->show();
        return;
    }
    if(mainframe  && profile){
        if(!strcmp(mainframe->metaObject()->className(),"Profile")){
            return ;//如果还是原来的
        }
        mainframe->close();
        mainframe = profile;
        mainframe->show();
    }else{
        if(profile){
            mainframe = profile;
        }else{
            if(mainframe)mainframe->close();
            profile  = mainframe = new Profile(user.get(),this);
        }
        ui->horizontalLayout->addWidget(mainframe,12);
        mainframe->show();
    }
}
void MainWindow::on_pushButton_clicked()
{
    open_profile();
}

void MainWindow::on_pushButton_2_clicked()
{
    open_market();
}

bool MainWindow::check_user_password(const QString&password){
    return user->validate_password(password.toStdString());
}

int MainWindow::get_user_id() const
{
    return user->id();
}
void MainWindow::my_on_action_discount(){
    (new CategoryDiscount())->exec();
}
//void MainWindow::set_user_name(const QString&name){
//    User::username();
//}
//void MainWindow::set_user_password(const QString&password);
//void MainWindow::set_user(const QString&name,const QString&password){
//    user->change_password()
//}
