/******************************************************************************
 * File:    mainwindow.h
 *          Declaration of class MainWindow
 *
 * Domain:  QPF.qpfgui.mainwindow
 *
 * Version:  2.0
 *
 * Date:    2016-11-03
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declaration of class MainWindow
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
 *   See <ChangeLog>
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

#include "datatypes.h"
#include "hmitypes.h"
#include "config.h"
#include "frmagentstatus.h"
#include "textview.h"
#include "sm.h"

#include "hmipxy.h"

#include "dbmng.h"

#include "proctskstatmodel.h"
#include "sysalertmodel.h"
#include "procalertmodel.h"
#include "productsmodel.h"
#include "txtblmodel.h"
#include "filters.h"
#include "procfmkmonit.h"

namespace Ui {
    class MainWindow;
}

namespace QPF {

class LogWatcher;
class ActionHandler;

class MainWindow : public QMainWindow, StateMachine
{
    Q_OBJECT

    friend class ActionHandler;

    // Valid Manager states
    static const int ERROR;
    static const int OFF;
    static const int INITIALISED;
    static const int RUNNING;
    static const int OPERATIONAL;

    // Valid Manager states
    static const std::string ERROR_StateName;
    static const std::string OFF_StateName;
    static const std::string INITIALISED_StateName;
    static const std::string RUNNING_StateName;
    static const std::string OPERATIONAL_StateName;

public:
    explicit MainWindow(QString url = QString("db://eucops:e314clid@localhost:5432/qpfdb"),
                        QString sessionName = QString(""),
                        QString masterAddr = QString("127.0.0.1"),
                        int conPort = DEFAULT_INITIAL_PORT,
                        QWidget *parent = 0);
    ~MainWindow();

    void commandFromHMIPxy(std::string command, std::string arg);
    
protected:
    void manualSetupUI();
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

public slots:
    void setAppInfo(QString name, QString rev, QString bld);

    void showConfigTool();
    void setActiveSubWindow(QWidget *window);
    void updateSystemView();
    void localarchViewUpdate();
    void resizeLocalArch();
    void updateLocalArchModel();
    void setAutomaticUpdateLocalArchModel(bool b);
    void setAutomaticExpandLocalArchModel(bool b);
    void setAutomaticResizeLocalArchModel(bool b);
    void setMultiselectProducts(bool b);
    void setProductsFilter(QString qry, QStringList hdr);
    void restartProductsFilter();

    void setAlertFilter(QString qry, QStringList hdr);
    void restartAlertFilter();

private slots:
    void processPath();
    void saveAs();
    void cut();
    void copy();
    void paste();
    void about();
    void quitApp();
    void quitAllQPF();

    void showSelectedInNav(QListWidgetItem* item);
    void selectRowInNav(int row);
    void removeRowInNav(int row);

    void reprocessProduct();
    void analyzeProduct();
    void exportProduct();

    void processPendingEvents();

    void transitToOperational();

    void showDBBrowser();
    void showExtToolsDef();
    void showVerbLevel();

    void openWithDefault();
    void openLocation();
    void openWith();
    void openLocalArchiveElement(QModelIndex idx);
    void openLocalArchiveFullPath(QString fullPath);

    void showAlertInfo(QTableView * tblvw, DBTableModel * model);
    void showSysAlertInfo();
    void showProcAlertInfo();
    void jumpToAlertSource(const QModelIndex & idx);
    
    void sortTaskViewByColumn(int c);

    void showWorkDir();
    void displayTaskInfo();
    //void restartTask();
    //void stopTask();

    void doTaskPause();
    void doTaskResume();
    void doTaskCancel();

    void doAgentSuspend();
    void doAgentStop();
    void doAgentReactivate();

    void doHostSuspend();
    void doHostStop();
    void doHostReactivate();

    bool runDockerCmd(QModelIndex idx, QString cmd);
    void dumpTaskInfoToTree(QString taskName, const Json::Value & v, QTreeWidget * t);
    void dumpToTree(const Json::Value & v, QTreeWidgetItem * t);

    void sortTxViewByColumn(int c);
    void displayTxInfo();

    void showTabsListMenu();
    void selectTabFromList();

    void jsontreeExpand();
    void jsontreeExpandSubtree();
    void jsontreeExpandAll();
    void jsontreeCollapse();
    void jsontreeCollapseSubtree();
    void jsontreeCollapseAll();

private:
    void readConfig(QString dbUrl);
    TextView *createTextView();
    void readSettings();
    void writeSettings();
    QVariant getFromSettings(QString name);
    void putToSettings(QString name, QVariant value);
    void getUserToolsFromSettings();
    void putUserToolsToSettings();
    TextView *activeTextView();
    void switchLayoutDirection();

    void processProductsInPath(QString folder);
    void getProductsInFolder(QString & path, QStringList & files, bool recursive = true);

    void init();
    void clearMDIArea();    
    void setLogWatch();
    void addPathToLogWatch(QString & logDir);
    
    void handleFinishedHMI();
    QString getState();
    void showState();

    virtual void defineValidTransitions();

    void attachJsonPopUpMenu(QWidget * w);

    void initLocalArchiveView();
    void setUToolTasks();
    void initTasksMonitView();
    void initAlertsTables();
    void initTxView();

    void updateAgentsMonitPanel();

    void showJSONdata(QString title, QString & dataString);
    void binaryGetFITSHeader(QString fileName, QString & tr);

    void getAllChildren(QModelIndex index, QModelIndexList &indices);

    void storeQUTools2Cfg(MapOfUserDefTools qutmap);

    //void convertQUTools2UTools(MapOfUserDefTools qutmap,
    //                           std::map<std::string, UserDefTool> & utmap);

private:
    Ui::MainWindow *ui;

    QString dbUrl;
    QString sessionId;
    QString masterAddress;
    int     startingPort;

    QSignalMapper *windowMapper;

    bool updateProductsModel;
    bool expandProductsModel;
    bool resizeProductsModel;


    QVector<LogWatcher*> nodeLogs;
    QStringList nodeNames;
    QVector<QString> activeNodes;
    bool isThereActiveCores;

    HMIProxy * hmiNode;

    QString  fileInDataParams;
    QTimer * taskMonitTimer;

    QString inboxDirName;

    QVBoxLayout * vlyFrmAgents;
    QSpacerItem * spacerFrmAgents;

    std::map<std::string, TaskAgentInfo*> taskAgentsInfo;
    std::map<std::string, FrmAgentStatus*> taskAgentsInfoPanel;

    // User Defined Tools
    MapOfUserDefTools userDefTools;
    QStringList       userDefProdTypes;

    ProcTaskStatusModel * procTaskStatusModel;
    SysAlertModel *       sysAlertModel;
    ProcAlertModel *      procAlertModel;
    ProductsModel *       productsModel;
    TxTableModel *        txModel;

    ProcFmkMonitor *        procFmkMonit;

    QMap<QString,QString> nodeStates;

    QMap<QString,TaskStatus>  agentProcStatus;
    QMap<QString,TaskStatus>  hostProcStatus;
    QMap<QString,QStringList> agentsInHost;
    QMap<QString,QString>     hostForAgent;

    QPoint pointOfAction;
    //DBManager *       dbMng;

    bool isProductsCustomFilterActive;
    bool isAlertsCustomFilterActive;
    bool isMenuForTabWidget;
    bool isViewsUpdateActive;

    QString newPathToWatch;

    ActionHandler * actHdl;
};

}

#endif // MAINWINDOW_H
