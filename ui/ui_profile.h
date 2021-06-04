/********************************************************************************
** Form generated from reading UI file 'profile.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROFILE_H
#define UI_PROFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Profile
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_6;
    QLabel *balance_2;
    QLineEdit *balance;
    QPushButton *deposit;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *username;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *new_password_1;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *new_password_2;
    QLabel *hint;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QWidget *Profile)
    {
        if (Profile->objectName().isEmpty())
            Profile->setObjectName(QString::fromUtf8("Profile"));
        Profile->resize(400, 300);
        layoutWidget = new QWidget(Profile);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(60, 60, 281, 238));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        balance_2 = new QLabel(layoutWidget);
        balance_2->setObjectName(QString::fromUtf8("balance_2"));

        horizontalLayout_6->addWidget(balance_2);

        balance = new QLineEdit(layoutWidget);
        balance->setObjectName(QString::fromUtf8("balance"));
        balance->setEnabled(true);

        horizontalLayout_6->addWidget(balance);

        deposit = new QPushButton(layoutWidget);
        deposit->setObjectName(QString::fromUtf8("deposit"));

        horizontalLayout_6->addWidget(deposit);

        horizontalLayout_6->setStretch(0, 4);
        horizontalLayout_6->setStretch(1, 10);
        horizontalLayout_6->setStretch(2, 4);

        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        username = new QLineEdit(layoutWidget);
        username->setObjectName(QString::fromUtf8("username"));
        username->setEnabled(false);

        horizontalLayout->addWidget(username);

        horizontalLayout->setStretch(0, 2);
        horizontalLayout->setStretch(1, 8);

        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));

        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        new_password_1 = new QLineEdit(layoutWidget);
        new_password_1->setObjectName(QString::fromUtf8("new_password_1"));
        new_password_1->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(new_password_1);

        horizontalLayout_2->setStretch(0, 2);
        horizontalLayout_2->setStretch(1, 8);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        new_password_2 = new QLineEdit(layoutWidget);
        new_password_2->setObjectName(QString::fromUtf8("new_password_2"));
        new_password_2->setEchoMode(QLineEdit::Password);

        horizontalLayout_4->addWidget(new_password_2);

        horizontalLayout_4->setStretch(0, 2);
        horizontalLayout_4->setStretch(1, 8);

        verticalLayout->addLayout(horizontalLayout_4);

        hint = new QLabel(layoutWidget);
        hint->setObjectName(QString::fromUtf8("hint"));
        hint->setWordWrap(true);

        verticalLayout->addWidget(hint);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_3);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        verticalLayout->setStretch(1, 3);
        verticalLayout->setStretch(3, 3);
        verticalLayout->setStretch(4, 3);
        verticalLayout->setStretch(5, 1);
        verticalLayout->setStretch(6, 3);
        verticalLayout->setStretch(7, 3);

        retranslateUi(Profile);

        QMetaObject::connectSlotsByName(Profile);
    } // setupUi

    void retranslateUi(QWidget *Profile)
    {
        Profile->setWindowTitle(QApplication::translate("Profile", "Form", nullptr));
        balance_2->setText(QApplication::translate("Profile", "\344\275\231\351\242\235", nullptr));
        deposit->setText(QApplication::translate("Profile", "\345\205\205\345\200\274", nullptr));
        label->setText(QApplication::translate("Profile", "\350\264\246\345\217\267", nullptr));
        label_2->setText(QApplication::translate("Profile", "\346\226\260\345\257\206\347\240\201", nullptr));
        label_3->setText(QApplication::translate("Profile", "\345\206\215\346\254\241\350\276\223\345\205\245", nullptr));
        hint->setText(QString());
        pushButton->setText(QApplication::translate("Profile", "\347\241\256\350\256\244\344\277\256\346\224\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Profile: public Ui_Profile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROFILE_H
