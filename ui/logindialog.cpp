#include<QMessageBox>
#include"mainwindow.h"
#include "logindialog.h"
#include "ui_logindialog.h"

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(this,&loginDialog::sendUser,static_cast<const MainWindow*>(parent),&MainWindow::on_login);
}

loginDialog::~loginDialog()
{
    delete ui;
}
void loginDialog::give_hint(QString qs){
    ui->hint->setText(qs);
    ui->hint->setStyleSheet("QLabel{color:rgb(255, 0, 0);}");//设置颜色为红色
}
void loginDialog::on_pushButton_clicked()
{
    QMessageBox *msgBox =  new QMessageBox(this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose, true);

    std::shared_ptr<User> user(nullptr);
    if(ui->lineEdit->text().isEmpty()){
        give_hint("用户名不能为空");
        return;
    }
    if(ui->lineEdit_2->text().isEmpty()){
        give_hint("密码不能为空");
    }
    if(!User::validate_username_format(ui->lineEdit->text().toStdString()) ||
       !User::validate_password_format(ui->lineEdit_2->text().toStdString())){
      //提前校验
      give_hint("用户名或密码错误");
      return;
    }
    try{
        user = User::login(ui->lineEdit->text().toStdString(),
                    ui->lineEdit_2->text().toStdString());
    }catch(const char *){
        msgBox->setText("登录失败");
        msgBox->exec();
        //登录失败不会关闭
        return;
    }

    if(user!=nullptr){
        msgBox->setText("登录成功");
        emit sendUser(user);
    }else{
        msgBox->setText("登录失败");
    }
    int ret = msgBox->exec();
    if(ret==QMessageBox::Ok){
        this->close();
    }
}
