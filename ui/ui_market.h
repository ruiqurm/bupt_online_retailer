/********************************************************************************
** Form generated from reading UI file 'market.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARKET_H
#define UI_MARKET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Market
{
public:
    QWidget *MainFrame;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_6;
    QComboBox *comboBox_3;
    QLineEdit *search_input;
    QPushButton *search_button;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label;
    QLineEdit *lineEdit_5;
    QLabel *label_4;
    QLineEdit *lineEdit_6;
    QSpacerItem *horizontalSpacer_2;
    QTableView *market;

    void setupUi(QWidget *Market)
    {
        if (Market->objectName().isEmpty())
            Market->setObjectName(QString::fromUtf8("Market"));
        Market->resize(699, 382);
        MainFrame = new QWidget(Market);
        MainFrame->setObjectName(QString::fromUtf8("MainFrame"));
        MainFrame->setGeometry(QRect(0, 0, 711, 393));
        verticalLayout_4 = new QVBoxLayout(MainFrame);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 5, -1, -1);
        horizontalSpacer_6 = new QSpacerItem(25, 24, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        comboBox_3 = new QComboBox(MainFrame);
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));
        comboBox_3->setEditable(false);

        horizontalLayout_4->addWidget(comboBox_3);

        search_input = new QLineEdit(MainFrame);
        search_input->setObjectName(QString::fromUtf8("search_input"));

        horizontalLayout_4->addWidget(search_input);

        search_button = new QPushButton(MainFrame);
        search_button->setObjectName(QString::fromUtf8("search_button"));

        horizontalLayout_4->addWidget(search_button);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        horizontalLayout_4->setStretch(0, 3);
        horizontalLayout_4->setStretch(2, 6);
        horizontalLayout_4->setStretch(3, 2);
        horizontalLayout_4->setStretch(4, 3);

        verticalLayout_4->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label = new QLabel(MainFrame);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_6->addWidget(label);

        lineEdit_5 = new QLineEdit(MainFrame);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));

        horizontalLayout_6->addWidget(lineEdit_5);

        label_4 = new QLabel(MainFrame);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_6->addWidget(label_4);

        lineEdit_6 = new QLineEdit(MainFrame);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));

        horizontalLayout_6->addWidget(lineEdit_6);


        horizontalLayout_5->addLayout(horizontalLayout_6);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        horizontalLayout_5->setStretch(0, 2);
        horizontalLayout_5->setStretch(1, 3);

        verticalLayout_4->addLayout(horizontalLayout_5);

        market = new QTableView(MainFrame);
        market->setObjectName(QString::fromUtf8("market"));

        verticalLayout_4->addWidget(market);


        retranslateUi(Market);

        QMetaObject::connectSlotsByName(Market);
    } // setupUi

    void retranslateUi(QWidget *Market)
    {
        Market->setWindowTitle(QApplication::translate("Market", "Form", nullptr));
        search_button->setText(QApplication::translate("Market", "\346\220\234\347\264\242", nullptr));
        label->setText(QApplication::translate("Market", "\344\273\267\346\240\274\345\214\272\351\227\264:", nullptr));
        label_4->setText(QApplication::translate("Market", "-", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Market: public Ui_Market {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARKET_H
