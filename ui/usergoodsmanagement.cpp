#include "usergoodsmanagement.h"
#include "ui_usergoodsmanagement.h"
#include "addgoodsdialog.h"
#include "adapter.h"
#include <QModelIndexList>
#include<QList>
#include<QMessageBox>
#include <QTextStream>

userGoodsManagement::userGoodsManagement(User* u,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userGoodsManagement),
    user(u),
    model(new QStandardItemModel(this)),
    Goodsdata(nullptr)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    model->setColumnCount(6);
    model->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("商品编号"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("商品名"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromUtf8("价格"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromUtf8("简介"));
    model->setHeaderData(4,Qt::Horizontal,QString::fromUtf8("剩余量"));
    model->setHeaderData(5,Qt::Horizontal,QString::fromUtf8("类别"));
    auto & record = GoodsRecord::get_record();
    auto _data = record.get_user_goods(user->id());
    int size = _data->size();
    QList<QStandardItem*> rowData;
    for(const auto&it:*_data){
        rowData.clear();
        rowData<< new QStandardItem(QString::number(it->id()))
               << new QStandardItem(QString::fromStdString(it->name()))
               << new QStandardItem(QString::number(it->price()))
               << new QStandardItem(QString::fromStdString(it->description()))
               << new QStandardItem(QString::number(it->remain()))
               << new QStandardItem(qtGoodsAdapter::toString((it->type()) ) );
       model->appendRow(rowData);
    }
    Goodsdata.swap(_data);
    ui->table->setModel(model);
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

userGoodsManagement::~userGoodsManagement()
{
    delete ui;
}

void userGoodsManagement::_on_add_new_goods(std::shared_ptr<Goods>goods)
{
    int id = goods->id();
    auto it = std::find_if(Goodsdata->begin(),Goodsdata->end(),[id](std::shared_ptr<Goods>g)->bool{return g->id()==id;});
    if(it!=Goodsdata->end()){
        int row = model->findItems(QString::number(goods->id()))[0]->index().row();
//        int row = ui->table->findChild()->index().row();
        model->item(row,1)->setText(QString::fromStdString(goods->name()));
        model->item(row,2)->setText(QString::number(goods->price()));
        model->item(row,3)->setText(QString::fromStdString(goods->description()));
        model->item(row,4)->setText(QString::number(goods->remain()));
        model->item(row,5)->setText(qtGoodsAdapter::toString((goods->type()) ));
        (*it).swap(goods);
    }else{
        QList<QStandardItem*> rowData;
        rowData<< new QStandardItem(QString::number(goods->id()))
               << new QStandardItem(QString::fromStdString(goods->name()))
               << new QStandardItem(QString::number(goods->price()))
               << new QStandardItem(QString::fromStdString(goods->description()))
               << new QStandardItem(QString::number(goods->remain()))
               << new QStandardItem(qtGoodsAdapter::toString((goods->type()) ) );
       Goodsdata->push_back(goods);
       model->appendRow(rowData);
   }
}

void userGoodsManagement::on_add_button_clicked()
{
    (new AddGoodsDialog(user,this))->exec();//内部自动析构
}



void userGoodsManagement::on_remove_button_clicked(){
    QModelIndexList indexList = ui->table->selectionModel()->selectedIndexes();
    QMessageBox msgBox(QMessageBox::Warning,"警告","",QMessageBox::YesToAll|QMessageBox::Yes|QMessageBox::No);
//    msgBox.setAttribute(Qt::WA_DeleteOnClose, true);
//    if (msgBox->exec()==1){
//     QTextStream textStream("");
    QString pattern("尝试删除第%1行");
    auto &record =GoodsRecord::get_record();
    bool delete_all = false;
    int ret = -1;
    int last_one = -1;
        for (auto& i:indexList){
            if(last_one!=-1&&last_one==i.row()){
                continue;
            }else{
                last_one = i.row();
            }
            if(!delete_all){
                msgBox.setText(pattern.arg(i.row()+1));
                ret = msgBox.exec();
                if(ret==QMessageBox::YesToAll){
                    delete_all=true;
                }else if(ret!=QMessageBox::Yes){
                    continue;
                }
            }
            int id = (*Goodsdata)[i.row()]->id();
            Goodsdata->erase(Goodsdata->begin()+i.row());
            record.remove(id);
            model->removeRow(i.row());
        }
}

//void userGoodsManagement::on_copy_button_clicked()
//{
//    QModelIndexList indexList = ui->table->selectionModel()->selectedIndexes();
//    QList<QStandardItem*> rowData;
//    int last_one = -1;
//    GoodsData d;
//    for (auto& i:indexList){
//        if(last_one!=-1&&last_one==i.row()){
//            continue;
//        }else{
//            last_one = i.row();
//        }
//        model->row
//        rowData<< new QStandardItem(QString::fromStdString(data.name))
//               << new QStandardItem(QString::number(data.price))
//               << new QStandardItem(QString::fromStdString(data.description))
//               << new QStandardItem(QString::number(data.remain))
//               << new QStandardItem(qtGoodsAdapter::toString((data.type) ) );
//       model->appendRow(rowData);
//        qDebug("%d ",i.row());
//    }
//}

void userGoodsManagement::on_table_doubleClicked(const QModelIndex &index)
{
    int row = index.row();

    (new AddGoodsDialog(user,this,(* Goodsdata)[row].get()))->exec();//内部自动析构
}


