#ifndef MARKET_H
#define MARKET_H
#include<QStandardItem>
#include<vector>
#include<memory>
#include <QWidget>
#include "include/concreteGoods.h"
#include "include/concreteUser.h"

namespace Ui {
class Market;
}

class Market : public QWidget
{
    Q_OBJECT

public:
    explicit Market(User* u=nullptr,QWidget *parent = nullptr);
    ~Market();
public slots:
    void update_data_default();
private slots:
    void on_search_button_clicked();

    void on_market_doubleClicked(const QModelIndex &index);

private:
    Ui::Market *ui;
    QStandardItemModel *model;
    void update_data(GoodsRecord::pGoodsVec vec);//passing uniquw pointer
    std::vector<std::shared_ptr<Goods>> goods;
    User* user;
};

#endif // MARKET_H
