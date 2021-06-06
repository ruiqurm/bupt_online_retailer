/********************************************************************************
** Form generated from reading UI file 'cartwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CARTWIDGET_H
#define UI_CARTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_cartWidget
{
public:
    QTableView *cart;
    QPushButton *submit;
    QPushButton *clear;

    void setupUi(QWidget *cartWidget)
    {
        if (cartWidget->objectName().isEmpty())
            cartWidget->setObjectName(QString::fromUtf8("cartWidget"));
        cartWidget->resize(659, 409);
        cart = new QTableView(cartWidget);
        cart->setObjectName(QString::fromUtf8("cart"));
        cart->setGeometry(QRect(30, 30, 591, 331));
        cart->setEditTriggers(QAbstractItemView::AnyKeyPressed|QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);
        submit = new QPushButton(cartWidget);
        submit->setObjectName(QString::fromUtf8("submit"));
        submit->setGeometry(QRect(80, 370, 80, 23));
        clear = new QPushButton(cartWidget);
        clear->setObjectName(QString::fromUtf8("clear"));
        clear->setGeometry(QRect(170, 370, 80, 23));

        retranslateUi(cartWidget);

        QMetaObject::connectSlotsByName(cartWidget);
    } // setupUi

    void retranslateUi(QWidget *cartWidget)
    {
        cartWidget->setWindowTitle(QApplication::translate("cartWidget", "Form", nullptr));
        submit->setText(QApplication::translate("cartWidget", "\346\217\220\344\272\244\350\256\242\345\215\225", nullptr));
        clear->setText(QApplication::translate("cartWidget", "\346\270\205\347\251\272\350\264\255\347\211\251\350\275\246", nullptr));
    } // retranslateUi

};

namespace Ui {
    class cartWidget: public Ui_cartWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CARTWIDGET_H
