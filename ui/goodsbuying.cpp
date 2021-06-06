#include "goodsbuying.h"
#include "ui_goodsbuying.h"
#include"include/transaction.h"
#include<QPushButton>

#include<QWindow>
//QMainWindow* findMainWindow()
//{
//    for(QWidget* pWidget : QApplication::topLevelWidgets())
//    {
//        QMainWindow pMainWnd = qobject_cast<QMainWindow*>(pWidget);
//        if (pMainWnd)
//           return pMainWnd;
//    }
//    return nullptr;
//}
goodsBuying::goodsBuying(GoodsContext&context,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::goodsBuying),
    context(context)
{
    ui->setupUi(this);
    ui->buy->setText("购买");
    ui->cancel->setText("取消");
    ui->add_to_cart->setText("加入购物车");
    ui->num->setValue(1);
    ui->num->setMaximum(context.goods().remain());
    connect(ui->num, SIGNAL(valueChanged(int)), this, SLOT(compute_price(int)));
    mainWindow =dynamic_cast<MainWindow*>(MainWindow::getMainWindow());
    if(mainWindow){
        connect(this,&goodsBuying::add_goods_to_cart,
                mainWindow, &MainWindow::_on_add_goods_to_cart);
    }
}

double goodsBuying::compute_price(int num){
    context.num() = num;
    price = context.goods().get_price(context);
    ui->price->setText(QString("￥%1").arg(price));
    return price;
}
goodsBuying::~goodsBuying()
{
    delete ui;
}


void goodsBuying::on_cancel_clicked()
{
    this->close();
}

void goodsBuying::on_add_to_cart_clicked()
{
    if(price<=0){
        this->close();
    }
    emit add_goods_to_cart(context,price);
    this->close();
}
