/**
 * @file logindialog.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 登录窗口页
 */
#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include<memory>
#include <QDialog>
#include "include/concreteUser.h"
namespace Ui {
class loginDialog;
}

/**
 * @brief QT登录窗口页
 * 
 */
class loginDialog : public QDialog
{
    Q_OBJECT
signals:
    /// 把用户发送一个接收者
    void sendUser(std::shared_ptr<User>);//将要发出去的信号
public:
    explicit loginDialog(QWidget *parent = nullptr);
    ~loginDialog();


private slots:
    /// 按钮确认触发
    void on_pushButton_clicked();

private:
    Ui::loginDialog *ui;
    /// 错误提示
    void give_hint(QString qs);
};

#endif // LOGINDIALOG_H
