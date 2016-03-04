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

#include <future>

#include "hmipxy.h"
#include "msgtypes.h"
#include "config.h"
#include "archivemodel.h"

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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    typedef LibComm::Router2RouterPeer::Peer   Peer;
    typedef LibComm::Router2RouterPeer::Frame  Frame;

    struct ProcessingElement {
        std::string name;
        std::string exePath;
        std::string inPath;
        std::string outPath;
    };

    struct QPFHMIConfig {
        std::string appName;
        std::string appVersion;
        std::string lastAccess;

        std::vector<std::string>                  productTypes;
        std::map<std::string, ProcessingElement*> procElems;

        Peer evtMngCfg;
        Peer qpfhmiCfg;
    };

signals:
    void goToOperational();

public slots:
    void commandSystem();

protected slots:
    void transitToOperational();
    void sendInData();
    void prepareSendMultInData();
    void selectInDataParamsFile();
    void prepareSendInDataFromFile();
    void sendMultInData();
    void sendInDataFromFile();
    void stopSendingMultInData();
    void handleFinishedHMI();

    void checkForTaskRes();
    void showTaskRes();

    void showTaskMonitContextMenu(const QPoint & p);
    void showWorkDir();
    void displayTaskInfo();

    void dumpTaskInfoToTree(QString taskName, Json::Value & v, QTreeWidget * t);
    void dumpToTree(Json::Value & v, QTreeWidgetItem * t);

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

    //----------------------------------------------------------------------
    // Method: stopSendingMessages
    //----------------------------------------------------------------------
    void stopSendingMessages();


private:
    Ui::MainWindow *ui;

    QVector<LogWatcher*> nodeLogs;

    HMIProxy * hmiNode;
    //std::future<int> hmiResult;
    std::thread hmiPxyThread;

    QList<ProductMetadata> multInDataContentValues;
    QString  fileInDataParams;
    QTimer * timerMultInData;

    //Config::Config * cfg;
    QPFHMIConfig    qpfCfg;
    Configuration * cfg;

    ArchiveModel * archHdl;

    QTimer * taskMonitTimer;
    QMap<QString, Json::Value> taskResInfo;

    QAction * acWorkDir;
    QAction * acDisplayTaskInfo;
};

}

#endif // MAINWINDOW_H
