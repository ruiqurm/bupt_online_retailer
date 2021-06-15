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
#include "include/transaction.h"
#include<cstring>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow()),
    user(nullptr),
    mainframe(nullptr),
    market(nullptr),
    sellerMenu(nullptr),
    cart(nullptr)
{
    ui->setupUi(this);
    market = mainframe = new Market;
    mainframe->show();
    ui->horizontalLayout->addWidget(mainframe,12);
//    open_market();
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
void MainWindow::login(){
    loginDialog *dlg = new loginDialog(this);
    dlg->show();
    return;
}
void MainWindow::on_login(std::shared_ptr<User> user){
    qDebug("receive :\nusername:%s,type=%d",
           user->username().c_str(),user->get_user_type());
    this->user = user;
    if(user->get_user_type()==USER_TYPE::seller&&sellerMenu==nullptr){
        sellerMenu = ui->menubar->addMenu("商品管理");
        connect(sellerMenu->addAction("快速添加商品"), SIGNAL(triggered()), this, SLOT(my_on_action_add_item_triggered()));
        connect(sellerMenu->addAction("管理商品"), SIGNAL(triggered()), this, SLOT(my_on_action_manage_item_triggered()));
        connect(sellerMenu->addAction("管理品类打折"), SIGNAL(triggered()), this, SLOT(my_on_action_discount()));
    }
    cart  = new cartWidget(user.get(),this);
    ui->horizontalLayout->addWidget(cart,12);
    cart->hide();
    mainframe =market;
    market->show();

//    delete user;
}
void MainWindow::my_on_receive_logout(){
    mainframe->hide();
    mainframe = market;
    this->user->logout();
    this->user.reset();
//    this->user = nullptr;
    if(this->cart){
        delete this->cart;
        this->cart = nullptr;
    }
    delete sellerMenu;
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
    if(!strcmp(mainframe->metaObject()->className(),"Market")){
        return;//如果还是原来的
    }else if(!strcmp(mainframe->metaObject()->className(),"Profile")){
        delete mainframe;
        mainframe = market;
    }else{
        //切换到cart
        mainframe->hide();
        mainframe = market;
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
    if(!strcmp(mainframe->metaObject()->className(),"Profile")){
        return;
    }else{
        mainframe->hide();
        mainframe = new Profile(user.get(),this);
        ui->horizontalLayout->addWidget(mainframe,12);
        mainframe->show();
    }
}
void MainWindow::open_cart(){
    if(!user){
        //还没登录
        loginDialog *dlg = new loginDialog(this);
        dlg->show();
        return;
    }
    if(!strcmp(mainframe->metaObject()->className(),"cartWidget")){
        return ;//如果还是原来的
    }else if(!strcmp(mainframe->metaObject()->className(),"Profile")){
        delete mainframe;
        mainframe = cart;
        mainframe->show();
    }else{
        mainframe->hide();
        mainframe = cart;
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

void MainWindow::on_pushButton_3_clicked()
{
    open_cart();
}
void  MainWindow::_on_add_goods_to_cart(GoodsContext&context,double price){
    if(cart){
        dynamic_cast<cartWidget*>(cart)->add_goods(&context.goods(),context.num());
    }
}
MainWindow* MainWindow::getMainWindow()
{
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
            return dynamic_cast<MainWindow*>(mainWin);
    return nullptr;
}
