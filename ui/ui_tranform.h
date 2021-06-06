/********************************************************************************
** Form generated from reading UI file 'tranform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANFORM_H
#define UI_TRANFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_tranForm
{
public:
    QTableView *detail;
    QPushButton *pay;
    QPushButton *cancel;
    QLabel *price;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *seller;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *buyer;

    void setupUi(QDialog *tranForm)
    {
        if (tranForm->objectName().isEmpty())
            tranForm->setObjectName(QString::fromUtf8("tranForm"));
        tranForm->resize(388, 520);
        detail = new QTableView(tranForm);
        detail->setObjectName(QString::fromUtf8("detail"));
        detail->setGeometry(QRect(20, 80, 351, 321));
        pay = new QPushButton(tranForm);
        pay->setObjectName(QString::fromUtf8("pay"));
        pay->setGeometry(QRect(70, 480, 80, 23));
        cancel = new QPushButton(tranForm);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setGeometry(QRect(220, 480, 80, 23));
        price = new QLabel(tranForm);
        price->setObjectName(QString::fromUtf8("price"));
        price->setGeometry(QRect(40, 410, 321, 55));
        price->setStyleSheet(QString::fromUtf8("font: 24pt \"Sans Serif\";\n"
"color: rgb(255, 68, 0);"));
        widget = new QWidget(tranForm);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(30, 20, 157, 25));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        seller = new QLineEdit(widget);
        seller->setObjectName(QString::fromUtf8("seller"));
        seller->setReadOnly(true);

        horizontalLayout->addWidget(seller);

        widget1 = new QWidget(tranForm);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(30, 50, 157, 25));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        buyer = new QLineEdit(widget1);
        buyer->setObjectName(QString::fromUtf8("buyer"));
        buyer->setReadOnly(true);

        horizontalLayout_2->addWidget(buyer);


        retranslateUi(tranForm);

        QMetaObject::connectSlotsByName(tranForm);
    } // setupUi

    void retranslateUi(QDialog *tranForm)
    {
        tranForm->setWindowTitle(QApplication::translate("tranForm", "Dialog", nullptr));
        pay->setText(QApplication::translate("tranForm", "\344\273\230\346\254\276", nullptr));
        cancel->setText(QApplication::translate("tranForm", "\346\222\244\351\224\200", nullptr));
        price->setText(QApplication::translate("tranForm", "\302\2450.00", nullptr));
        label->setText(QApplication::translate("tranForm", "\345\215\226\345\256\266", nullptr));
        label_2->setText(QApplication::translate("tranForm", "\344\271\260\345\256\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class tranForm: public Ui_tranForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANFORM_H
