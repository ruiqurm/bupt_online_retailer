#include "goodsbuying.h"
#include "ui_goodsbuying.h"
#include<QPushButton>
goodsBuying::goodsBuying(GoodsContext&context,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::goodsBuying),
    context(context)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("购买");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    ui->buttonBox->button(QDialogButtonBox::Save)->setText("加入购物车");
    ui->num->setMaximum(context.goods().remain());
    connect(ui->num, SIGNAL(valueChanged(int)), this, SLOT(compute_price(int)));
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

void goodsBuying::on_buttonBox_accepted()
{
//    ui->num->
    this->close();
}
