#include "tranform.h"
#include "ui_tranform.h"
#include<QMessageBox>
tranForm::tranForm(int id,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tranForm),
    model(new QStandardItemModel(this))
{
    auto&record = TransactionRecord::get_record();
    tran = record.get(id);
    ui->setupUi(this);
    ui->seller->setText(QString::fromStdString(tran->from_name()));
    ui->buyer->setText(QString::fromStdString(tran->to_name()));
    model->setColumnCount(3);
    model->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("商品名"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("数量"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromUtf8("价格"));
    ui->detail->setModel(model);
    auto detail = tran->detail();
    int j = 0;
    for(auto it:*detail){
        model->setItem(j,0,new QStandardItem(QString::fromStdString(std::get<0>(it.second))));
        model->setItem(j,1,new QStandardItem(QString::number(std::get<1>(it.second))));
        model->setItem(j,2,new QStandardItem(QString::number(std::get<2>(it.second))));
        j++;
    }
    if(tran->is_finished()){
        ui->pay->setHidden(true);
        ui->cancel->setHidden(true);
    }
    ui->price->setText(QString("¥%1").arg(tran->volume()));
}
tranForm::tranForm(std::shared_ptr<Transaction> _tran,QWidget *parent):
    tran(_tran),
    model(new QStandardItemModel(this)),
    QDialog(parent),
    ui(new Ui::tranForm)
{
    ui->setupUi(this);
    ui->seller->setText(QString::fromStdString(tran->from_name()));
    ui->buyer->setText(QString::fromStdString(tran->to_name()));
    model->setColumnCount(3);
    model->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("商品名"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("数量"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromUtf8("价格"));
    ui->detail->setModel(model);
    auto detail = tran->detail();
    int j = 0;
    for(auto it:*detail){
        model->setItem(j,0,new QStandardItem(QString::fromStdString(std::get<0>(it.second))));
        model->setItem(j,1,new QStandardItem(QString::number(std::get<1>(it.second))));
        model->setItem(j,2,new QStandardItem(QString::number(std::get<2>(it.second))));
        j++;
    }
    if(tran->is_finished()){
        ui->pay->setHidden(true);
        ui->cancel->setHidden(true);
    }
    ui->price->setText(QString("¥%1").arg(tran->volume()));
}
tranForm::~tranForm()
{
    delete ui;
}

void tranForm::on_pay_clicked()
{
    QMessageBox box;
    if(tran->set_finished()){
        box.setText("支付成功");
        box.exec();
        this->close();
    }else{
        box.setText("支付失败");
        box.exec();
    }
}

void tranForm::on_cancel_clicked()
{
    QMessageBox box;
    tran->cancel();
    box.setText("取消成功");
    box.exec();
    this->close();
}
