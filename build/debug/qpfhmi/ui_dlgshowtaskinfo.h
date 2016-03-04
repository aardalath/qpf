/********************************************************************************
** Form generated from reading UI file 'dlgshowtaskinfo.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGSHOWTASKINFO_H
#define UI_DLGSHOWTASKINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DlgShowTaskInfo
{
public:
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QTreeWidget *treeTaskInfo;
    QPlainTextEdit *ptxtedTaskInfo;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnClose;

    void setupUi(QDialog *DlgShowTaskInfo)
    {
        if (DlgShowTaskInfo->objectName().isEmpty())
            DlgShowTaskInfo->setObjectName(QStringLiteral("DlgShowTaskInfo"));
        DlgShowTaskInfo->resize(966, 789);
        verticalLayout = new QVBoxLayout(DlgShowTaskInfo);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        splitter = new QSplitter(DlgShowTaskInfo);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        treeTaskInfo = new QTreeWidget(splitter);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeTaskInfo->setHeaderItem(__qtreewidgetitem);
        treeTaskInfo->setObjectName(QStringLiteral("treeTaskInfo"));
        splitter->addWidget(treeTaskInfo);
        ptxtedTaskInfo = new QPlainTextEdit(splitter);
        ptxtedTaskInfo->setObjectName(QStringLiteral("ptxtedTaskInfo"));
        splitter->addWidget(ptxtedTaskInfo);

        verticalLayout->addWidget(splitter);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnClose = new QPushButton(DlgShowTaskInfo);
        btnClose->setObjectName(QStringLiteral("btnClose"));

        horizontalLayout->addWidget(btnClose);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(DlgShowTaskInfo);
        QObject::connect(btnClose, SIGNAL(clicked()), DlgShowTaskInfo, SLOT(accept()));

        QMetaObject::connectSlotsByName(DlgShowTaskInfo);
    } // setupUi

    void retranslateUi(QDialog *DlgShowTaskInfo)
    {
        DlgShowTaskInfo->setWindowTitle(QApplication::translate("DlgShowTaskInfo", "Dialog", 0));
        btnClose->setText(QApplication::translate("DlgShowTaskInfo", "&Close", 0));
    } // retranslateUi

};

namespace Ui {
    class DlgShowTaskInfo: public Ui_DlgShowTaskInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGSHOWTASKINFO_H
