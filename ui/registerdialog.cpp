
#include<QDebug>
#include<QMessageBox>
#include<QString>
#include<regex>
#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "include/concreteUser.h"
registerDialog::registerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

registerDialog::~registerDialog()
{
    delete ui;
}
void registerDialog::give_hint(QString qs){
    ui->hint->setText(qs);
    ui->hint->setStyleSheet("QLabel{color:rgb(255, 0, 0);}");//设置颜色为红色
}

void registerDialog::on_pushButton_clicked()
{
    int val=-1;
    if (ui->radioButton->isChecked())val=USER_TYPE::seller;
    else if(ui->radioButton_2->isChecked())val=USER_TYPE::customer;
    else{
        give_hint("未选择类别");return;
    }
    if(ui->lineEdit->text().isEmpty()){
        give_hint("用户名为空");return;
    }
    if(ui->lineEdit_2->text().isEmpty()||ui->lineEdit_3->text().isEmpty()){
        give_hint("密码为空");return;
    }
    if(!User::validate_username_format(ui->lineEdit->text().toStdString())){
      //提前校验
      give_hint("用户名长度在4-16位之间，且必须为字母、数字，或者-_,!@#$%^&*()");
      return;
    }
    if(!User::validate_password_format(ui->lineEdit_2->text().toStdString())){
      //提前校验
      give_hint("密码长度在4-16位之间，且必须为字母、数字，或者-_,!@#$%^&*()");
      return;
    }
    if (ui->lineEdit_3->text()!=ui->lineEdit_2->text()){
        ui->hint->setText("两次输入的密码不一样");
        ui->hint->setStyleSheet("QLabel{color:rgb(255, 0, 0);}");//设置颜色为红色
        return;
    }
    qDebug("\033[32m username = %s password = %s type = %s \033[0m",
           ui->lineEdit->text().toStdString().c_str(),
           ui->lineEdit_2->text().toStdString().c_str(),
           (val==USER_TYPE::seller)?"商家":(val==USER_TYPE::customer?"消费者":"未知"));
   int sign = User::register_(val,ui->lineEdit->text().toStdString(),ui->lineEdit_2->text().toStdString());
   QMessageBox *msgBox =  new QMessageBox(this);
   if(sign==false){
       qDebug()<<"注册失败";
       msgBox->setText("注册失败");
       msgBox->exec();
       //注册失败不会关闭
   }else{
       qDebug()<<"注册成功";
       msgBox->setText("注册成功");
       msgBox->exec();
       this->close();
   }
}
