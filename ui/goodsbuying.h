/**
 * @file goodsbuying.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 商品购买页
 */
#ifndef GOODSBUYING_H
#define GOODSBUYING_H
#include<mainwindow.h>
#include <QDialog>
#include"include/concreteGoods.h"
namespace Ui {
class goodsBuying;
}

/**
 * @brief QT商品购买页
 * 
 */
class goodsBuying : public QDialog
{
    Q_OBJECT
signals:
    void add_goods_to_cart(GoodsContext&context,double price);
public:
    /**
     * @param context 购买上下文，里面包括用户，商品和数量等
     * @param parent 
     */
    explicit goodsBuying(GoodsContext&context,QWidget *parent = nullptr);
    ~goodsBuying();
public slots:
    /// 计算价格
    double compute_price(int);
private slots:

    void on_cancel_clicked();
    
    /// 添加到购物车
    void on_add_to_cart_clicked();

private:
    Ui::goodsBuying *ui;
    GoodsContext&context;
    double price;MainWindow* mainWindow;
};

#endif // GOODSBUYING_H
