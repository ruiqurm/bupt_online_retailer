/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_registerDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QLabel *hint;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *registerDialog)
    {
        if (registerDialog->objectName().isEmpty())
            registerDialog->setObjectName(QString::fromUtf8("registerDialog"));
        registerDialog->resize(400, 300);
        layoutWidget = new QWidget(registerDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(60, 40, 281, 211));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        horizontalLayout->setStretch(0, 2);
        horizontalLayout->setStretch(1, 8);

        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_2 = new QLineEdit(layoutWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(lineEdit_2);

        horizontalLayout_2->setStretch(0, 2);
        horizontalLayout_2->setStretch(1, 8);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        lineEdit_3 = new QLineEdit(layoutWidget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setEchoMode(QLineEdit::Password);

        horizontalLayout_4->addWidget(lineEdit_3);

        horizontalLayout_4->setStretch(0, 2);
        horizontalLayout_4->setStretch(1, 8);

        verticalLayout->addLayout(horizontalLayout_4);

        hint = new QLabel(layoutWidget);
        hint->setObjectName(QString::fromUtf8("hint"));
        hint->setWordWrap(true);

        verticalLayout->addWidget(hint);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        radioButton_2 = new QRadioButton(layoutWidget);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        horizontalLayout_3->addWidget(radioButton_2);

        radioButton = new QRadioButton(layoutWidget);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout_3->addWidget(radioButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_3);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        verticalLayout->setStretch(0, 3);
        verticalLayout->setStretch(1, 3);
        verticalLayout->setStretch(2, 3);
        verticalLayout->setStretch(3, 1);
        verticalLayout->setStretch(4, 3);
        verticalLayout->setStretch(5, 3);

        retranslateUi(registerDialog);

        QMetaObject::connectSlotsByName(registerDialog);
    } // setupUi

    void retranslateUi(QDialog *registerDialog)
    {
        registerDialog->setWindowTitle(QApplication::translate("registerDialog", "\346\263\250\345\206\214", nullptr));
        label->setText(QApplication::translate("registerDialog", "\350\264\246\345\217\267", nullptr));
        label_2->setText(QApplication::translate("registerDialog", "\345\257\206\347\240\201", nullptr));
        label_3->setText(QApplication::translate("registerDialog", "\345\206\215\346\254\241\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        hint->setText(QString());
        radioButton_2->setText(QApplication::translate("registerDialog", "\346\266\210\350\264\271\350\200\205", nullptr));
        radioButton->setText(QApplication::translate("registerDialog", "\345\225\206\345\256\266", nullptr));
        pushButton->setText(QApplication::translate("registerDialog", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class registerDialog: public Ui_registerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
