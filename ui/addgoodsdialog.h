/**
 * @file addgoodsdialog.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 添加商品的窗口
 */
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

/**
 * @brief QT添加、修改商品
 * @details 可以添加新商品也可以修改旧商品
 */
class AddGoodsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @param u 所属用户
     * @param ugm 商品管理界面的指针
     * @param g 商品
     * @param parent 
     */    
    explicit AddGoodsDialog(User*,userGoodsManagement* ugm=nullptr,Goods*g=nullptr,QWidget *parent=nullptr);
    ~AddGoodsDialog();
signals:
//    void update_market();
    /// 更新商品管理页
    void update_management(shared_ptr<Goods> g);
private slots:
    /// 提交数据按钮触发    
    void on_pushButton_clicked();
    /// 是否有折扣按钮变化时
    void on_has_discount_stateChanged(int arg1);

private:
    Ui::AddGoodsDialog *ui;
    
    /// 添加输入提示字符串
    void give_hint(QString qs);
    
    /// 清空输入提示字符串
    void clear_hint();
    User* user;
    Goods* goods;
    
    /// 用户商品管理页
    userGoodsManagement* manage;
    /// 原先商品的折扣
    std::shared_ptr<Discount> origin_discount;
};

#endif // ADDGOODSDIALOG_H
