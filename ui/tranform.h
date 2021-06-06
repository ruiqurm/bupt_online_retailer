#ifndef TRANFORM_H
#define TRANFORM_H
#include "include/transaction.h"
#include <QDialog>
#include<QStandardItem>
namespace Ui {
class tranForm;
}

class tranForm : public QDialog
{
    Q_OBJECT

public:
    explicit tranForm(int id,QWidget *parent = nullptr);
    explicit tranForm(std::shared_ptr<Transaction> tran,QWidget *parent = nullptr);
    ~tranForm();

private slots:
    void on_pay_clicked();

    void on_cancel_clicked();

private:
    Ui::tranForm *ui;
    std::shared_ptr<Transaction> tran;
     QStandardItemModel *model;
};

#endif // TRANFORM_H
