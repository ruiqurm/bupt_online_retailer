/**
 * @file categorydiscount.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 购物车界面
 */
#ifndef CARTWIDGET_H
#define CARTWIDGET_H
#include "include/transaction.h"
#include <QWidget>
#include<QStandardItem>
namespace Ui {
class cartWidget;
}

/**
 * @brief QT购物车界面
 * 
 */
class cartWidget : public QWidget
{
    Q_OBJECT
public slots:
    /// 修改购买数量时除法
    void _on_count_change(QStandardItem *item);

    /// 添加商品槽
    void add_goods(Goods* g,int num);
    
    /// 刷新商品
    void flush_goods();
public:
    explicit cartWidget(User* u,QWidget *parent = nullptr);
    ~cartWidget();

private slots:

    /// 提交订单
    void on_submit_clicked();

private:
    Ui::cartWidget *ui;
    User* user;
    Cart cart;
    QStandardItemModel *model;
};

#endif // CARTWIDGET_H
