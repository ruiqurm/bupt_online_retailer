#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<memory>
#include<include/user.h>
#include <QMainWindow>
#include<QString>
#include<QMenu>
#include "include/concreteUser.h"
#include "cartwidget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
     Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(std::shared_ptr<User>u);
    ~MainWindow();
    bool check_user_password(const QString&password);
    int get_user_id()const;
    void set_user_username(const QString&name);
    void set_user_password(const QString&password);
    void set_user(const QString&name,const QString&password);
    static MainWindow* getMainWindow();
    User* get_user(){
        return user.get();
    }
public slots:
    void login();
    void on_login(std::shared_ptr<User>);
    void my_on_receive_logout();
    void _on_add_goods_to_cart(GoodsContext&context,double price);
private slots:
    void on_action_login_triggered();
    void on_action_register_triggered();
    void on_action_logout_triggered();
    void my_on_action_add_item_triggered();
    void my_on_action_manage_item_triggered();
    void my_on_action_discount();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    void open_market();
    void open_profile();
    void open_cart();
    Ui::MainWindow *ui;
    std::shared_ptr<User>user;
    QWidget * mainframe,
            * market,
            * cart;
    QMenu* sellerMenu;
};
#endif // MAINWINDOW_H
