#ifndef CATEGORYDISCOUNT_H
#define CATEGORYDISCOUNT_H
#include "include/concreteGoods.h"
#include <QDialog>
#include <QModelIndexList>
#include <QStandardItem>
namespace Ui {
class CategoryDiscount;
}

class CategoryDiscount : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryDiscount(QWidget *parent = nullptr);
    ~CategoryDiscount();

private slots:
    void _on_model_change(QStandardItem *item);

//    void on_table_doubleClicked(const QModelIndex &index);

private:
    Ui::CategoryDiscount *ui;
    QStandardItemModel *model;
    pDisVec data;
    bool is_doubleclick;
    QModelIndex index;
};

#endif // CATEGORYDISCOUNT_H
