#ifndef ADDGOODSDIALOG_H
#define ADDGOODSDIALOG_H
#include"include/concreteUser.h"
#include "market.h"
#include "usergoodsmanagement.h"

#include <QDialog>
#include<QString>
namespace Ui {
class AddGoodsDialog;
}

class AddGoodsDialog : public QDialog
{
    Q_OBJECT

public:
    //传递一个user指针 保证不会析构指针
    explicit AddGoodsDialog(User*,userGoodsManagement* ugm=nullptr,Goods*g=nullptr,QWidget *parent=nullptr);
    ~AddGoodsDialog();
signals:
//    void update_market();
    void update_management(shared_ptr<Goods> g);
private slots:
    void on_pushButton_clicked();

    void on_has_discount_stateChanged(int arg1);

private:
    Ui::AddGoodsDialog *ui;
    void give_hint(QString qs);
    void clear_hint();
    User* user;
    Goods* goods;
    std::shared_ptr<Discount> origin_discount;
};

#endif // ADDGOODSDIALOG_H
