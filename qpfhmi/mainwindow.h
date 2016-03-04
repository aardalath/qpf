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

#include <future>

#include "hmipxy.h"
#include "msgtypes.h"
#include "config.h"
#include "archivemodel.h"
#include "frmagentstatus.h"
#include "simindata.h"
#include "textview.h"

//INI#include "iniparser.h"
#include "json/json.h"

namespace Ui {
class MainWindow;
}

namespace QPF {

class LogWatcher;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, char * cfgFile = 0);
    ~MainWindow();

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
    void sendInData();
    void prepareSendMultInData();
    void prepareSendInDataFromFile();
    void selectInDataParamsFile();
    void stopSendingMultInData();
    void sentInData(int msgsLeft);
    void endOfInDataMsgs();

    void handleFinishedHMI();
    void processPendingEvents();

    void checkForTaskRes();
    void showTaskRes();

    void updateTasksMonitTree(int nCols);
    void initTasksMonitTree(int nCols);
    void updateAgentsMonitPanel();

    void showTaskMonitContextMenu(const QPoint & p);
    void showWorkDir();
    void displayTaskInfo();

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

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    TextView *activeTextView();
    QMdiSubWindow *findTextView(const QString &fileName);

    QSignalMapper *windowMapper;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *saveAsAct;
    QAction *exitAct;
#ifndef QT_NO_CLIPBOARD
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
#endif
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
    QVector<double> getLoadAvgs();
    void clearLayout(QLayout *layout);

private:
    Ui::MainWindow *ui;

    char * configFile;
    QVector<LogWatcher*> nodeLogs;

    HMIProxy * hmiNode;
    //std::future<int> hmiResult;
    std::thread hmiPxyThread;

    SimInData * simInData;
    Configuration * cfg;
    ArchiveModel * archHdl;
    QString  fileInDataParams;
    QTimer * taskMonitTimer;

    QMap<QString, Json::Value> taskResInfo;
    QMap<QString, QTreeWidgetItem *> taskResItems;
    QMap<QString, Json::Value> processedTasksInfo;

    QAction * acWorkDir;
    QAction * acShowTaskInfo;
    QAction * acPauseTask;
    QAction * acResumeTask;
    QAction * acStopTask;

    QVBoxLayout * vlyFrmAgents;
    QSpacerItem * spacerFrmAgents;

    std::map<std::string, TaskAgentInfo*> taskAgentsInfo;
    std::map<std::string, FrmAgentStatus*> taskAgentsInfoPanel;
};

}

#endif // MAINWINDOW_H
