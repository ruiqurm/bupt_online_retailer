/********************************************************************************
** Form generated from reading UI file 'addgoodsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDGOODSDIALOG_H
#define UI_ADDGOODSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddGoodsDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QLineEdit *name;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QTextEdit *description;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *price;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QLineEdit *count;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_8;
    QLineEdit *discount;
    QCheckBox *has_discount;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QComboBox *goodstype;
    QSpacerItem *horizontalSpacer;
    QLabel *hint;
    QPushButton *pushButton;

    void setupUi(QDialog *AddGoodsDialog)
    {
        if (AddGoodsDialog->objectName().isEmpty())
            AddGoodsDialog->setObjectName(QString::fromUtf8("AddGoodsDialog"));
        AddGoodsDialog->resize(385, 341);
        layoutWidget = new QWidget(AddGoodsDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(70, 0, 261, 331));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);

        name = new QLineEdit(layoutWidget);
        name->setObjectName(QString::fromUtf8("name"));

        horizontalLayout->addWidget(name);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        description = new QTextEdit(layoutWidget);
        description->setObjectName(QString::fromUtf8("description"));

        horizontalLayout_2->addWidget(description);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        price = new QLineEdit(layoutWidget);
        price->setObjectName(QString::fromUtf8("price"));

        horizontalLayout_3->addWidget(price);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        count = new QLineEdit(layoutWidget);
        count->setObjectName(QString::fromUtf8("count"));

        horizontalLayout_4->addWidget(count);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_5->addWidget(label_8);

        discount = new QLineEdit(layoutWidget);
        discount->setObjectName(QString::fromUtf8("discount"));
        discount->setEnabled(true);

        horizontalLayout_5->addWidget(discount);

        has_discount = new QCheckBox(layoutWidget);
        has_discount->setObjectName(QString::fromUtf8("has_discount"));

        horizontalLayout_5->addWidget(has_discount);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_6->addWidget(label_6);

        goodstype = new QComboBox(layoutWidget);
        goodstype->setObjectName(QString::fromUtf8("goodstype"));

        horizontalLayout_6->addWidget(goodstype);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_6);

        hint = new QLabel(layoutWidget);
        hint->setObjectName(QString::fromUtf8("hint"));

        verticalLayout->addWidget(hint);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);


        retranslateUi(AddGoodsDialog);

        QMetaObject::connectSlotsByName(AddGoodsDialog);
    } // setupUi

    void retranslateUi(QDialog *AddGoodsDialog)
    {
        AddGoodsDialog->setWindowTitle(QApplication::translate("AddGoodsDialog", "Dialog", nullptr));
        label_4->setText(QApplication::translate("AddGoodsDialog", "\345\225\206\345\223\201\345\220\215\347\247\260", nullptr));
        label_3->setText(QApplication::translate("AddGoodsDialog", "\347\256\200\344\273\213", nullptr));
        label_2->setText(QApplication::translate("AddGoodsDialog", "\344\273\267\346\240\274", nullptr));
        label->setText(QApplication::translate("AddGoodsDialog", "\346\225\260\351\207\217", nullptr));
        label_8->setText(QApplication::translate("AddGoodsDialog", "\346\212\230\346\211\243", nullptr));
        discount->setText(QApplication::translate("AddGoodsDialog", "1", nullptr));
        has_discount->setText(QApplication::translate("AddGoodsDialog", "\346\230\257\345\220\246\346\211\223\346\212\230", nullptr));
        label_6->setText(QApplication::translate("AddGoodsDialog", "\347\261\273\345\236\213", nullptr));
        hint->setText(QString());
        pushButton->setText(QApplication::translate("AddGoodsDialog", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddGoodsDialog: public Ui_AddGoodsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDGOODSDIALOG_H
