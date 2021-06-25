/**
 * @file categorydiscount.h
 * @author ruiqurm (ruiqurm@gmail.com)
 * @brief 管理品类打折
 */
#ifndef CATEGORYDISCOUNT_H
#define CATEGORYDISCOUNT_H
#include "include/concreteGoods.h"
#include <QDialog>
#include <QModelIndexList>
#include <QStandardItem>
namespace Ui {
class CategoryDiscount;
}

/**
 * @brief QT管理品类打折
 * 
 */
class CategoryDiscount : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryDiscount(QWidget *parent = nullptr);
    ~CategoryDiscount();

private slots:
    /// 品类打折数据修改
    void _on_model_change(QStandardItem *item);

//    void on_table_doubleClicked(const QModelIndex &index);

private:
    Ui::CategoryDiscount *ui;
    /// 表格数据
    QStandardItemModel *model;
    /// discount列表指针
    pDisVec data;
    /// 是否双击
    bool is_doubleclick;
    /// 双击时的index
    QModelIndex index;
};

#endif // CATEGORYDISCOUNT_H
