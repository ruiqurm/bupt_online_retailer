#ifndef GOODSBUYING_H
#define GOODSBUYING_H
#include<mainwindow.h>
#include <QDialog>
#include"include/concreteGoods.h"
namespace Ui {
class goodsBuying;
}

class goodsBuying : public QDialog
{
    Q_OBJECT
signals:
    void add_goods_to_cart(GoodsContext&context,double price);
public:
    explicit goodsBuying(GoodsContext&context,QWidget *parent = nullptr);
    ~goodsBuying();
public slots:
    double compute_price(int);
private slots:

    void on_cancel_clicked();

    void on_add_to_cart_clicked();

private:
    Ui::goodsBuying *ui;
    GoodsContext&context;
    double price;MainWindow* mainWindow;
};

#endif // GOODSBUYING_H
