#ifndef GOODSBUYING_H
#define GOODSBUYING_H

#include <QDialog>
#include"include/concreteGoods.h"
namespace Ui {
class goodsBuying;
}

class goodsBuying : public QDialog
{
    Q_OBJECT

public:
    explicit goodsBuying(GoodsContext&context,QWidget *parent = nullptr);
    ~goodsBuying();
public slots:
    double compute_price(int);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::goodsBuying *ui;
    GoodsContext&context;
    double price;
};

#endif // GOODSBUYING_H
