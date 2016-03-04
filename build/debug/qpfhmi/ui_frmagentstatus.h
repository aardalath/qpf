/********************************************************************************
** Form generated from reading UI file 'frmagentstatus.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMAGENTSTATUS_H
#define UI_FRMAGENTSTATUS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FrmAgentStatus
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblTaskAgentName;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label;
    QLabel *lblTotalTasks;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLabel *lblRuningTasks;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_6;
    QLabel *lblClient;
    QLabel *label_9;
    QLabel *lblServer;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_8;
    QLabel *lblMaxTasks;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout;
    QLabel *label_7;
    QLabel *lblLoadAvg1;
    QLabel *lblLoadAvg2;
    QLabel *lblLoadAvg3;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_10;
    QLabel *lblUptime;
    QFrame *frmStatusBar;
    QHBoxLayout *horizontalLayout_4;
    QFrame *frmRunning;
    QFrame *frmWaiting;
    QFrame *frmPaused;
    QFrame *frmStopped;
    QFrame *frmFailed;
    QFrame *frmFinished;

    void setupUi(QFrame *FrmAgentStatus)
    {
        if (FrmAgentStatus->objectName().isEmpty())
            FrmAgentStatus->setObjectName(QStringLiteral("FrmAgentStatus"));
        FrmAgentStatus->resize(871, 91);
        FrmAgentStatus->setFrameShape(QFrame::Box);
        FrmAgentStatus->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(FrmAgentStatus);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lblTaskAgentName = new QLabel(FrmAgentStatus);
        lblTaskAgentName->setObjectName(QStringLiteral("lblTaskAgentName"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        lblTaskAgentName->setFont(font);

        verticalLayout->addWidget(lblTaskAgentName);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(2);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label = new QLabel(FrmAgentStatus);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_8->addWidget(label);

        lblTotalTasks = new QLabel(FrmAgentStatus);
        lblTotalTasks->setObjectName(QStringLiteral("lblTotalTasks"));
        lblTotalTasks->setFont(font);

        horizontalLayout_8->addWidget(lblTotalTasks);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout_8, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(FrmAgentStatus);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        lblRuningTasks = new QLabel(FrmAgentStatus);
        lblRuningTasks->setObjectName(QStringLiteral("lblRuningTasks"));
        lblRuningTasks->setFont(font);

        horizontalLayout_3->addWidget(lblRuningTasks);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        gridLayout->addLayout(horizontalLayout_3, 0, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_6 = new QLabel(FrmAgentStatus);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_2->addWidget(label_6);

        lblClient = new QLabel(FrmAgentStatus);
        lblClient->setObjectName(QStringLiteral("lblClient"));
        lblClient->setFont(font);

        horizontalLayout_2->addWidget(lblClient);

        label_9 = new QLabel(FrmAgentStatus);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_2->addWidget(label_9);

        lblServer = new QLabel(FrmAgentStatus);
        lblServer->setObjectName(QStringLiteral("lblServer"));
        lblServer->setFont(font);

        horizontalLayout_2->addWidget(lblServer);


        horizontalLayout_5->addLayout(horizontalLayout_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout_5, 0, 2, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_8 = new QLabel(FrmAgentStatus);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_7->addWidget(label_8);

        lblMaxTasks = new QLabel(FrmAgentStatus);
        lblMaxTasks->setObjectName(QStringLiteral("lblMaxTasks"));
        lblMaxTasks->setFont(font);

        horizontalLayout_7->addWidget(lblMaxTasks);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);


        gridLayout->addLayout(horizontalLayout_7, 1, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_7 = new QLabel(FrmAgentStatus);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout->addWidget(label_7);

        lblLoadAvg1 = new QLabel(FrmAgentStatus);
        lblLoadAvg1->setObjectName(QStringLiteral("lblLoadAvg1"));
        lblLoadAvg1->setFont(font);

        horizontalLayout->addWidget(lblLoadAvg1);

        lblLoadAvg2 = new QLabel(FrmAgentStatus);
        lblLoadAvg2->setObjectName(QStringLiteral("lblLoadAvg2"));
        lblLoadAvg2->setFont(font);

        horizontalLayout->addWidget(lblLoadAvg2);

        lblLoadAvg3 = new QLabel(FrmAgentStatus);
        lblLoadAvg3->setObjectName(QStringLiteral("lblLoadAvg3"));
        lblLoadAvg3->setFont(font);

        horizontalLayout->addWidget(lblLoadAvg3);


        horizontalLayout_6->addLayout(horizontalLayout);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_10 = new QLabel(FrmAgentStatus);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_9->addWidget(label_10);

        lblUptime = new QLabel(FrmAgentStatus);
        lblUptime->setObjectName(QStringLiteral("lblUptime"));
        lblUptime->setFont(font);

        horizontalLayout_9->addWidget(lblUptime);


        horizontalLayout_6->addLayout(horizontalLayout_9);


        gridLayout->addLayout(horizontalLayout_6, 1, 2, 1, 1);

        frmStatusBar = new QFrame(FrmAgentStatus);
        frmStatusBar->setObjectName(QStringLiteral("frmStatusBar"));
        frmStatusBar->setFrameShape(QFrame::StyledPanel);
        frmStatusBar->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frmStatusBar);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 4, 0, 4);
        frmRunning = new QFrame(frmStatusBar);
        frmRunning->setObjectName(QStringLiteral("frmRunning"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frmRunning->sizePolicy().hasHeightForWidth());
        frmRunning->setSizePolicy(sizePolicy);
        frmRunning->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 255);"));
        frmRunning->setFrameShape(QFrame::Panel);
        frmRunning->setFrameShadow(QFrame::Plain);
        frmRunning->setLineWidth(0);

        horizontalLayout_4->addWidget(frmRunning);

        frmWaiting = new QFrame(frmStatusBar);
        frmWaiting->setObjectName(QStringLiteral("frmWaiting"));
        sizePolicy.setHeightForWidth(frmWaiting->sizePolicy().hasHeightForWidth());
        frmWaiting->setSizePolicy(sizePolicy);
        frmWaiting->setStyleSheet(QStringLiteral("background-color: rgb(14, 170, 204);"));
        frmWaiting->setFrameShape(QFrame::Panel);
        frmWaiting->setFrameShadow(QFrame::Plain);
        frmWaiting->setLineWidth(0);

        horizontalLayout_4->addWidget(frmWaiting);

        frmPaused = new QFrame(frmStatusBar);
        frmPaused->setObjectName(QStringLiteral("frmPaused"));
        sizePolicy.setHeightForWidth(frmPaused->sizePolicy().hasHeightForWidth());
        frmPaused->setSizePolicy(sizePolicy);
        frmPaused->setStyleSheet(QStringLiteral("background-color: rgb(255, 254, 0);"));
        frmPaused->setFrameShape(QFrame::Panel);
        frmPaused->setFrameShadow(QFrame::Plain);
        frmPaused->setLineWidth(0);

        horizontalLayout_4->addWidget(frmPaused);

        frmStopped = new QFrame(frmStatusBar);
        frmStopped->setObjectName(QStringLiteral("frmStopped"));
        sizePolicy.setHeightForWidth(frmStopped->sizePolicy().hasHeightForWidth());
        frmStopped->setSizePolicy(sizePolicy);
        frmStopped->setStyleSheet(QStringLiteral("background-color: rgb(121, 121, 121);"));
        frmStopped->setFrameShape(QFrame::Panel);
        frmStopped->setFrameShadow(QFrame::Plain);
        frmStopped->setLineWidth(0);

        horizontalLayout_4->addWidget(frmStopped);

        frmFailed = new QFrame(frmStatusBar);
        frmFailed->setObjectName(QStringLiteral("frmFailed"));
        sizePolicy.setHeightForWidth(frmFailed->sizePolicy().hasHeightForWidth());
        frmFailed->setSizePolicy(sizePolicy);
        frmFailed->setStyleSheet(QStringLiteral("background-color: rgb(255, 0, 0);"));
        frmFailed->setFrameShape(QFrame::Panel);
        frmFailed->setFrameShadow(QFrame::Plain);
        frmFailed->setLineWidth(0);

        horizontalLayout_4->addWidget(frmFailed);

        frmFinished = new QFrame(frmStatusBar);
        frmFinished->setObjectName(QStringLiteral("frmFinished"));
        sizePolicy.setHeightForWidth(frmFinished->sizePolicy().hasHeightForWidth());
        frmFinished->setSizePolicy(sizePolicy);
        frmFinished->setStyleSheet(QStringLiteral("background-color: rgb(0, 240, 0);"));
        frmFinished->setFrameShape(QFrame::Panel);
        frmFinished->setFrameShadow(QFrame::Plain);
        frmFinished->setLineWidth(0);

        horizontalLayout_4->addWidget(frmFinished);


        gridLayout->addWidget(frmStatusBar, 1, 1, 1, 1);

        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 100);
        gridLayout->setColumnStretch(2, 1);

        verticalLayout->addLayout(gridLayout);

        lblTaskAgentName->raise();

        retranslateUi(FrmAgentStatus);

        QMetaObject::connectSlotsByName(FrmAgentStatus);
    } // setupUi

    void retranslateUi(QFrame *FrmAgentStatus)
    {
        FrmAgentStatus->setWindowTitle(QApplication::translate("FrmAgentStatus", "Task Agent Status", 0));
        lblTaskAgentName->setText(QApplication::translate("FrmAgentStatus", "Task Agent", 0));
        label->setText(QApplication::translate("FrmAgentStatus", "Total tasks:", 0));
        lblTotalTasks->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        label_2->setText(QApplication::translate("FrmAgentStatus", "Run/Wait/Pau/Sto/Fail/Fin:", 0));
        lblRuningTasks->setText(QApplication::translate("FrmAgentStatus", "0/0/0/0/0/0", 0));
        label_6->setText(QApplication::translate("FrmAgentStatus", "Client/Server:", 0));
        lblClient->setText(QApplication::translate("FrmAgentStatus", "localhost", 0));
        label_9->setText(QApplication::translate("FrmAgentStatus", "/", 0));
        lblServer->setText(QApplication::translate("FrmAgentStatus", "localhost", 0));
        label_8->setText(QApplication::translate("FrmAgentStatus", "Max.Concurrent Tasks:", 0));
        lblMaxTasks->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        label_7->setText(QApplication::translate("FrmAgentStatus", "Load Avgs.:", 0));
        lblLoadAvg1->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        lblLoadAvg2->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        lblLoadAvg3->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        label_10->setText(QApplication::translate("FrmAgentStatus", "Up time:", 0));
        lblUptime->setText(QApplication::translate("FrmAgentStatus", "0", 0));
    } // retranslateUi

};

namespace Ui {
    class FrmAgentStatus: public Ui_FrmAgentStatus {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMAGENTSTATUS_H
