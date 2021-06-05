#include "addgoodsdialog.h"
#include "ui_addgoodsdialog.h"
#include "include/concreteGoods.h"
#include "adapter.h"
#include <QVariant>
#include <QMessageBox>
AddGoodsDialog::AddGoodsDialog(User* u,userGoodsManagement* ugm,Goods* g,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddGoodsDialog),
    user(u),
    goods(g),
    manage(ugm),
    origin_discount(nullptr)
{
    if(ugm!=nullptr){
        connect(this,&AddGoodsDialog::update_management,ugm,&userGoodsManagement::_on_add_new_goods);
    }
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    if(g){
        ui->name->setText(QString::fromStdString(g->name()));
        ui->price->setText(QString::number(g->price()));
        ui->description->setText(QString::fromStdString(g->description()));
//        ui->goodstype->set
        ui->count->setText(QString::number(g->remain()));
        auto& record = DiscountRecord::get_record();
        origin_discount = record.get_goods_discount(g->id());
        if (origin_discount!=nullptr){//打过折
            ui->discount->setEnabled(true);
            ui->has_discount->setChecked(true);
            ui->discount->setText(QString::number(origin_discount->discount()));
        }else{
            ui->discount->setEnabled(false);
            ui->has_discount->setChecked(false);
        }
        ui->goodstype->setCurrentIndex(g->get_goods_type());
        ui->goodstype->setEditable(false);
    }else{
        ui->discount->setEnabled(false);
        for(int i=0;i<=GOODS_TYPE::EGoods;i++){
            ui->goodstype->addItem(qtGoodsAdapter::toString(i));
        }
    }
}

AddGoodsDialog::~AddGoodsDialog()
{
    delete ui;
}

void AddGoodsDialog::give_hint(QString qs){
    ui->hint->setText(qs);
    ui->hint->setStyleSheet("QLabel{color:rgb(255, 0, 0);}");//设置颜色为红色
}
void AddGoodsDialog::clear_hint(){
    ui->hint->clear();
}
void AddGoodsDialog::on_pushButton_clicked()
{
    double discount = 1;
    //处理折扣
    clear_hint();
    if(!ui->discount->text().isEmpty()){
        bool good=false;
        discount = ui->discount->text().toFloat(&good);
        if(!good){
            give_hint("非法的折扣");
            return;
        }
        if(discount<=0||discount>1){
            give_hint("非法的折扣");
            return;
        }
    }
    qDebug("折扣=%f",discount);

    //处理数量
    int count = 0;
    if(!ui->discount->text().isEmpty()){
        bool good=false;
        count = ui->count->text().toInt(&good);
        if(!good){
            give_hint("非法的数量");
            return;
        }
    }else{
        give_hint("未输入数量");
        return;
    }
    qDebug("数量=%d",count);

    //处理价格
    double price=0;
    if(!ui->price->text().isEmpty()){
        bool good=false;
        price = ui->price->text().toFloat(&good);
        if(!good || price<0){
            give_hint("非法的价格");
            return;
        }
    }else{
        give_hint("未输入价格");
        return;
    }
    qDebug("价格=%f",price);

    //处理商品名
    if(ui->name->text().isEmpty()){
        give_hint("未输入商品名");
        return;
    }
    if(ui->description->toPlainText().isEmpty()){
        give_hint("未输入描述");
        return;
    }
     QMessageBox msgBox;
//     msgBox.setAttribute(Qt::WA_DeleteOnClose, true);
    if(!goods){
        auto&record = GoodsRecord::get_record();
        GoodsData data(ui->name->text().toStdString(),price,user->id(),ui->goodstype->currentIndex(),
                       count,ui->description->toPlainText().toStdString());
        int id = record.set(data);
        auto& disrecord  = DiscountRecord::get_record();
        disrecord.make_discount(Discount::DISCOUNT,id,discount);
//        if(market!=nullptr)emit update_market();
        if(manage)emit update_management(record.get(id));
        msgBox.setText("创建成功");
    }else{
        goods->name() = ui->name->text().toStdString();
        goods->price() = price;
        goods->remain() = count;
        goods->description() = ui->description->toPlainText().toStdString();
        goods->save();
        if(ui->has_discount->isChecked() && origin_discount==nullptr){
            //原来没有折扣，现在有折扣
            auto& record = DiscountRecord::get_record();
            record.make_discount(Discount::DISCOUNT,goods->id(),discount,-1);
        }else if(!ui->has_discount->isChecked() && origin_discount!=nullptr){
            //原来有，现在没有折扣
            auto& record = DiscountRecord::get_record();
            record.remove(origin_discount->id());
        }else if(ui->has_discount->isChecked() && origin_discount!=nullptr){
            //原来有，现在还有
            if (origin_discount->discount()!=discount){
                auto& record = DiscountRecord::get_record();
                record.update_discount(origin_discount->id(),discount,-1);
            }
        }
//        if(has_goods_manage)emit update_management(std::shared_ptr<Goods>(goods));
        msgBox.setText("修改成功");
    }

    msgBox.exec();
    this->close();
}

void AddGoodsDialog::on_has_discount_stateChanged(int check)
{
//    qDebug("%d",check);
    if(check){
        ui->discount->setEnabled(true);
    }else{
        ui->discount->setEnabled(false);
    }
}
