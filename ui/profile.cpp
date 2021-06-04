#include "profile.h"
#include "ui_profile.h"
#include "mainwindow.h"
#include<QMessageBox>
#include<QVariant>
#include<QInputDialog>
#include<QMessageBox>

Profile::Profile(User* user,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Profile),
    user(user)
{
    ui->setupUi(this);
    //setAttribute(Qt::WA_DeleteOnClose);
    ui->username->setText(QString::fromStdString(user->username()));
    ui->balance->setText(QVariant(user->balance()).toString());
    ui->balance->setReadOnly(true);
    qDebug("open profile");
    connect(this,&Profile::send_logout,static_cast<MainWindow*>(parent),&MainWindow::my_on_receive_logout);
//    if(parent->)
}

Profile::~Profile()
{
    delete ui;
    qDebug("close profile");
}
void Profile::give_hint(QString qs){
    ui->hint->setText(qs);
    ui->hint->setStyleSheet("QLabel{color:rgb(255, 0, 0);}");//设置颜色为红色
}
void Profile::on_pushButton_clicked()
{
    int change = 0;
    if (!ui->username->text().isEmpty()){
        if(User::validate_username_format(ui->username->text().toStdString())){
            change |= 1;
        }else{
            give_hint("名字格式错误");
            return ;
        }
    }
    if (!ui->new_password_1->text().isEmpty()){
        if(ui->new_password_1->text() == ui->new_password_2->text() && User::validate_password_format(ui->new_password_1->text().toStdString())){
            change |=2;
        }else{
            give_hint("密码格式错误");
            return;
        }
    }
    if(change==0){
                give_hint("没有输入");
                return;
    }
    if(change&1){
        user->username() = ui->username->text().toStdString();
    }
    if(change&2){
        user->password()= ui->new_password_1->text().toStdString();
    }
    qDebug("%d",user->save());

    QMessageBox msgBox;
    if(change==1){
        msgBox.setText("修改帐号成功,请重新登录");
    }else if (change==2){
        msgBox.setText("修改密码成功,请重新登录");
    }else if(change==3){
        msgBox.setText("修改帐号和密码成功,请重新登录");
    }
    msgBox.exec();
    emit send_logout();
}

void Profile::on_deposit_clicked()
{
    bool ok;
    double dep = QInputDialog::getDouble(this, tr("充值金额"),
                                 tr("输入要充值的金额"), 100, 0, 10000000,1, &ok);
    if(ok){
        user->balance()+=dep;
        ui->balance->setText(QVariant(user->balance()).toString());
        if(!user->save()){
            qDebug("User保存失败");
        }
    }else{
        QMessageBox msgBox(QMessageBox::Warning,tr("错误"),tr("非法的充值金额"));
        msgBox.exec();
    }
}
