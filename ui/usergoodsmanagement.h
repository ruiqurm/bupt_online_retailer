/**
 * @file usergoodsmanagement.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 管理商品窗口
 */
#ifndef USERGOODSMANAGEMENT_H
#define USERGOODSMANAGEMENT_H
#include "include/concreteUser.h"
#include "include/concreteGoods.h"
#include "market.h"
#include <QDialog>
#include<QStandardItem>
namespace Ui {
class userGoodsManagement;
}
/**
 * @brief QT管理商品窗口
 * 
 */
class userGoodsManagement : public QDialog
{
    Q_OBJECT
public:
    explicit userGoodsManagement(User *u,QWidget *parent = nullptr);
    ~userGoodsManagement();
public slots:
    /// 新商品被添加的信号（从添加商品框）发过来
    void _on_add_new_goods(std::shared_ptr<Goods>g);
private slots:
    /// 添加新商品的按钮触发，弹出添加商品框
    void on_add_button_clicked();

    /// 弹出移除商品框
    void on_remove_button_clicked();

    /// 修改信息。同样弹出添加商品框（里面填好数据了）
    void on_table_doubleClicked(const QModelIndex &index);

private:
    Ui::userGoodsManagement *ui;
    User* user;
    QStandardItemModel *model;
    GoodsRecord::pGoodsVec Goodsdata;
};

#endif // USERGOODSMANAGEMENT_H
