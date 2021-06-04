/********************************************************************************
** Form generated from reading UI file 'categorydiscount.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CATEGORYDISCOUNT_H
#define UI_CATEGORYDISCOUNT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_CategoryDiscount
{
public:
    QTableView *table;

    void setupUi(QDialog *CategoryDiscount)
    {
        if (CategoryDiscount->objectName().isEmpty())
            CategoryDiscount->setObjectName(QString::fromUtf8("CategoryDiscount"));
        CategoryDiscount->resize(364, 364);
        table = new QTableView(CategoryDiscount);
        table->setObjectName(QString::fromUtf8("table"));
        table->setGeometry(QRect(20, 20, 311, 331));

        retranslateUi(CategoryDiscount);

        QMetaObject::connectSlotsByName(CategoryDiscount);
    } // setupUi

    void retranslateUi(QDialog *CategoryDiscount)
    {
        CategoryDiscount->setWindowTitle(QApplication::translate("CategoryDiscount", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CategoryDiscount: public Ui_CategoryDiscount {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CATEGORYDISCOUNT_H
