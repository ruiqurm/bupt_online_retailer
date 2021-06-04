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

class userGoodsManagement : public QDialog
{
    Q_OBJECT
public:
    explicit userGoodsManagement(User *u,QWidget *parent = nullptr);
    ~userGoodsManagement();
public slots:
    void _on_add_new_goods(std::shared_ptr<Goods>g);
private slots:
    void on_add_button_clicked();


    void on_remove_button_clicked();


    void on_table_doubleClicked(const QModelIndex &index);

private:
    Ui::userGoodsManagement *ui;
    User* user;
    QStandardItemModel *model;
    GoodsRecord::pGoodsVec Goodsdata;
};

#endif // USERGOODSMANAGEMENT_H
