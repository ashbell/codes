/********************************************************************************
** Form generated from reading UI file 'untitled.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNTITLED_H
#define UI_UNTITLED_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GotoCellDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *GotoCellDialog)
    {
        if (GotoCellDialog->objectName().isEmpty())
            GotoCellDialog->setObjectName(QStringLiteral("GotoCellDialog"));
        GotoCellDialog->resize(242, 82);
        verticalLayout = new QVBoxLayout(GotoCellDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(GotoCellDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setEnabled(true);

        horizontalLayout->addWidget(label);


        horizontalLayout_3->addLayout(horizontalLayout);

        lineEdit = new QLineEdit(GotoCellDialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setEnabled(true);

        horizontalLayout_3->addWidget(lineEdit);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        cancelButton = new QPushButton(GotoCellDialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setEnabled(true);
        cancelButton->setAutoDefault(false);
        cancelButton->setDefault(true);

        horizontalLayout_2->addWidget(cancelButton);

        okButton = new QPushButton(GotoCellDialog);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setEnabled(false);
        okButton->setAutoDefault(false);
        okButton->setDefault(false);

        horizontalLayout_2->addWidget(okButton);


        verticalLayout->addLayout(horizontalLayout_2);

#ifndef QT_NO_SHORTCUT
        label->setBuddy(lineEdit);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(lineEdit, okButton);
        QWidget::setTabOrder(okButton, cancelButton);

        retranslateUi(GotoCellDialog);

        QMetaObject::connectSlotsByName(GotoCellDialog);
    } // setupUi

    void retranslateUi(QDialog *GotoCellDialog)
    {
        GotoCellDialog->setWindowTitle(QApplication::translate("GotoCellDialog", "Go To Cell", 0));
        label->setText(QApplication::translate("GotoCellDialog", "&Cell Location", 0));
        cancelButton->setText(QApplication::translate("GotoCellDialog", "Cancel", 0));
        okButton->setText(QApplication::translate("GotoCellDialog", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class GotoCellDialog: public Ui_GotoCellDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNTITLED_H
