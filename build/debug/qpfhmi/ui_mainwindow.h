/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionActivate_Debug_Info;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_11;
    QTabWidget *tabMainWgd;
    QWidget *tabpgLog;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabLogs;
    QWidget *tabpgEvtInj;
    QVBoxLayout *verticalLayout_6;
    QTabWidget *tabEventsToInject;
    QWidget *tabpgSingleInData;
    QVBoxLayout *verticalLayout_5;
    QFormLayout *formLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_8;
    QDateTimeEdit *dtStart;
    QSpacerItem *horizontalSpacer_9;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_9;
    QDateTimeEdit *dtEnd;
    QSpacerItem *horizontalSpacer_10;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *cboxProdType;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_6;
    QLineEdit *edProdId;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *edProdVer;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_10;
    QHBoxLayout *horizontalLayout_6;
    QComboBox *cboxProdStatus;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_11;
    QLineEdit *edURL;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *btnSendInDataEvt;
    QWidget *tabpgMultnData;
    QVBoxLayout *verticalLayout_14;
    QHBoxLayout *horizontalLayout_13;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout_2;
    QLabel *label_7;
    QDateTimeEdit *dtStartMult;
    QLabel *label_25;
    QDateTimeEdit *dtEndMult;
    QLabel *label_15;
    QHBoxLayout *horizontalLayout_11;
    QComboBox *cboxStep;
    QLabel *label_16;
    QSpacerItem *horizontalSpacer_12;
    QLabel *label_12;
    QHBoxLayout *horizontalLayout_18;
    QComboBox *cboxFreq;
    QLabel *label_21;
    QSpacerItem *horizontalSpacer_19;
    QLabel *label_17;
    QHBoxLayout *horizontalLayout_3;
    QListWidget *lstProductStatus;
    QSpacerItem *horizontalSpacer_16;
    QSpacerItem *verticalSpacer_5;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_10;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_14;
    QSpacerItem *verticalSpacer_7;
    QHBoxLayout *horizontalLayout_14;
    QListWidget *lstProductTypes;
    QSpacerItem *horizontalSpacer_15;
    QSpacerItem *verticalSpacer_8;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_17;
    QSpacerItem *horizontalSpacer_18;
    QPushButton *btnSendMultInDataEvt;
    QWidget *tabpgInDataFromFile;
    QVBoxLayout *verticalLayout_9;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_22;
    QLineEdit *edInDataParamFile;
    QToolButton *btnSelectInDataParamFile;
    QHBoxLayout *horizontalLayout_19;
    QSpacerItem *horizontalSpacer_11;
    QLabel *label_23;
    QSpacerItem *horizontalSpacer_13;
    QSpacerItem *verticalSpacer_6;
    QHBoxLayout *horizontalLayout_20;
    QSpacerItem *horizontalSpacer_20;
    QPushButton *btnSendMultInDataEvtFile;
    QWidget *tabpgMonit;
    QVBoxLayout *verticalLayout_12;
    QTreeWidget *treeTaskMonit;
    QWidget *tabAgents;
    QVBoxLayout *vlyAgents;
    QScrollArea *scrollAreaAgents;
    QWidget *scrollAreaWidgetContents;
    QWidget *tabpgArchive;
    QVBoxLayout *verticalLayout_13;
    QTableView *tblvwArchive;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTools;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockMainControl;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnStart;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btnStopMultInDataEvt;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *chkDebugInfo;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1112, 612);
        QFont font;
        font.setPointSize(10);
        MainWindow->setFont(font);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionActivate_Debug_Info = new QAction(MainWindow);
        actionActivate_Debug_Info->setObjectName(QStringLiteral("actionActivate_Debug_Info"));
        actionActivate_Debug_Info->setCheckable(true);
        actionActivate_Debug_Info->setChecked(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_11 = new QVBoxLayout(centralWidget);
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setContentsMargins(11, 11, 11, 11);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        tabMainWgd = new QTabWidget(centralWidget);
        tabMainWgd->setObjectName(QStringLiteral("tabMainWgd"));
        tabMainWgd->setMovable(true);
        tabpgLog = new QWidget();
        tabpgLog->setObjectName(QStringLiteral("tabpgLog"));
        verticalLayout_3 = new QVBoxLayout(tabpgLog);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        tabLogs = new QTabWidget(tabpgLog);
        tabLogs->setObjectName(QStringLiteral("tabLogs"));
        tabLogs->setTabPosition(QTabWidget::South);
        tabLogs->setTabShape(QTabWidget::Triangular);
        tabLogs->setMovable(true);

        verticalLayout_3->addWidget(tabLogs);

        tabMainWgd->addTab(tabpgLog, QString());
        tabpgEvtInj = new QWidget();
        tabpgEvtInj->setObjectName(QStringLiteral("tabpgEvtInj"));
        verticalLayout_6 = new QVBoxLayout(tabpgEvtInj);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        tabEventsToInject = new QTabWidget(tabpgEvtInj);
        tabEventsToInject->setObjectName(QStringLiteral("tabEventsToInject"));
        tabEventsToInject->setToolTipDuration(-1);
        tabpgSingleInData = new QWidget();
        tabpgSingleInData->setObjectName(QStringLiteral("tabpgSingleInData"));
        verticalLayout_5 = new QVBoxLayout(tabpgSingleInData);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label = new QLabel(tabpgSingleInData);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        dtStart = new QDateTimeEdit(tabpgSingleInData);
        dtStart->setObjectName(QStringLiteral("dtStart"));
        dtStart->setCalendarPopup(true);

        horizontalLayout_8->addWidget(dtStart);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_9);

        horizontalLayout_8->setStretch(0, 2);
        horizontalLayout_8->setStretch(1, 3);

        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout_8);

        label_2 = new QLabel(tabpgSingleInData);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        dtEnd = new QDateTimeEdit(tabpgSingleInData);
        dtEnd->setObjectName(QStringLiteral("dtEnd"));
        dtEnd->setCalendarPopup(true);

        horizontalLayout_9->addWidget(dtEnd);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_10);

        horizontalLayout_9->setStretch(0, 2);
        horizontalLayout_9->setStretch(1, 3);

        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout_9);

        label_5 = new QLabel(tabpgSingleInData);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        cboxProdType = new QComboBox(tabpgSingleInData);
        cboxProdType->setObjectName(QStringLiteral("cboxProdType"));

        horizontalLayout_4->addWidget(cboxProdType);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);


        formLayout->setLayout(2, QFormLayout::FieldRole, horizontalLayout_4);

        label_6 = new QLabel(tabpgSingleInData);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_6);

        edProdId = new QLineEdit(tabpgSingleInData);
        edProdId->setObjectName(QStringLiteral("edProdId"));

        formLayout->setWidget(3, QFormLayout::FieldRole, edProdId);

        label_8 = new QLabel(tabpgSingleInData);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_8);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        edProdVer = new QLineEdit(tabpgSingleInData);
        edProdVer->setObjectName(QStringLiteral("edProdVer"));

        horizontalLayout_5->addWidget(edProdVer);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_6);

        horizontalLayout_5->setStretch(1, 5);

        formLayout->setLayout(4, QFormLayout::FieldRole, horizontalLayout_5);

        label_10 = new QLabel(tabpgSingleInData);
        label_10->setObjectName(QStringLiteral("label_10"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_10);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        cboxProdStatus = new QComboBox(tabpgSingleInData);
        cboxProdStatus->setObjectName(QStringLiteral("cboxProdStatus"));

        horizontalLayout_6->addWidget(cboxProdStatus);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);


        formLayout->setLayout(5, QFormLayout::FieldRole, horizontalLayout_6);

        label_11 = new QLabel(tabpgSingleInData);
        label_11->setObjectName(QStringLiteral("label_11"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_11);

        edURL = new QLineEdit(tabpgSingleInData);
        edURL->setObjectName(QStringLiteral("edURL"));

        formLayout->setWidget(6, QFormLayout::FieldRole, edURL);


        verticalLayout_5->addLayout(formLayout);

        verticalSpacer_2 = new QSpacerItem(20, 111, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_8);

        btnSendInDataEvt = new QPushButton(tabpgSingleInData);
        btnSendInDataEvt->setObjectName(QStringLiteral("btnSendInDataEvt"));

        horizontalLayout_7->addWidget(btnSendInDataEvt);


        verticalLayout_5->addLayout(horizontalLayout_7);

        tabEventsToInject->addTab(tabpgSingleInData, QString());
        tabpgMultnData = new QWidget();
        tabpgMultnData->setObjectName(QStringLiteral("tabpgMultnData"));
        verticalLayout_14 = new QVBoxLayout(tabpgMultnData);
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setContentsMargins(11, 11, 11, 11);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setSpacing(6);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        label_7 = new QLabel(tabpgMultnData);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_7);

        dtStartMult = new QDateTimeEdit(tabpgMultnData);
        dtStartMult->setObjectName(QStringLiteral("dtStartMult"));
        dtStartMult->setCalendarPopup(true);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, dtStartMult);

        label_25 = new QLabel(tabpgMultnData);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_25);

        dtEndMult = new QDateTimeEdit(tabpgMultnData);
        dtEndMult->setObjectName(QStringLiteral("dtEndMult"));
        dtEndMult->setCalendarPopup(true);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, dtEndMult);

        label_15 = new QLabel(tabpgMultnData);
        label_15->setObjectName(QStringLiteral("label_15"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_15);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        cboxStep = new QComboBox(tabpgMultnData);
        cboxStep->setObjectName(QStringLiteral("cboxStep"));

        horizontalLayout_11->addWidget(cboxStep);

        label_16 = new QLabel(tabpgMultnData);
        label_16->setObjectName(QStringLiteral("label_16"));

        horizontalLayout_11->addWidget(label_16);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_12);

        horizontalLayout_11->setStretch(2, 3);

        formLayout_2->setLayout(2, QFormLayout::FieldRole, horizontalLayout_11);

        label_12 = new QLabel(tabpgMultnData);
        label_12->setObjectName(QStringLiteral("label_12"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_12);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        cboxFreq = new QComboBox(tabpgMultnData);
        cboxFreq->setObjectName(QStringLiteral("cboxFreq"));

        horizontalLayout_18->addWidget(cboxFreq);

        label_21 = new QLabel(tabpgMultnData);
        label_21->setObjectName(QStringLiteral("label_21"));

        horizontalLayout_18->addWidget(label_21);

        horizontalSpacer_19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_18->addItem(horizontalSpacer_19);


        formLayout_2->setLayout(3, QFormLayout::FieldRole, horizontalLayout_18);

        label_17 = new QLabel(tabpgMultnData);
        label_17->setObjectName(QStringLiteral("label_17"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label_17);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lstProductStatus = new QListWidget(tabpgMultnData);
        new QListWidgetItem(lstProductStatus);
        new QListWidgetItem(lstProductStatus);
        lstProductStatus->setObjectName(QStringLiteral("lstProductStatus"));
        lstProductStatus->setSelectionMode(QAbstractItemView::MultiSelection);

        horizontalLayout_3->addWidget(lstProductStatus);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_16);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 10);

        formLayout_2->setLayout(4, QFormLayout::FieldRole, horizontalLayout_3);


        verticalLayout->addLayout(formLayout_2);

        verticalSpacer_5 = new QSpacerItem(20, 94, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_5);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 10);

        horizontalLayout_13->addLayout(verticalLayout);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        label_14 = new QLabel(tabpgMultnData);
        label_14->setObjectName(QStringLiteral("label_14"));

        verticalLayout_8->addWidget(label_14);

        verticalSpacer_7 = new QSpacerItem(17, 48, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_7);


        horizontalLayout_10->addLayout(verticalLayout_8);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        lstProductTypes = new QListWidget(tabpgMultnData);
        new QListWidgetItem(lstProductTypes);
        new QListWidgetItem(lstProductTypes);
        new QListWidgetItem(lstProductTypes);
        new QListWidgetItem(lstProductTypes);
        new QListWidgetItem(lstProductTypes);
        lstProductTypes->setObjectName(QStringLiteral("lstProductTypes"));
        lstProductTypes->setSelectionMode(QAbstractItemView::MultiSelection);

        horizontalLayout_14->addWidget(lstProductTypes);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_15);

        horizontalLayout_14->setStretch(0, 3);
        horizontalLayout_14->setStretch(1, 2);

        horizontalLayout_10->addLayout(horizontalLayout_14);


        verticalLayout_10->addLayout(horizontalLayout_10);

        verticalSpacer_8 = new QSpacerItem(20, 94, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_10->addItem(verticalSpacer_8);

        verticalLayout_10->setStretch(0, 3);
        verticalLayout_10->setStretch(1, 2);

        horizontalLayout_13->addLayout(verticalLayout_10);

        horizontalLayout_13->setStretch(0, 6);
        horizontalLayout_13->setStretch(1, 4);

        verticalLayout_14->addLayout(horizontalLayout_13);

        verticalSpacer_3 = new QSpacerItem(20, 33, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_14->addItem(verticalSpacer_3);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_18);

        btnSendMultInDataEvt = new QPushButton(tabpgMultnData);
        btnSendMultInDataEvt->setObjectName(QStringLiteral("btnSendMultInDataEvt"));

        horizontalLayout_17->addWidget(btnSendMultInDataEvt);


        verticalLayout_14->addLayout(horizontalLayout_17);

        tabEventsToInject->addTab(tabpgMultnData, QString());
        tabpgInDataFromFile = new QWidget();
        tabpgInDataFromFile->setObjectName(QStringLiteral("tabpgInDataFromFile"));
        verticalLayout_9 = new QVBoxLayout(tabpgInDataFromFile);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_22 = new QLabel(tabpgInDataFromFile);
        label_22->setObjectName(QStringLiteral("label_22"));

        horizontalLayout_12->addWidget(label_22);

        edInDataParamFile = new QLineEdit(tabpgInDataFromFile);
        edInDataParamFile->setObjectName(QStringLiteral("edInDataParamFile"));

        horizontalLayout_12->addWidget(edInDataParamFile);

        btnSelectInDataParamFile = new QToolButton(tabpgInDataFromFile);
        btnSelectInDataParamFile->setObjectName(QStringLiteral("btnSelectInDataParamFile"));

        horizontalLayout_12->addWidget(btnSelectInDataParamFile);


        verticalLayout_7->addLayout(horizontalLayout_12);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_19->addItem(horizontalSpacer_11);

        label_23 = new QLabel(tabpgInDataFromFile);
        label_23->setObjectName(QStringLiteral("label_23"));
        QFont font1;
        font1.setItalic(true);
        label_23->setFont(font1);
        label_23->setWordWrap(true);

        horizontalLayout_19->addWidget(label_23);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_19->addItem(horizontalSpacer_13);


        verticalLayout_7->addLayout(horizontalLayout_19);


        verticalLayout_9->addLayout(verticalLayout_7);

        verticalSpacer_6 = new QSpacerItem(20, 255, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_6);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_20->addItem(horizontalSpacer_20);

        btnSendMultInDataEvtFile = new QPushButton(tabpgInDataFromFile);
        btnSendMultInDataEvtFile->setObjectName(QStringLiteral("btnSendMultInDataEvtFile"));

        horizontalLayout_20->addWidget(btnSendMultInDataEvtFile);


        verticalLayout_9->addLayout(horizontalLayout_20);

        tabEventsToInject->addTab(tabpgInDataFromFile, QString());

        verticalLayout_6->addWidget(tabEventsToInject);

        tabMainWgd->addTab(tabpgEvtInj, QString());
        tabpgMonit = new QWidget();
        tabpgMonit->setObjectName(QStringLiteral("tabpgMonit"));
        verticalLayout_12 = new QVBoxLayout(tabpgMonit);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        treeTaskMonit = new QTreeWidget(tabpgMonit);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeTaskMonit->setHeaderItem(__qtreewidgetitem);
        treeTaskMonit->setObjectName(QStringLiteral("treeTaskMonit"));

        verticalLayout_12->addWidget(treeTaskMonit);

        tabMainWgd->addTab(tabpgMonit, QString());
        tabAgents = new QWidget();
        tabAgents->setObjectName(QStringLiteral("tabAgents"));
        vlyAgents = new QVBoxLayout(tabAgents);
        vlyAgents->setSpacing(6);
        vlyAgents->setContentsMargins(11, 11, 11, 11);
        vlyAgents->setObjectName(QStringLiteral("vlyAgents"));
        scrollAreaAgents = new QScrollArea(tabAgents);
        scrollAreaAgents->setObjectName(QStringLiteral("scrollAreaAgents"));
        scrollAreaAgents->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 843, 484));
        scrollAreaAgents->setWidget(scrollAreaWidgetContents);

        vlyAgents->addWidget(scrollAreaAgents);

        tabMainWgd->addTab(tabAgents, QString());
        tabpgArchive = new QWidget();
        tabpgArchive->setObjectName(QStringLiteral("tabpgArchive"));
        verticalLayout_13 = new QVBoxLayout(tabpgArchive);
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setContentsMargins(11, 11, 11, 11);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        tblvwArchive = new QTableView(tabpgArchive);
        tblvwArchive->setObjectName(QStringLiteral("tblvwArchive"));

        verticalLayout_13->addWidget(tblvwArchive);

        tabMainWgd->addTab(tabpgArchive, QString());

        verticalLayout_11->addWidget(tabMainWgd);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1112, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QStringLiteral("menuTools"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockMainControl = new QDockWidget(MainWindow);
        dockMainControl->setObjectName(QStringLiteral("dockMainControl"));
        dockMainControl->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockMainControl->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnStart = new QPushButton(dockWidgetContents);
        btnStart->setObjectName(QStringLiteral("btnStart"));
        btnStart->setMinimumSize(QSize(100, 40));
        btnStart->setCheckable(false);

        horizontalLayout->addWidget(btnStart);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 4);
        horizontalLayout->setStretch(2, 1);

        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        btnStopMultInDataEvt = new QPushButton(dockWidgetContents);
        btnStopMultInDataEvt->setObjectName(QStringLiteral("btnStopMultInDataEvt"));

        horizontalLayout_2->addWidget(btnStopMultInDataEvt);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(2, 1);

        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 463, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        chkDebugInfo = new QCheckBox(groupBox);
        chkDebugInfo->setObjectName(QStringLiteral("chkDebugInfo"));
        chkDebugInfo->setChecked(true);

        verticalLayout_4->addWidget(chkDebugInfo);


        verticalLayout_2->addWidget(groupBox);

        dockMainControl->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockMainControl);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuFile->addAction(actionQuit);
        menuTools->addAction(actionActivate_Debug_Info);

        retranslateUi(MainWindow);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(dtStartMult, SIGNAL(dateTimeChanged(QDateTime)), dtEndMult, SLOT(setDateTime(QDateTime)));
        QObject::connect(actionActivate_Debug_Info, SIGNAL(toggled(bool)), MainWindow, SLOT(setDebugInfo(bool)));
        QObject::connect(actionActivate_Debug_Info, SIGNAL(toggled(bool)), chkDebugInfo, SLOT(setChecked(bool)));
        QObject::connect(chkDebugInfo, SIGNAL(toggled(bool)), actionActivate_Debug_Info, SLOT(setChecked(bool)));

        tabMainWgd->setCurrentIndex(0);
        tabEventsToInject->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QPF HMI", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0));
        actionActivate_Debug_Info->setText(QApplication::translate("MainWindow", "Activate Debug Info", 0));
        tabMainWgd->setTabText(tabMainWgd->indexOf(tabpgLog), QApplication::translate("MainWindow", "Log Messages", 0));
        label->setText(QApplication::translate("MainWindow", "Start Time:", 0));
        dtStart->setDisplayFormat(QApplication::translate("MainWindow", "yyyyMMddTHHmmss", 0));
        label_2->setText(QApplication::translate("MainWindow", "End Time:", 0));
        dtEnd->setDisplayFormat(QApplication::translate("MainWindow", "yyyyMMddTHHmmss", 0));
        label_5->setText(QApplication::translate("MainWindow", "Product Type:", 0));
        cboxProdType->clear();
        cboxProdType->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "CALIB_1", 0)
         << QApplication::translate("MainWindow", "CALIB_2", 0)
         << QApplication::translate("MainWindow", "PROD_1", 0)
         << QApplication::translate("MainWindow", "PROD_2", 0)
         << QApplication::translate("MainWindow", "PROD_3", 0)
        );
        label_6->setText(QApplication::translate("MainWindow", "Product Id.:", 0));
        label_8->setText(QApplication::translate("MainWindow", "Product Version:", 0));
        label_10->setText(QApplication::translate("MainWindow", "Product Status:", 0));
        cboxProdStatus->clear();
        cboxProdStatus->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "OK", 0)
         << QApplication::translate("MainWindow", "NOK", 0)
        );
        label_11->setText(QApplication::translate("MainWindow", "URL:", 0));
        btnSendInDataEvt->setText(QApplication::translate("MainWindow", "Send INDATA event", 0));
        tabEventsToInject->setTabText(tabEventsToInject->indexOf(tabpgSingleInData), QApplication::translate("MainWindow", "Single Incoming Data Message", 0));
        label_7->setText(QApplication::translate("MainWindow", "Start Time: from", 0));
        dtStartMult->setDisplayFormat(QApplication::translate("MainWindow", "yyyyMMddTHHmmss", 0));
        label_25->setText(QApplication::translate("MainWindow", "to", 0));
        dtEndMult->setDisplayFormat(QApplication::translate("MainWindow", "yyyyMMddTHHmmss", 0));
        label_15->setText(QApplication::translate("MainWindow", "every", 0));
        cboxStep->clear();
        cboxStep->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1", 0)
         << QApplication::translate("MainWindow", "6", 0)
         << QApplication::translate("MainWindow", "12", 0)
         << QApplication::translate("MainWindow", "24", 0)
        );
        label_16->setText(QApplication::translate("MainWindow", "hours", 0));
        label_12->setText(QApplication::translate("MainWindow", "Frequency of arrival", 0));
        cboxFreq->clear();
        cboxFreq->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1", 0)
         << QApplication::translate("MainWindow", "5", 0)
         << QApplication::translate("MainWindow", "10", 0)
         << QApplication::translate("MainWindow", "30", 0)
         << QApplication::translate("MainWindow", "60", 0)
        );
        label_21->setText(QApplication::translate("MainWindow", "seconds", 0));
        label_17->setText(QApplication::translate("MainWindow", "Product Status:", 0));

        const bool __sortingEnabled = lstProductStatus->isSortingEnabled();
        lstProductStatus->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = lstProductStatus->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("MainWindow", "OK", 0));
        QListWidgetItem *___qlistwidgetitem1 = lstProductStatus->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("MainWindow", "NOK", 0));
        lstProductStatus->setSortingEnabled(__sortingEnabled);

        label_14->setText(QApplication::translate("MainWindow", "Product Type:", 0));

        const bool __sortingEnabled1 = lstProductTypes->isSortingEnabled();
        lstProductTypes->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem2 = lstProductTypes->item(0);
        ___qlistwidgetitem2->setText(QApplication::translate("MainWindow", "CALIB_1", 0));
        QListWidgetItem *___qlistwidgetitem3 = lstProductTypes->item(1);
        ___qlistwidgetitem3->setText(QApplication::translate("MainWindow", "CALIB_2", 0));
        QListWidgetItem *___qlistwidgetitem4 = lstProductTypes->item(2);
        ___qlistwidgetitem4->setText(QApplication::translate("MainWindow", "PROD_1", 0));
        QListWidgetItem *___qlistwidgetitem5 = lstProductTypes->item(3);
        ___qlistwidgetitem5->setText(QApplication::translate("MainWindow", "PROD_2", 0));
        QListWidgetItem *___qlistwidgetitem6 = lstProductTypes->item(4);
        ___qlistwidgetitem6->setText(QApplication::translate("MainWindow", "PROD_3", 0));
        lstProductTypes->setSortingEnabled(__sortingEnabled1);

        btnSendMultInDataEvt->setText(QApplication::translate("MainWindow", "Start sending of INDATA messages", 0));
        tabEventsToInject->setTabText(tabEventsToInject->indexOf(tabpgMultnData), QApplication::translate("MainWindow", "Multiple Incoming Data Messages", 0));
        label_22->setText(QApplication::translate("MainWindow", "Select Input File:", 0));
        btnSelectInDataParamFile->setText(QApplication::translate("MainWindow", "...", 0));
        label_23->setText(QApplication::translate("MainWindow", "The format of the input file is expected to be a set of rows of ASCII text with fields separated by blanks, with the following meaning:\n"
" - 1st field: <secondsFromStart>\n"
" - 2nd field: <startDate>\n"
" - 3rd field: <endDate>\n"
" - 4th field: <productType>\n"
" - 5th field: <productStatus>", 0));
        btnSendMultInDataEvtFile->setText(QApplication::translate("MainWindow", "Start sending of INDATA messages", 0));
        tabEventsToInject->setTabText(tabEventsToInject->indexOf(tabpgInDataFromFile), QApplication::translate("MainWindow", "Take In. Data Parameters from file", 0));
        tabMainWgd->setTabText(tabMainWgd->indexOf(tabpgEvtInj), QApplication::translate("MainWindow", "Event Injection", 0));
        tabMainWgd->setTabText(tabMainWgd->indexOf(tabpgMonit), QApplication::translate("MainWindow", "Monitoring", 0));
        tabMainWgd->setTabText(tabMainWgd->indexOf(tabAgents), QApplication::translate("MainWindow", "Agents Status", 0));
        tabMainWgd->setTabText(tabMainWgd->indexOf(tabpgArchive), QApplication::translate("MainWindow", "Archive", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuTools->setTitle(QApplication::translate("MainWindow", "Tools", 0));
        dockMainControl->setWindowTitle(QApplication::translate("MainWindow", "Main Control", 0));
        btnStart->setText(QApplication::translate("MainWindow", "START SYSTEM", 0));
        btnStopMultInDataEvt->setText(QApplication::translate("MainWindow", "Stop sending\n"
"INDATA messages", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Setup", 0));
        chkDebugInfo->setText(QApplication::translate("MainWindow", "Debug Info", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
