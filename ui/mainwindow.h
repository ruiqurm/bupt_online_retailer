/**
 * @file mainwindow.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 主窗口
 */
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
/**
 * @brief QT主窗口
 * 
 */
class MainWindow : public QMainWindow
{
     Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(std::shared_ptr<User>u);
    ~MainWindow();
    /// 检查用户密码
    bool check_user_password(const QString&password);

    /// 获取用户id
    int get_user_id()const;

    /// 设置用户名，未用
    void set_user_username(const QString&name);

    /// 设置用户密码
    void set_user_password(const QString&password);
    
    /// 未用
    void set_user(const QString&name,const QString&password);
    
    static MainWindow* getMainWindow();
    User* get_user(){
        return user.get();
    }
public slots:
    /// 弹出登录界面
    void login();
    /// 接收登录窗口发来的登录用户
    void on_login(std::shared_ptr<User>);

    /// 收到登出信号
    void my_on_receive_logout();

    /// 收到添加新商品到购物车信号
    void _on_add_goods_to_cart(GoodsContext&context,double price);
private slots:

    /// 登录按钮触发
    void on_action_login_triggered();

    /// 注册按钮触发    
    void on_action_register_triggered();

    /// 登出按钮触发    
    void on_action_logout_triggered();

    /// 添加商品按钮触发
    void my_on_action_add_item_triggered();

    /// 管理商品按钮触发
    void my_on_action_manage_item_triggered();

    /// 商品折扣触发
    void my_on_action_discount();

    /// 打开profile界面
    void on_pushButton_clicked();

    /// 打开market界面
    void on_pushButton_2_clicked();

    /// 打开cart界面
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
