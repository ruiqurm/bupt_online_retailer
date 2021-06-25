/**
 * @file market.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 商品市场界面
 */
#ifndef MARKET_H
#define MARKET_H
#include<QStandardItem>
#include<vector>
#include<memory>
#include <QWidget>
#include "include/concreteGoods.h"
#include "include/concreteUser.h"
#include"mainwindow.h"
namespace Ui {
class Market;
}

/**
 * @brief QT商品市场界面
 * 
 */
class Market : public QWidget
{
    Q_OBJECT
signals:
    void login();
public:
    explicit Market(QWidget *parent = nullptr);
    ~Market();
public slots:
    /// 更新数据
    void update_data_default();
private slots:
    /// 搜索按钮触发
    void on_search_button_clicked();

    /// 商品被双击（购买）
    void on_market_doubleClicked(const QModelIndex &index);

private:
    Ui::Market *ui;
    /// 商品数据（字符串）
    QStandardItemModel *model;
    
    /// 刷新窗口中的数据
    void update_data(GoodsRecord::pGoodsVec vec);//passing uniquw pointer
    
    /// 商品数据
    std::vector<std::shared_ptr<Goods>> goods;
    MainWindow* mainwindow;
};

#endif // MARKET_H
