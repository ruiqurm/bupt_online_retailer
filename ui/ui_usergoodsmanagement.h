/********************************************************************************
** Form generated from reading UI file 'usergoodsmanagement.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERGOODSMANAGEMENT_H
#define UI_USERGOODSMANAGEMENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_userGoodsManagement
{
public:
    QTableView *table;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QPushButton *add_button;
    QPushButton *remove_button;
    QPushButton *copy_button_2;
    QPushButton *discount;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *userGoodsManagement)
    {
        if (userGoodsManagement->objectName().isEmpty())
            userGoodsManagement->setObjectName(QString::fromUtf8("userGoodsManagement"));
        userGoodsManagement->resize(414, 394);
        table = new QTableView(userGoodsManagement);
        table->setObjectName(QString::fromUtf8("table"));
        table->setEnabled(true);
        table->setGeometry(QRect(11, 11, 311, 371));
        layoutWidget = new QWidget(userGoodsManagement);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(320, 10, 88, 371));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(5, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        add_button = new QPushButton(layoutWidget);
        add_button->setObjectName(QString::fromUtf8("add_button"));

        verticalLayout->addWidget(add_button);

        remove_button = new QPushButton(layoutWidget);
        remove_button->setObjectName(QString::fromUtf8("remove_button"));

        verticalLayout->addWidget(remove_button);

        copy_button_2 = new QPushButton(layoutWidget);
        copy_button_2->setObjectName(QString::fromUtf8("copy_button_2"));

        verticalLayout->addWidget(copy_button_2);

        discount = new QPushButton(layoutWidget);
        discount->setObjectName(QString::fromUtf8("discount"));

        verticalLayout->addWidget(discount);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(userGoodsManagement);

        QMetaObject::connectSlotsByName(userGoodsManagement);
    } // setupUi

    void retranslateUi(QDialog *userGoodsManagement)
    {
        userGoodsManagement->setWindowTitle(QApplication::translate("userGoodsManagement", "Dialog", nullptr));
        add_button->setText(QApplication::translate("userGoodsManagement", "+", nullptr));
        remove_button->setText(QApplication::translate("userGoodsManagement", "-", nullptr));
        copy_button_2->setText(QApplication::translate("userGoodsManagement", "c", nullptr));
        discount->setText(QApplication::translate("userGoodsManagement", "\346\211\223\346\212\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class userGoodsManagement: public Ui_userGoodsManagement {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERGOODSMANAGEMENT_H
