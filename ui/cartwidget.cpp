#include "cartwidget.h"
#include "ui_cartwidget.h"
#include <QModelIndexList>
#include <QMessageBox>
#include"adapter.h"
#include<QSpinBox>
#include<QList>
#include "include/concreteUser.h"
#include "tranform.h"
cartWidget::cartWidget(User* u,QWidget *parent) :

    QWidget(parent),
    user(u),
    ui(new Ui::cartWidget),
     model(new QStandardItemModel(this)),
    cart(u->id())
{
    ui->setupUi(this);
    model->setColumnCount(5);
    model->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("商品编号"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("商品名"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromUtf8("价格"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromUtf8("类别"));
    model->setHeaderData(4,Qt::Horizontal,QString::fromUtf8("购买量"));
    flush_goods();
     ui->cart->setModel(model);
    connect(model,&QStandardItemModel::itemChanged,this,&cartWidget::_on_count_change);
}
void cartWidget::flush_goods(){
    auto &grecord = GoodsRecord::get_record();
    std::vector<int>v;
    for(auto&iter:cart.data()){
         v.push_back(iter.first);
    }

    auto goodsdata = grecord.get(v);
    int j =0;
    for (auto& goods:*goodsdata){
        if(!cart.exist(goods->id())){
            continue;
        }
        model->setItem(j,0,new QStandardItem(QString::number(goods->id())));
        model->setItem(j,1,new QStandardItem(QString::fromStdString(goods->name())));
        model->setItem(j,2,new QStandardItem(QString::number(goods->price())));
        model->setItem(j,3,new QStandardItem(qtGoodsAdapter::toString((goods->type()) ) ));
        model->setItem(j,4,new QStandardItem(QString::number(cart[goods->id()])));
//        ui->cart->setCellWidget(QSpinBox(this));
        j+=1;
    }
}
cartWidget::~cartWidget()
{
    delete ui;
}
void cartWidget::add_goods(Goods* goods,int num){
    qDebug("向购物车添加商品,good=%s,num=%d",goods->name().c_str(),num);
    if(cart.exist(goods->id())){
        int row = model->findItems(QString::number(goods->id()))[0]->index().row();
        cart.add(goods->id(),num,goods->remain());
        model->setItem(row,4,new QStandardItem(QString::number(cart[goods->id()])));
    }else{
        cart.add(goods->id(),num,goods->remain());
        QList<QStandardItem*> rowData;
        rowData<< new QStandardItem(QString::number(goods->id()))
               << new QStandardItem(QString::fromStdString(goods->name()))
               << new QStandardItem(QString::number(goods->price()))
               << new QStandardItem(qtGoodsAdapter::toString((goods->type()) ) )
               << new QStandardItem(QString::number(cart[goods->id()]));
       model->appendRow(rowData);
    }
}
void cartWidget::_on_count_change(QStandardItem *item){
    auto index = ui->cart->currentIndex();
    if(index.column()!=4){
        return;
    }
    bool ok=false;
    int count = model->data(index).toInt(&ok);
    int id = model->index(index.row(),0).data().toInt();
    if(!ok){
        item->setText(QString::number(cart[id]));
        return;
    }else{
        auto &record = GoodsRecord::get_record();
        if (cart.set(id,count,record.get(id)->remain())){
            item->setText(QString::number(cart[id]));
        }else{
            model->removeRow(index.row());
        }
    }
}

void cartWidget::on_submit_clicked(){
    QModelIndexList indexList = ui->cart->selectionModel()->selectedIndexes();
    std::vector<int>v;
    for(auto&i:indexList){
        int id = model->index(i.row(),0).data().toInt();
        v.push_back(id);
    }
    int id;
    if(user->get_user_type()==USER_TYPE::customer){
        auto p = Customer::cast(user);
        if ((id = p->buy(cart,v))>0){
            for(auto&i:indexList){
                model->removeRow(i.row());
            }
            qDebug("交易成功");
        }else{
            qDebug("交易失败");
            return;
        }
    }else{
        auto p = Seller::cast(user);
        if ((id = p->buy(cart,v))>0){
            for(auto&i:indexList){
                model->removeRow(i.row());
            }
            qDebug("交易成功");
        }else{
            qDebug("交易失败");
            return;
        }
    }
    (new tranForm(id))->exec();
}
