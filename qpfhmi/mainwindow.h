/******************************************************************************
 * File:    mainwindow.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.qpfhmi.mainwindow
 *
 * Version: 1.0
 *
 * Date:    2015/07/01
 *
 * Copyright (C) 2015 J C Gonzalez
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declare MainWindow class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   none
 *
 * Files read / modified:
 *   none
 *
 * History:
 *   See <Changelog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFuture>
#include <QFutureWatcher>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QMdiSubWindow>
#include <QSignalMapper>

#include "hmipxy.h"
#include "datatypes.h"
#include "config.h"
#include "archivemodel.h"
#include "frmagentstatus.h"
#include "simindata.h"
#include "textview.h"
#include "statem.h"

#include "json/json.h"

namespace Ui {
class MainWindow;
}

namespace QPF {

class LogWatcher;

class MainWindow : public QMainWindow, LibComm::StateMachine
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, Configuration * cfgHdl = 0);
    ~MainWindow();

    struct Alert {
        QDateTime timeStamp;
        QString id;
        QString severity;
        QString component;
        QString description;
    };

signals:
    void goToOperational();
    void stopSendingMessages();

public slots:
    void commandSystem();

protected:
    void manualSetupUI();
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

protected slots:
    void transitToOperational();
    void processInbox();
    void selectInboxPath();
    void stopSendingMultInData();
    void sentInData(int msgsLeft);
    void endOfInDataMsgs();

    void handleFinishedHMI();
    void processPendingEvents();

    void checkForTaskRes();
    void showTaskRes();

    void initTasksMonitTree(int nCols);
    void initAlertsTable();
    void initArchiveTable();

    void updateTasksMonitTree(int nCols);
    void updateAlertsTree();

    void updateAgentsMonitPanel();

    void showTaskMonitContextMenu(const QPoint & p);
    void showAlertsContextMenu(const QPoint & p);
    void showArchiveTableContextMenu(const QPoint & p);

    void showWorkDir();
    void displayTaskInfo();

    void showArchInfo();

    void pauseTask();
    void resumeTask();
    void stopTask();
    bool runDockerCmd(QTreeWidgetItem * item, QString cmd);

    void dumpTaskInfoToTree(QString taskName, const Json::Value & v, QTreeWidget * t);
    void dumpToTree(const Json::Value & v, QTreeWidgetItem * t);

    void setDebugInfo(bool b);
    void sortTaskViewByColumn(int c);

protected:

    //----------------------------------------------------------------------
    // Method: readConfig
    //----------------------------------------------------------------------
    void readConfig();

    //----------------------------------------------------------------------
    // Method: init
    //----------------------------------------------------------------------
    void init();

    //----------------------------------------------------------------------
    // METHOD: initLogWatch
    //----------------------------------------------------------------------
    void initLogWatch();

    //----------------------------------------------------------------------
    // METHOD: setLogWatch
    //----------------------------------------------------------------------
    void setLogWatch();

private slots:
    void saveAs();
#ifndef QT_NO_CLIPBOARD
    void cut();
    void copy();
    void paste();
#endif
    void about();
    void updateMenus();
    void updateWindowMenu();
    TextView *createTextView();
    void switchLayoutDirection();
    void setActiveSubWindow(QWidget *window);

    void showConfigTool();
    void showDBBrowser();
    void showExtToolsDef();
    void showVerbLevel();
    void execTestRun();

    void restart();
    void quitApp();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    TextView *activeTextView();
    QMdiSubWindow *findTextView(const QString &fileName);

    virtual void defineValidTransitions();
    void showState();

    QSignalMapper *windowMapper;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *windowMenu;
    QMenu *toolsMenu;
    QMenu *sessionInfoMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *saveAsAct;
    QAction *restartAct;
    QAction *quitAct;
#ifndef QT_NO_CLIPBOARD
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
#endif

    QAction *configToolAct;
    QAction *browseDBAct;
    QAction *extToolsAct;
    QAction *verbosityAct;
    QAction *execTestRunAct;

    QAction *dbgInfoAct;

    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

private:
    void clearLayout(QLayout *layout);

private:
    Ui::MainWindow *ui;

    Configuration * cfg;

    QVector<LogWatcher*> nodeLogs;

    HMIProxy * hmiNode;
    std::thread hmiPxyThread;

    SimInData * simInData;
    ArchiveModel * archHdl;
    QString  fileInDataParams;
    QTimer * taskMonitTimer;

    QString inboxDirName;

    QMap<QString, Json::Value> taskResInfo;
    QMap<QString, QTreeWidgetItem *> taskResItems;
    QMap<QString, Json::Value> processedTasksInfo;
    QList<Alert> alerts;

    QAction * acWorkDir;
    QAction * acShowTaskInfo;
    QAction * acPauseTask;
    QAction * acResumeTask;
    QAction * acStopTask;

    QAction * acAckAlert;

    QAction * acArchiveShow;
    QMenu *   acArchiveOpenExt;
    QList<QAction *> acArchiveOpenExtTools;

    QVBoxLayout * vlyFrmAgents;
    QSpacerItem * spacerFrmAgents;

    std::map<std::string, TaskAgentInfo*> taskAgentsInfo;
    std::map<std::string, FrmAgentStatus*> taskAgentsInfoPanel;

public:
    static int const EXIT_CODE_RESTART;
};

}

#endif // MAINWINDOW_H
