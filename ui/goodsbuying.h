/**
 * @file goodsbuying.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 商品购买页
 */
#ifndef GOODSBUYING_H
#define GOODSBUYING_H

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
    /// 确认购买，未用
    void on_buttonBox_accepted();

private:
    Ui::goodsBuying *ui;
    GoodsContext&context;
    double price;
};

#endif // GOODSBUYING_H
