#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include<memory>
#include <QDialog>
#include "include/concreteUser.h"
namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT
signals:
    void sendUser(std::shared_ptr<User>);//将要发出去的信号
public:
    explicit loginDialog(QWidget *parent = nullptr);
    ~loginDialog();


private slots:
    void on_pushButton_clicked();

private:
    Ui::loginDialog *ui;
    void give_hint(QString qs);
};

#endif // LOGINDIALOG_H
