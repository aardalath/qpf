#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
#include "statem.h"

#include "dbmng.h"

#include "proctskstatmodel.h"
#include "sysalertmodel.h"
#include "procalertmodel.h"
#include "productsmodel.h"
#include "txtblmodel.h"

namespace Ui {
    class MainWindow;
}

namespace QPF {

class LogWatcher;

class MainWindow : public QMainWindow, LibComm::StateMachine
{
    Q_OBJECT

    // Valid Manager states
    static const int ERROR        = -1;
    static const int OFF          =  0;
    static const int INITIALISED  =  1;
    static const int RUNNING      =  2;
    static const int OPERATIONAL  =  3;

public:
    explicit MainWindow(QString dbUrl = QString("db://eucops:e314clid@localhost:5432/qpfdb"),
                        QString sessionName = QString(""),
                        QWidget *parent = 0);
    ~MainWindow();

protected:
    void manualSetupUI();
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

public slots:
    void updateMenus();
    void setActiveSubWindow(QWidget *window);
    void updateSystemView();

private slots:
    void saveAs();
    void cut();
    void copy();
    void paste();
    void about();
    void quitApp();
    void quitAllQPF();

    void updateWindowMenu();

    void processPendingEvents();

    void transitToOperational();

    void showConfigTool();
    void showDBBrowser();
    void showExtToolsDef();
    void showVerbLevel();

    void openWithDefault();
    void openWith();
    void showArchiveTableContextMenu(const QPoint & p);
    void openLocalArchiveElement(QModelIndex idx);

    void showAlertsContextMenu(const QPoint & p);
    void showAlertInfo(QTableView * tblvw);
    void showSysAlertInfo();
    void showProcAlertInfo();

    void sortTaskViewByColumn(int c);
    void showTaskMonitContextMenu(const QPoint & p);
    void showWorkDir();
    void displayTaskInfo();
    void pauseTask();
    void resumeTask();
    void stopTask();
    bool runDockerCmd(QModelIndex idx, QString cmd);
    void dumpTaskInfoToTree(QString taskName, const Json::Value & v, QTreeWidget * t);
    void dumpToTree(const Json::Value & v, QTreeWidgetItem * t);

    void sortTxViewByColumn(int c);
    void showTxContextMenu(const QPoint & p);
    void displayTxInfo();

    void closeTab(int n);

private:
    void readConfig(QString dbUrl);
    TextView *createTextView();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    QVariant getFromSettings(QString name);
    void putToSettings(QString name, QVariant value);
    void getUserToolsFromSettings();
    void putUserToolsToSettings();
    TextView *activeTextView();
    void switchLayoutDirection();
    void init();
    void setLogWatch();
    void handleFinishedHMI();
    void showState();

    virtual void defineValidTransitions();

    void initLocalArchiveView();
    void initTasksMonitView();
    void initAlertsTables();
    void initTxView();

    void updateAgentsMonitPanel();

    void showJSONdata(QString title, QString & dataString);
    void binaryGetFITSHeader(QString fileName, QString & tr);


private:
    Ui::MainWindow *ui;

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
    QAction *quitAllAct;
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

    QAction *acDefault;

    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    Configuration * cfg;

    QVector<LogWatcher*> nodeLogs;
    QStringList nodeNames;

    //HMIProxy * hmiNode;
    //std::thread hmiPxyThread;

    //SimInData * simInData;
    //ArchiveModel * archHdl;
    QString  fileInDataParams;
    QTimer * taskMonitTimer;

    QString inboxDirName;

    //QMap<QString, QJsonObject> & tasksInfo;

    //QMap<QString, Json::Value> taskResInfo;
    //QMap<QString, QTreeWidgetItem *> taskResItems;
    //QMap<QString, Json::Value> processedTasksInfo;

    //QList<Alert> alerts;

    QAction * acWorkDir;
    QAction * acShowTaskInfo;
    QAction * acPauseTask;
    QAction * acResumeTask;
    QAction * acStopTask;

    QAction * acShowMsgInfo;

    QAction * acShowAlert;
    QAction * acAckAlert;

    QAction * acArchiveShow;
    QMenu *   acArchiveOpenExt;
    QList<QAction *> acArchiveOpenExtTools;
    QMap<QString, QAction *> acUserTools;

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

    //DBManager *       dbMng;
};

}

#endif // MAINWINDOW_H
