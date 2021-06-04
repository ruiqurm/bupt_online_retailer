/********************************************************************************
** Form generated from reading UI file 'goodsbuying.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GOODSBUYING_H
#define UI_GOODSBUYING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_goodsBuying
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *price;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QSpinBox *num;

    void setupUi(QDialog *goodsBuying)
    {
        if (goodsBuying->objectName().isEmpty())
            goodsBuying->setObjectName(QString::fromUtf8("goodsBuying"));
        goodsBuying->resize(318, 218);
        buttonBox = new QDialogButtonBox(goodsBuying);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(20, 160, 281, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::Save);
        buttonBox->setCenterButtons(true);
        layoutWidget = new QWidget(goodsBuying);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(50, 30, 231, 121));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        price = new QLabel(layoutWidget);
        price->setObjectName(QString::fromUtf8("price"));
        price->setStyleSheet(QString::fromUtf8("font: 24pt \"Sans Serif\";\n"
"color: rgb(255, 68, 0);"));

        horizontalLayout_2->addWidget(price);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        num = new QSpinBox(layoutWidget);
        num->setObjectName(QString::fromUtf8("num"));
        num->setMinimum(0);

        horizontalLayout->addWidget(num);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(goodsBuying);
        QObject::connect(buttonBox, SIGNAL(accepted()), goodsBuying, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), goodsBuying, SLOT(reject()));

        QMetaObject::connectSlotsByName(goodsBuying);
    } // setupUi

    void retranslateUi(QDialog *goodsBuying)
    {
        goodsBuying->setWindowTitle(QApplication::translate("goodsBuying", "Dialog", nullptr));
        label->setText(QApplication::translate("goodsBuying", "\351\242\204\350\256\241\344\273\267\346\240\274", nullptr));
        price->setText(QApplication::translate("goodsBuying", "\302\2450.00", nullptr));
        label_2->setText(QApplication::translate("goodsBuying", "\346\225\260\351\207\217", nullptr));
        num->setPrefix(QString());
    } // retranslateUi

};

namespace Ui {
    class goodsBuying: public Ui_goodsBuying {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOODSBUYING_H
