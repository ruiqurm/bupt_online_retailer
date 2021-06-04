#ifndef PROFILE_H
#define PROFILE_H
#include<QString>
#include <QWidget>
#include"include/user.h"
namespace Ui {
class Profile;
}

class Profile : public QWidget
{
    Q_OBJECT
signals:
    void send_logout();
public:
    explicit Profile(User* u,QWidget *parent = nullptr);
    ~Profile();

private slots:
    void on_pushButton_clicked();

    void on_deposit_clicked();

private:
    Ui::Profile *ui;
    void give_hint(QString s);
    User* user;
};

#endif // PROFILE_H
