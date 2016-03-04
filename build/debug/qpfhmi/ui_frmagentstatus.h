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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FrmAgentStatus
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblTaskAgentName;
    QHBoxLayout *horizontalLayout_11;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label;
    QLabel *lblTotalTasks;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_8;
    QLabel *lblMaxTasks;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_6;
    QLabel *lblClient;
    QLabel *label_9;
    QLabel *lblServer;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_7;
    QLabel *lblLoadAvg1;
    QLabel *lblLoadAvg2;
    QLabel *lblLoadAvg3;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_10;
    QLabel *lblUptime;
    QHBoxLayout *horizontalLayout_10;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLabel *lblRuningTasks;
    QProgressBar *pbPercRunning;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLabel *lblWaitingTasks;
    QProgressBar *pbPercWaiting;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QLabel *lblFailedTasks;
    QProgressBar *pbPercFailed;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLabel *lblFinishedTasks;
    QProgressBar *pbPercFinished;

    void setupUi(QFrame *FrmAgentStatus)
    {
        if (FrmAgentStatus->objectName().isEmpty())
            FrmAgentStatus->setObjectName(QStringLiteral("FrmAgentStatus"));
        FrmAgentStatus->resize(888, 80);
        FrmAgentStatus->setFrameShape(QFrame::Box);
        FrmAgentStatus->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(FrmAgentStatus);
        verticalLayout->setSpacing(2);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(4, 4, 4, 4);
        lblTaskAgentName = new QLabel(FrmAgentStatus);
        lblTaskAgentName->setObjectName(QStringLiteral("lblTaskAgentName"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        lblTaskAgentName->setFont(font);

        verticalLayout->addWidget(lblTaskAgentName);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label = new QLabel(FrmAgentStatus);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_8->addWidget(label);

        lblTotalTasks = new QLabel(FrmAgentStatus);
        lblTotalTasks->setObjectName(QStringLiteral("lblTotalTasks"));
        lblTotalTasks->setFont(font);

        horizontalLayout_8->addWidget(lblTotalTasks);


        horizontalLayout_11->addLayout(horizontalLayout_8);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_8 = new QLabel(FrmAgentStatus);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_7->addWidget(label_8);

        lblMaxTasks = new QLabel(FrmAgentStatus);
        lblMaxTasks->setObjectName(QStringLiteral("lblMaxTasks"));
        lblMaxTasks->setFont(font);

        horizontalLayout_7->addWidget(lblMaxTasks);


        horizontalLayout_11->addLayout(horizontalLayout_7);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer);

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


        horizontalLayout_11->addLayout(horizontalLayout_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_2);

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


        horizontalLayout_11->addLayout(horizontalLayout);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_3);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_10 = new QLabel(FrmAgentStatus);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_9->addWidget(label_10);

        lblUptime = new QLabel(FrmAgentStatus);
        lblUptime->setObjectName(QStringLiteral("lblUptime"));
        lblUptime->setFont(font);

        horizontalLayout_9->addWidget(lblUptime);


        horizontalLayout_11->addLayout(horizontalLayout_9);

        horizontalLayout_11->setStretch(0, 2);
        horizontalLayout_11->setStretch(1, 1);
        horizontalLayout_11->setStretch(2, 3);
        horizontalLayout_11->setStretch(3, 1);
        horizontalLayout_11->setStretch(4, 4);
        horizontalLayout_11->setStretch(5, 1);
        horizontalLayout_11->setStretch(6, 2);
        horizontalLayout_11->setStretch(7, 1);
        horizontalLayout_11->setStretch(8, 2);

        verticalLayout->addLayout(horizontalLayout_11);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(FrmAgentStatus);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        lblRuningTasks = new QLabel(FrmAgentStatus);
        lblRuningTasks->setObjectName(QStringLiteral("lblRuningTasks"));
        lblRuningTasks->setFont(font);

        horizontalLayout_3->addWidget(lblRuningTasks);

        pbPercRunning = new QProgressBar(FrmAgentStatus);
        pbPercRunning->setObjectName(QStringLiteral("pbPercRunning"));
        pbPercRunning->setValue(24);

        horizontalLayout_3->addWidget(pbPercRunning);


        horizontalLayout_10->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(FrmAgentStatus);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        lblWaitingTasks = new QLabel(FrmAgentStatus);
        lblWaitingTasks->setObjectName(QStringLiteral("lblWaitingTasks"));
        lblWaitingTasks->setFont(font);

        horizontalLayout_4->addWidget(lblWaitingTasks);

        pbPercWaiting = new QProgressBar(FrmAgentStatus);
        pbPercWaiting->setObjectName(QStringLiteral("pbPercWaiting"));
        pbPercWaiting->setValue(24);

        horizontalLayout_4->addWidget(pbPercWaiting);


        horizontalLayout_10->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_4 = new QLabel(FrmAgentStatus);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_5->addWidget(label_4);

        lblFailedTasks = new QLabel(FrmAgentStatus);
        lblFailedTasks->setObjectName(QStringLiteral("lblFailedTasks"));
        lblFailedTasks->setFont(font);

        horizontalLayout_5->addWidget(lblFailedTasks);

        pbPercFailed = new QProgressBar(FrmAgentStatus);
        pbPercFailed->setObjectName(QStringLiteral("pbPercFailed"));
        pbPercFailed->setValue(24);

        horizontalLayout_5->addWidget(pbPercFailed);


        horizontalLayout_10->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_5 = new QLabel(FrmAgentStatus);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_6->addWidget(label_5);

        lblFinishedTasks = new QLabel(FrmAgentStatus);
        lblFinishedTasks->setObjectName(QStringLiteral("lblFinishedTasks"));
        lblFinishedTasks->setFont(font);

        horizontalLayout_6->addWidget(lblFinishedTasks);

        pbPercFinished = new QProgressBar(FrmAgentStatus);
        pbPercFinished->setObjectName(QStringLiteral("pbPercFinished"));
        pbPercFinished->setValue(24);

        horizontalLayout_6->addWidget(pbPercFinished);


        horizontalLayout_10->addLayout(horizontalLayout_6);


        verticalLayout->addLayout(horizontalLayout_10);


        retranslateUi(FrmAgentStatus);

        QMetaObject::connectSlotsByName(FrmAgentStatus);
    } // setupUi

    void retranslateUi(QFrame *FrmAgentStatus)
    {
        FrmAgentStatus->setWindowTitle(QApplication::translate("FrmAgentStatus", "Task Agent Status", 0));
        lblTaskAgentName->setText(QApplication::translate("FrmAgentStatus", "Task Agent", 0));
        label->setText(QApplication::translate("FrmAgentStatus", "Total tasks:", 0));
        lblTotalTasks->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        label_8->setText(QApplication::translate("FrmAgentStatus", "Max.Concurrent Tasks:", 0));
        lblMaxTasks->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        label_6->setText(QApplication::translate("FrmAgentStatus", "Client/Server:", 0));
        lblClient->setText(QApplication::translate("FrmAgentStatus", "localhost", 0));
        label_9->setText(QApplication::translate("FrmAgentStatus", "/", 0));
        lblServer->setText(QApplication::translate("FrmAgentStatus", "localhost", 0));
        label_7->setText(QApplication::translate("FrmAgentStatus", "Load Avgs.:", 0));
        lblLoadAvg1->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        lblLoadAvg2->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        lblLoadAvg3->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        label_10->setText(QApplication::translate("FrmAgentStatus", "Up time:", 0));
        lblUptime->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        label_2->setText(QApplication::translate("FrmAgentStatus", "Running:", 0));
        lblRuningTasks->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        label_3->setText(QApplication::translate("FrmAgentStatus", "Waiting:", 0));
        lblWaitingTasks->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        label_4->setText(QApplication::translate("FrmAgentStatus", "Failed:", 0));
        lblFailedTasks->setText(QApplication::translate("FrmAgentStatus", "0", 0));
        label_5->setText(QApplication::translate("FrmAgentStatus", "Finished:", 0));
        lblFinishedTasks->setText(QApplication::translate("FrmAgentStatus", "0", 0));
    } // retranslateUi

};

namespace Ui {
    class FrmAgentStatus: public Ui_FrmAgentStatus {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMAGENTSTATUS_H
