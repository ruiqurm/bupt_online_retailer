#include "market.h"
#include "ui_market.h"
#include"goodsbuying.h"
#include<include/concreteGoods.h>
#include<QInputDialog>
#include"adapter.h"
#include<QString>
#include<exception>

Market::Market(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Market)
{
    ui->setupUi(this);
    qDebug("open market");
    //setAttribute(Qt::WA_DeleteOnClose);
    ui->comboBox_3->addItem("全部");
    ui->comboBox_3->addItem("商品名");
    ui->comboBox_3->addItem("商家");
    model = new QStandardItemModel(this);
    model->setColumnCount(6);
    ui->market->setColumnHidden(0,true);
    model->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("序号"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("商品名"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromUtf8("价格"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromUtf8("简介"));
    model->setHeaderData(4,Qt::Horizontal,QString::fromUtf8("剩余量"));
    model->setHeaderData(5,Qt::Horizontal,QString::fromUtf8("类别"));
    update_data_default();
    ui->market->setModel(model);
    ui->market->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->market->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     mainwindow = MainWindow::getMainWindow();
    connect(this,&Market::login,mainwindow,&MainWindow::login);
}
void Market::update_data_default(){
    auto& record = GoodsRecord::get_record();
    auto data = record.get_all_goods();
    int size = data->size();
    model->setRowCount(0);
    for(int i=0;i<size;i++){
        model->setItem(i, 0, new QStandardItem(QString::number((*data)[i]->id())));
        model->setItem(i, 1, new QStandardItem(QString::fromStdString((*data)[i]->name())));
        model->setItem(i, 2, new QStandardItem(QString::number((*data)[i]->price())));
        model->setItem(i, 3,new QStandardItem(QString::fromStdString((*data)[i]->description())));
        model->setItem(i, 4, new QStandardItem(QString::number((*data)[i]->remain())));
        model->setItem(i, 5, new QStandardItem(qtGoodsAdapter::toString((*data)[i]->type()) ) );
    }
    goods.swap(*data.release());
}

void Market::update_data(GoodsRecord::pGoodsVec data){
   model->setRowCount(0);
   int size = data->size();
   for(int i=0;i<size;i++){
       model->setItem(i, 0, new QStandardItem(QString::number((*data)[i]->id())));
       model->setItem(i, 1, new QStandardItem(QString::fromStdString((*data)[i]->name())));
       model->setItem(i, 2, new QStandardItem(QString::number((*data)[i]->price())));
       model->setItem(i, 3,new QStandardItem(QString::fromStdString((*data)[i]->description())));
       model->setItem(i, 4, new QStandardItem(QString::number((*data)[i]->remain())));
       model->setItem(i, 5, new QStandardItem(qtGoodsAdapter::toString((*data)[i]->type()) ));
   }
   goods.swap(*data.release());
}

Market::~Market()
{
    delete ui;
    qDebug("close market");
}


void Market::on_search_button_clicked()
{
    auto&record = GoodsRecord::get_record();
    if(ui->search_input->text().isEmpty()){
        update_data(record.get_all_goods());
    }else{
        update_data(record.get_goods_by_name(ui->search_input->text().toStdString()));
    }

//    int index = ui->comboBox_3->currentIndex();
//    switch (index) {
//    case 0:
//        break;
//    case 1:
//        break;
//    case 2:
//        break;
//    default:
//        return;
//    }
}

void Market::on_market_doubleClicked(const QModelIndex &index)
{
    User* user = mainwindow->get_user();
    if(user==nullptr){
        emit login();
        return;
    }
    int id = model->item(index.row(),0)->text().toInt();
//    qDebug("%d",id);
    auto it = std::find_if(goods.begin(),goods.end(),[id](shared_ptr<Goods>&g)->bool{return (g->id()==id);});
    Goods* p=nullptr;
    if(it!=goods.end()){
        p = it->get();
    }else{
        auto &record = GoodsRecord::get_record();
        auto p2 = record.get(id);
        p = p2.get();
        goods.push_back(p2);
     }
    GoodsContext context(p,1,user);
    (new goodsBuying(context,this))->exec();
//    int count = QInputDialog::getInt(nullptr,"","",0,1,)  (0, "购买","购买数量:", QLineEdit::Normal,1, &ok);
//    QInputDialog::getInt()

}
