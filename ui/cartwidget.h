#ifndef CARTWIDGET_H
#define CARTWIDGET_H
#include "include/transaction.h"
#include <QWidget>
#include<QStandardItem>
namespace Ui {
class cartWidget;
}

class cartWidget : public QWidget
{
    Q_OBJECT
public slots:
    void _on_count_change(QStandardItem *item);
    void add_goods(Goods* g,int num);
    void flush_goods();
public:
    explicit cartWidget(User* u,QWidget *parent = nullptr);
    ~cartWidget();

private slots:
    void on_submit_clicked();

private:
    Ui::cartWidget *ui;
    User* user;
    Cart cart;
    QStandardItemModel *model;
};

#endif // CARTWIDGET_H
