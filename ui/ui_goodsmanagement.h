/********************************************************************************
** Form generated from reading UI file 'goodsmanagement.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GOODSMANAGEMENT_H
#define UI_GOODSMANAGEMENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_goodsManagement
{
public:
    QTableView *table;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *goodsManagement)
    {
        if (goodsManagement->objectName().isEmpty())
            goodsManagement->setObjectName(QString::fromUtf8("goodsManagement"));
        goodsManagement->resize(468, 391);
        table = new QTableView(goodsManagement);
        table->setObjectName(QString::fromUtf8("table"));
        table->setGeometry(QRect(10, 10, 411, 371));
        widget = new QWidget(goodsManagement);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(430, 20, 31, 54));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);


        retranslateUi(goodsManagement);

        QMetaObject::connectSlotsByName(goodsManagement);
    } // setupUi

    void retranslateUi(QWidget *goodsManagement)
    {
        goodsManagement->setWindowTitle(QApplication::translate("goodsManagement", "\345\225\206\345\223\201\347\256\241\347\220\206", nullptr));
        pushButton->setText(QApplication::translate("goodsManagement", "+", nullptr));
        pushButton_2->setText(QApplication::translate("goodsManagement", "-", nullptr));
    } // retranslateUi

};

namespace Ui {
    class goodsManagement: public Ui_goodsManagement {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOODSMANAGEMENT_H
