/**
 * @file profile.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 用户界面
 */
#ifndef PROFILE_H
#define PROFILE_H
#include<QString>
#include <QWidget>
#include"include/user.h"
#include<QStandardItem>
#include "include/transaction.h"
namespace Ui {
class Profile;
}
/**
 * @brief QT用户个人信息界面
 * 
 */
class Profile : public QWidget
{
    Q_OBJECT
signals:
    /// 发送登出信息给主界面
    void send_logout();
public:
    explicit Profile(User* u,QWidget *parent = nullptr);
    ~Profile();

private slots:
    /// 修改密码按钮触发
    void on_pushButton_clicked();

    /// 充值按钮触发
    void on_deposit_clicked();

    /// 查看历史订单（双击了历史订单数据）
    void on_history_doubleClicked(const QModelIndex &index);

private:
    Ui::Profile *ui;
    /// 错误提示
    void give_hint(QString s);
    User* user;
    QStandardItemModel *model;
    /// 历史订单
    TransactionRecord::pTransVec trans;
};

#endif // PROFILE_H
