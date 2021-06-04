#include "categorydiscount.h"
#include "ui_categorydiscount.h"
#include<QMessageBox>
#include "adapter.h"

CategoryDiscount::CategoryDiscount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoryDiscount),
    model(new QStandardItemModel(this))
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    model->setColumnCount(2);
    model->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("类别"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("折扣率"));
    auto&record = DiscountRecord::get_record();
    data = record.get_all_category_discount();
    for(int i=0;i<=GOODS_TYPE::EGoods;i++){
        model->setItem(i,0,new QStandardItem(qtGoodsAdapter::toString(i)));
        auto item = model->item(i,0);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        auto it = std::find_if(data->begin(),data->end(),[i](shared_ptr<Discount> &d)->bool{return (d->operand()==i);});
        if(it!=data->end()){
            model->setItem(i,1,new QStandardItem(QString::number((*it)->discount())));
        }else{
            model->setItem(i,1,new QStandardItem(QString::number(1.0)));
        }

    }
    connect(model,&QStandardItemModel::itemChanged,this,&CategoryDiscount::_on_model_change);
//    QList<QStandardItem*> rowData;
//    for(const auto&it:*_data){
//        model->setItem();
//        it->operand()
//               << new QStandardItem(QString::number(it->discount()));
//       model->appendRow(rowData);
//    }
//    rowData<< new QStandardItem(qtGoodsAdapter::toString(it->operand()))
//           << new QStandardItem(QString::number(it->discount()));
    ui->table->setModel(model);
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

CategoryDiscount::~CategoryDiscount()
{
    delete ui;
}


void CategoryDiscount::_on_model_change(QStandardItem *item)
{
    //修改折扣率
    qDebug("修改折扣率");
    bool ok=false;
    auto index = ui->table->currentIndex();
    int row = index.row();
    double new_value = model->data(index).toFloat(&ok);
    if(!ok || new_value>1||new_value<=0){
        QMessageBox msgBox;
        msgBox.setText("输入出错");
        msgBox.exec();
        auto it = std::find_if(data->begin(),data->end(),[row](shared_ptr<Discount> &d)->bool{return (d->operand()==row);});
        if(it!=data->end()){
            item->setText(QString::number((*it)->discount()));
        }else{
            item->setText(QString::number(1.0));
        }
        return;
    }

    auto&record = DiscountRecord::get_record();
    auto it = std::find_if(data->begin(),data->end(),[row](shared_ptr<Discount> &d)->bool{return (d->operand()==row);});
    if(it!=data->end()){
        //已经存在
        if(qFuzzyCompare(new_value,1)){
            record.remove((*it)->id());
        }else{
            record.update_discount((*it)->id(),new_value,-1);
        }
    }else{
        if(qFuzzyCompare(new_value,1)){
            return;
        }
        record.make_discount(Discount::TYPE_DISCOUNT,row,new_value,-1);
        data->push_back(record.get_category_discount(new_value));
    }
    qDebug("修改成功");
}

//void CategoryDiscount::on_table_doubleClicked(const QModelIndex &index)
//{
//    is_doubleclick = true;
//}
