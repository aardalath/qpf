/******************************************************************************
 * File:    mainwindow.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.qpfhmi.mainwindow
 *
 * Version:  2.0
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implement MainWindow class
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

#include <iostream>
#include <fstream>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenu>

#include "version.h"

#include "tools.h"
#include "dbhdlpostgre.h"
#include "except.h"
#include "filenamespec.h"
#include "urlhdl.h"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include <QDebug>
#include <QDir>
#include <QtConcurrent/QtConcurrentRun>
#include <QTimer>
#include <QFileDialog>
#include <QScrollArea>
#include <QMessageBox>
#include <QDomDocument>
#include <QDesktopServices>
#include <QJsonObject>

#include <thread>
#include <QString>
#include <QListWidget>
#include <QPoint>
#include <QObject>
#include <QTextStream>
#include <QProcess>

#include "acthdl.h"

#include "logwatcher.h"
#include "progbardlg.h"
#include "txitemdlg.h"

#include "dlgshowtaskinfo.h"
#include "configtool.h"
#include "dbbrowser.h"
#include "exttoolsdef.h"
#include "verbleveldlg.h"
//#include "testrundlg.h"
#include "qjsonmodel.h"
#include "xmlsyntaxhighlight.h"
#include "dlgalert.h"
#include "dlgreproc.h"

#include "reqrep.h"
#include "pubsub.h"
#include "pipeline.h"

using Configuration::cfg;

Synchronizer synchro;

namespace QPF {

static const int MessageDelay = 2000;
static const char * FixedWidthStyle = "font: 8pt \"Droid Sans Mono\";";

// Valid Manager states
const int MainWindow::ERROR        = -1;
const int MainWindow::OFF          =  0;
const int MainWindow::INITIALISED  =  1;
const int MainWindow::RUNNING      =  2;
const int MainWindow::OPERATIONAL  =  3;

// Basic log macros
#define TMsg(s)  hmiNode->log(s, Log::TRACE)
#define DMsg(s)  hmiNode->log(s, Log::DEBUG)
#define IMsg(s)  hmiNode->log(s, Log::INFO)
#define WMsg(s)  hmiNode->log(s, Log::WARNING)
#define EMsg(s)  hmiNode->log(s, Log::ERROR)
#define FMsg(s)  hmiNode->log(s, Log::FATAL)

// Valid Manager state names (strings)
const std::string MainWindow::ERROR_StateName("ERROR");
const std::string MainWindow::OFF_StateName("OFF");
const std::string MainWindow::INITIALISED_StateName("INITIALISED");
const std::string MainWindow::RUNNING_StateName("RUNNING");
const std::string MainWindow::OPERATIONAL_StateName("OPERATIONAL");

void fwdCommandFromHMIPxy(void* context, std::string cmd, std::string arg) {
    static_cast<MainWindow*>(context)->commandFromHMIPxy(cmd, arg);
}


//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
MainWindow::MainWindow(QString url, QString sessionName,
                       QString masterAddr, int conPort,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    updateProductsModel(false),
    expandProductsModel(false),
    resizeProductsModel(false),
    isThereActiveCores(true)
{
    dbUrl         = url;
    sessionId     = sessionName;
    masterAddress = masterAddr;
    startingPort  = conPort;

    if (!sessionName.isEmpty()) {
        cfg.sessionId = sessionName.toStdString();
        setSessionTag(cfg.sessionId);
    }

    // Read QPF Configuration
    readConfig(dbUrl);

    if (masterAddr.isEmpty()) {
        masterAddress = QString::fromStdString(cfg.network.masterNode());
    }

    // Set-up UI
    ui->setupUi(this);
    manualSetupUI();

    defineValidTransitions();

    std::string msg = QString("masterAddress: %1    startingPort:  %2")
        .arg(masterAddress).arg(startingPort).toStdString();
    TRC(msg);

    init();

    // Prepare DBManager
    if (QSqlDatabase::connectionNames().isEmpty()) {
        QString databaseName ( Config::DBName.c_str() );
        QString userName     ( Config::DBUser.c_str() );
        QString password     ( Config::DBPwd.c_str() );
        QString hostName     ( Config::DBHost.c_str() );
        QString port         ( Config::DBPort.c_str() );

        DBManager::DBConnection connection = { "QPSQL",        // .driverName
                                               databaseName,
                                               userName, password,
                                               hostName, port.toInt() };
        DMsg("Adding DB connection");
        DBManager::addConnection("qpfdb", connection);
        DMsg("Adding DB connection - DONE");
    }

    showState();

    statusBar()->showMessage(tr("QPF HMI Ready . . ."), MessageDelay);

    DMsg("SessionId: " + cfg.sessionId);

    // Launch automatic view update timer
    QTimer * updateSystemViewTimer = new QTimer(this);
    connect(updateSystemViewTimer, SIGNAL(timeout()), this, SLOT(updateSystemView()));
    updateSystemViewTimer->start(1000);
    isViewsUpdateActive = true;
    
    //== Set log file watchers ========================================
    setLogWatch();

    // Finally, deactivate some elements
    ui->tabMainWgd->removeTab(1);    
}

//----------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------
MainWindow::~MainWindow()
{
    DBManager::close();
    delete ui;
}

//----------------------------------------------------------------------
// Method: setAppInfo
// Set information to show in the main window title
//----------------------------------------------------------------------
void MainWindow::setAppInfo(QString name, QString rev, QString bld)
{
    setWindowTitle(QString("%1 : %2 : %3").arg(name).arg(rev).arg(bld));
}

//----------------------------------------------------------------------
// Method: manualSetupUI
// Additional (manual) GUI setup actions
//----------------------------------------------------------------------
void MainWindow::manualSetupUI()
{
    actHdl = new ActionHandler(this);

    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            actHdl, SLOT(updateMenus()));

    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(setActiveSubWindow(QWidget*)));

    actHdl->createActions();
    actHdl->createMenus();
    actHdl->createToolBars();
    actHdl->createStatusBar();
    
    actHdl->updateMenus();

    actHdl->getAcQuit()->setShortcuts(QKeySequence::Close);
    actHdl->getAcQuitAll()->setShortcuts(QKeySequence::Quit);
    
    // Read settings and set title
    readSettings();
    setWindowTitle(tr("QLA Processing Framework"));
    setUnifiedTitleAndToolBarOnMac(true);
    ui->lblUptime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

    //== Tab panels handling ==========================================

    QTabBar * tb = ui->tabMainWgd->tabBar();
    tb->setTabIcon(0, QIcon(":/img/logs.png"));
    tb->setTabIcon(1, QIcon(":/img/messages.png"));
    tb->setTabIcon(2, QIcon(":/img/monit.png"));
    tb->setTabIcon(3, QIcon(":/img/storage2.png"));
    tb->setTabIcon(4, QIcon(":/img/alerts.png"));

    connect(ui->tabMainWgd, SIGNAL(currentChanged(int)),
            this, SLOT(selectRowInNav(int)));
    connect(ui->tabMainWgd, SIGNAL(tabCloseRequested(int)),
            actHdl, SLOT(closeTab(int)));

    ui->tabMainWgd->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tabMainWgd, SIGNAL(customContextMenuRequested(const QPoint &)),
            actHdl, SLOT(showTabsContextMenu(const QPoint &)));

    ui->lstwdgNav->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->lstwdgNav, SIGNAL(customContextMenuRequested(const QPoint &)),
            actHdl, SLOT(showTabsContextMenu(const QPoint &)));

    QToolButton * tbtnTabsList = new QToolButton(ui->tabMainWgd);
    tbtnTabsList->setArrowType(Qt::DownArrow);
    connect(tbtnTabsList, SIGNAL(clicked()),
            this, SLOT(showTabsListMenu()));
    ui->tabMainWgd->setCornerWidget(tbtnTabsList, Qt::TopLeftCorner);

    const QList<QString> fixedItemNames {"Log Information",
                                       //"Messages",
                                         "Monitoring",
                                         "Local Archive",
                                         "Processing Alerts"};
    foreach (QString s, fixedItemNames) {
        QListWidgetItem * itemForNav = new QListWidgetItem(s);
        QFont fn(itemForNav->font());
        fn.setBold(true);
        itemForNav->setFont(fn);
        ui->lstwdgNav->addItem(itemForNav);
    }

    connect(ui->lstwdgNav, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(showSelectedInNav(QListWidgetItem*)));

    //== Setup views ==================================================

    initLocalArchiveView();
    initTasksMonitView();
    initAlertsTables();
    initTxView();

    //== Setup models =================================================

    // 1. Processing Tasks Model
    procTaskStatusModel = new ProcTaskStatusModel;
    ui->tblvwTaskMonit->setModel(procTaskStatusModel);

    // 2. System Alerts Model
    sysAlertModel = new SysAlertModel;
    ui->tblvwSysAlerts->setModel(sysAlertModel);

    // 3. Processing Alerts Model
    procAlertModel = new ProcAlertModel;
    ui->tblvwAlerts->setModel(procAlertModel);

    // 4. Local Archive Products Model
    productsModel = new ProductsModel;
    ui->treevwArchive->setModel(productsModel);
    ui->treevwArchive->setItemDelegate(new DBTreeBoldHeaderDelegate(this));
    ui->treevwArchive->setSortingEnabled(true);

    // 5. Transmissions Model
    txModel = new TxTableModel(nodeNames);
    ui->tblvwTx->setModel(txModel);

    //== Setup processing hosts monitoring widgets ===================
    procFmkMonit = new ProcFmkMonitor(ui->scrollAreaAgents);
    procFmkMonit->setupHostsInfo(Config::procFmkInfo);
}

//----------------------------------------------------------------------
// Method: readConfig
// Reads configuration file
//----------------------------------------------------------------------
void MainWindow::readConfig(QString dbUrl)
{
    // Create configuration object and read configuration from DB
    cfg.init(dbUrl.toStdString());
    cfg.startingPort = startingPort;
    cfg.generateProcFmkInfoStructure();

    TRC(cfg.str());
    TRC(cfg.general.appName());
    TRC(cfg.network.masterNode());
    for (auto & kv : cfg.network.processingNodes()) {
        TRC(kv.first << ": " << kv.second);
    }
    TRC("Config::PATHBase:    " << Config::PATHBase);
    TRC("Config::PATHSession: " << Config::PATHSession);
    TRC("Config::PATHLog:     " << Config::PATHLog);

    std::vector<std::string> runPaths {
        Config::PATHSession,
            Config::PATHLog,
            Config::PATHRlog,
            Config::PATHTmp,
            Config::PATHTsk,
            Config::PATHMsg };
    for (auto & p : runPaths) {
        if (mkdir(p.c_str(), Config::PATHMode) != 0) {
            std::perror(("mkdir " + p).c_str());
            exit(EXIT_FAILURE);
        }
    }

    int mkdirStat = mkdir(Config::PATHWww.c_str(), Config::PATHMode);
    if ((mkdirStat != 0) && (errno != EEXIST)) {
        std::perror(("mkdir " + Config::PATHWww).c_str());
        exit(EXIT_FAILURE);
    }
    
    Log::setLogBaseDir(Config::PATHLog);

    putToSettings("lastCfgFile", QVariant(QString::fromStdString(cfg.cfgFileName)));

    // Get the name of the different Task Agents
    taskAgentsInfo.clear();

    nodeNames << tr("QPFHMI") << tr("EvtMng") << tr("LogMng")
              << tr("DatMng") << tr("TskOrc") << tr("TskMng");
    int h = 1;
    int j = 0;
    for (auto & kv : cfg.network.processingNodes()) {
        int numOfTskAgents = kv.second;
        for (unsigned int i = 0; i < numOfTskAgents; ++i, ++j) {
            /*QString taName = QString("TskAgent_%1_%2")
                .arg(h,2,10,QLatin1Char('0'))
                .arg(i+1,2,10,QLatin1Char('0'));
                std::string staName = taName.toStdString(); */
            std::string & staName = cfg.agentNames.at(j);
            TaskAgentInfo * taInfo = new TaskAgentInfo;
            (*taInfo)["name"]   = staName;
            (*taInfo)["client"] = kv.first;
            (*taInfo)["server"] = kv.first;
            taskAgentsInfo[staName] = taInfo;
        }
        ++h;
    }

    for (auto & kv : cfg.network.swarms()) {
        CfgGrpSwarm & swrm = kv.second;
        if (swrm.serviceNodes().size() > 0) {
            /*QString n = QString("Swarm_%1")
                .arg(QString::fromStdString(swrm.name()));
                std::string staName = n.toStdString();*/
            std::string & staName = cfg.agentNames.at(j);
            TaskAgentInfo * taInfo = new TaskAgentInfo;
            (*taInfo)["name"]   = staName;
            (*taInfo)["client"] = kv.first;
            (*taInfo)["server"] = kv.first;
            taskAgentsInfo[staName] = taInfo;
        }
        ++j;
    }

    QString lastAccess = QDateTime::currentDateTime().toString("yyyyMMddTHHmmss");
    cfg.general["lastAccess"] = lastAccess.toStdString();
    putToSettings("lastAccess", QVariant(lastAccess));

    getUserToolsFromSettings();
}

//----------------------------------------------------------------------
// Method: closeEvent
// Handles close events
//----------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    ui->mdiArea->closeAllSubWindows();
    if (ui->mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        writeSettings();
        event->accept();
    }
}

//----------------------------------------------------------------------
// Method: saveAs
// Saves log text to file
//----------------------------------------------------------------------
void MainWindow::saveAs()
{
    if (activeTextView() && activeTextView()->saveAs())
        statusBar()->showMessage(tr("File saved"), MessageDelay);
}

//----------------------------------------------------------------------
// Method: processPath
// Specify a folder and process the products contained therein
//----------------------------------------------------------------------
void MainWindow::processPath()
{
    QString folderName = QFileDialog::getExistingDirectory(this,
            tr("Process products in folder..."));
    if (! folderName.isEmpty()) {
        QtConcurrent::run(this, &MainWindow::processProductsInPath, folderName);
    }
}

#ifndef QT_NO_CLIPBOARD
void MainWindow::cut()
{
    if (activeTextView()) activeTextView()->getTextEditor()->cut();
}

void MainWindow::copy()
{
    if (activeTextView()) activeTextView()->getTextEditor()->copy();
}

void MainWindow::paste()
{
    if (activeTextView()) activeTextView()->getTextEditor()->paste();
}
#endif

//----------------------------------------------------------------------
// Method: about
// Reads configuration file
//----------------------------------------------------------------------
void MainWindow::about()
{
#ifndef BUILD_ID
#define BUILD_ID ""
#endif

    QString buildId(BUILD_ID);
    if (buildId.isEmpty()) {
        char buf[20];
        sprintf(buf, "%ld", (long)(time(0)));
        buildId = QString(buf);
    }

    QMessageBox::about(this, tr("About " APP_NAME),
                       tr("This is the " APP_PURPOSE " v " APP_RELEASE "\n"
                          APP_COMPANY "\n"
                          APP_DATE " - Build ") + buildId);
}

//----------------------------------------------------------------------
// Method: createTextView
// Create new subwindow
//----------------------------------------------------------------------
TextView *MainWindow::createTextView()
{
    TextView *child = new TextView;
    ui->mdiArea->addSubWindow(child);

#ifndef QT_NO_CLIPBOARD
    actHdl->activateClipboardFor(child);
#endif

    return child;
}
 
//----------------------------------------------------------------------
// Method: readSettings
// Read settings from system
//----------------------------------------------------------------------
void MainWindow::readSettings()
{
    QSettings settings(APP_SYS_NAME, APP_NAME);
    QPoint pos = settings.value("pos", QPoint(40, 40)).toPoint();
    QSize size = settings.value("size", QSize(800, 600)).toSize();
    move(pos);
    resize(size);
}

//----------------------------------------------------------------------
// Method: writeSettings
// Write current values of settings to system
//----------------------------------------------------------------------
void MainWindow::writeSettings()
{
    QSettings settings(APP_SYS_NAME, APP_NAME);
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

//----------------------------------------------------------------------
// Method: getFromSettings
// Reads settings file and retrieves a given variable
//----------------------------------------------------------------------
QVariant MainWindow::getFromSettings(QString name)
{
    QSettings settings(APP_SYS_NAME, APP_NAME);
    return settings.value(name);
}

//----------------------------------------------------------------------
// Method: putToSettings
// Stores a given variable to the settings file
//----------------------------------------------------------------------
void MainWindow::putToSettings(QString name, QVariant value)
{
    QSettings settings(APP_SYS_NAME, APP_NAME);
    settings.setValue(name, value);
}

//----------------------------------------------------------------------
// Method: getUserToolsFromSettings
// Retrieves user defined tools from settings file
//----------------------------------------------------------------------
void MainWindow::getUserToolsFromSettings()
{
    userDefTools.clear();
    int numUdefTools = cfg.userDefTools.size();
    for (int i = 0; i < numUdefTools; ++i) {
        CfgGrpUserDefToolsList & uts = cfg.userDefTools;

        QUserDefTool qudt;
        qudt.name       = QString::fromStdString(uts.name(i));
        qudt.desc       = QString::fromStdString(uts.description(i));
        qudt.exe        = QString::fromStdString(uts.executable(i));
        qudt.args       = QString::fromStdString(uts.arguments(i));
        qudt.prod_types = QString::fromStdString(uts.productTypes(i)).split(",");

        userDefTools[qudt.name] = qudt;
    }

    userDefProdTypes.clear();
    for (auto & s : cfg.products.productTypes()) {
        userDefProdTypes.append(QString::fromStdString(s));
    }
}

//----------------------------------------------------------------------
// Method: putUserToolsToSettings
// Retrieves user defined tools from settings file
//----------------------------------------------------------------------
void MainWindow::putUserToolsToSettings()
{
    /*
     TODO
     */
}

//----------------------------------------------------------------------
// Method: activeTextView
// Reads configuration file
//----------------------------------------------------------------------
TextView *MainWindow::activeTextView()
{
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
        return qobject_cast<TextView *>(activeSubWindow->widget());
    return 0;
}

//----------------------------------------------------------------------
// Method: switchLayoutDirection
// Reads configuration file
//----------------------------------------------------------------------
void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

//----------------------------------------------------------------------
// Method: setActiveSubWindow
//----------------------------------------------------------------------
void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window) return;
    ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

//----------------------------------------------------------------------
// Method: showConfigTool
// Shows configuration tool window
//----------------------------------------------------------------------
void MainWindow::showConfigTool()
{
    static ConfigTool cfgTool(0);

    cfgTool.prepare(userDefTools, userDefProdTypes);
    if (cfgTool.exec()) {
        TRC("Updating user tools!");
        cfgTool.getExtTools(userDefTools);
        setUToolTasks();
        if (hmiNode != 0) {
            hmiNode->sendNewConfig();
            TRC("Sending new configuration to all the nodes");
        }
    }
}

//----------------------------------------------------------------------
// Method: showDBBrowser
// Shows System Database Browser
//----------------------------------------------------------------------
void MainWindow::showDBBrowser()
{
    DBBrowser dlg;

    dlg.exec();
}

//----------------------------------------------------------------------
// Method: showExtToolsDef
// Shows external toos definition window
//----------------------------------------------------------------------
void MainWindow::showExtToolsDef()
{
    ExtToolsDef dlg;
    dlg.initialize(userDefTools, userDefProdTypes);
    if (dlg.exec()) {
        dlg.getTools(userDefTools);
        storeQUTools2Cfg(userDefTools);
        setUToolTasks();
    }
}

//----------------------------------------------------------------------
// Method: showVerbLevel
// Shows verbosity level selector dialog
//----------------------------------------------------------------------
void MainWindow::showVerbLevel()
{
    VerbLevelDlg dlg;

    if (dlg.exec()) {
        int minLvl = dlg.getVerbosityLevelIdx();
        Log::setMinLogLevel((Log::LogLevel)(minLvl));
        ui->lblVerbosity->setText(dlg.getVerbosityLevelName());
        hmiNode->sendMinLogLevel(dlg.getVerbosityLevelName().toStdString());
        statusBar()->showMessage(tr("Setting Verbosity level to ") + dlg.getVerbosityLevelName(), 2 * MessageDelay);
    }
}

//----------------------------------------------------------------------
// Method: quitApp
// Confirme and perform restart of the application
//----------------------------------------------------------------------
void MainWindow::quitApp()
{
    statusBar()->showMessage(tr("Quit application?"), 2 * MessageDelay);
    int ret = QMessageBox::warning(this, tr("Quit " APP_NAME),
                                   tr("Do you really want to quit the application?"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No);

    if (ret != QMessageBox::Yes) { return; }

    statusBar()->showMessage(tr("Closing HMI . . ."),
                             MessageDelay);

    qApp->closeAllWindows();
    qApp->quit();
}

//----------------------------------------------------------------------
// Method: quitAllQPF
// Confirme and perform restart of the application
//----------------------------------------------------------------------
void MainWindow::quitAllQPF()
{
    statusBar()->showMessage(tr("Quit application and end all QPF core instances?"),
                             2 * MessageDelay);
    int ret = QMessageBox::warning(this, tr("Quit " APP_NAME " and Core"),
                                   tr("Do you really want to quit the QLA Processing "
                                      "Framework HMI and Core instances?"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No);

    if (ret != QMessageBox::Yes) { return; }

    IMsg("Sending quit command to EvtMng . . .");
    hmiNode->quit();
    //hmiNode->sendCmd("EvtMng", "quit", "");
    //DBManager::addICommand("QUIT");

    statusBar()->showMessage(tr("STOP Signal being sent to all elements . . ."),
                             MessageDelay);
    sleep(1);

    qApp->closeAllWindows();
    qApp->quit();
}

//----------------------------------------------------------------------
// Method: processProductsInPath
// Specify a folder and process the products contained therein
//----------------------------------------------------------------------
void MainWindow::processProductsInPath(QString folder)
{

    // Get entire list (down the tree) of products in folder
    QStringList files;
    getProductsInFolder(folder, files);

    // Copy them (hard link, better) to the inbox

    std::string inbox = cfg.storage.inbox + "/";

    URLHandler uh;
    FileNameSpec fs;
    ProductMetadata m;
    foreach (const QString & fi, files) {
        fs.parseFileName(fi.toStdString(), m);
        m["urlSpace"] = UserSpace;
        uh.setProduct(m);
        m = uh.fromFolder2Inbox();
        //sleep(5);
    }
}

//----------------------------------------------------------------------
// Method:
// Obtain all the product files under the path
//----------------------------------------------------------------------
void MainWindow::getProductsInFolder(QString & path, QStringList & files, bool recursive)
{
    QDir dir(path);
    QFileInfoList allEntries = dir.entryInfoList(QDir::Files | QDir::Dirs |
            QDir::NoSymLinks | QDir::NoDotAndDotDot,
            QDir::Time | QDir::DirsLast);
    foreach (const QFileInfo & fi, allEntries) {
        QString absPath = fi.absoluteFilePath();
        if (fi.isDir()) {
            if (recursive) {
                getProductsInFolder(absPath, files, recursive);
            }
        } else {
            bool isProduct = true;
            if (isProduct) { files << absPath; }
        }
    }
}

//----------------------------------------------------------------------
// Method: transitToOperational
// Perform all setup tasks before stepping into Operational
//----------------------------------------------------------------------
void MainWindow::transitToOperational()
{
}

//----------------------------------------------------------------------
// Method: init
// Initialize non-GUI part of the component
//----------------------------------------------------------------------
void MainWindow::init()
{
    //-----------------------------------------------------------------
    // a. Create HMI node element
    //-----------------------------------------------------------------
    hmiNode = new HMIProxy("HMIProxy", masterAddress.toStdString(), &synchro);
    hmiNode->declareParentConnection(fwdCommandFromHMIPxy, (void*)(this));

    //-----------------------------------------------------------------
    // b. Create component connections
    //-----------------------------------------------------------------

    // Ports (in fact, deltas from startingPort)
    const int PortEvtMng     = 1;
    const int PortHMICmd     = 2;
    const int PortTskRepDist = 3;

    //QString msg = QString("Connecting to %1:%2")
    //  .arg(masterAddress).arg(startingPort + PortEvtMng);
    //std::string smsg = msg.toStdString();
    //TRC(smsg);

    // CHANNEL HMICMD - REQREP
    // - Out/In: QPFHMI/EvtMng
    std::string chnl     = ChnlHMICmd;
    QString qconnAddr    = QString("tcp://%1:%2")
      .arg(masterAddress).arg(startingPort + PortHMICmd);
    std::string connAddr = qconnAddr.toStdString();
    hmiNode->addConnection(chnl, new ReqRep(NN_REQ, connAddr));

    // CHANNEL FRAMEWORK MONITORING - PIPELINE
    // - Requester: TskMng
    // - Replier: QPFHMI
    chnl      = ChnlFmkMon;
    connAddr  = "ipc:///tmp/" + chnl + ".ipc";
    hmiNode->addConnection(chnl, new Pipeline(NN_PULL, connAddr));

    // START!
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    synchro.notify();
}

//----------------------------------------------------------------------
// METHOD: clearMDIArea
//----------------------------------------------------------------------
void MainWindow::clearMDIArea()
{
    // Clear MDI area
    QList<QMdiSubWindow *> swList = ui->mdiArea->subWindowList();
    for (int i = swList.count() - 1; i >= 0; --i) {
        QWidget * w = (QWidget*)(swList.at(i));
        ui->mdiArea->removeSubWindow(w);
        delete w;
    }
}

//----------------------------------------------------------------------
// METHOD: setLogWatch
//----------------------------------------------------------------------
void MainWindow::setLogWatch()
{
    statusBar()->showMessage(tr("Establishing log monitoring capture..."),
                             MessageDelay);

    clearMDIArea();
    nodeLogs.clear();
   
    // Get all log files from each log dir (log & rlog), and create a
    // MDI with the log viewer for each of them
    QString     logDir(QString::fromStdString(Log::getLogBaseDir()));

    addPathToLogWatch(logDir);
}

//----------------------------------------------------------------------
// METHOD: addPathToLogWatch
//----------------------------------------------------------------------
void MainWindow::addPathToLogWatch(QString & logDir)
{
    // Create list of log file names and window names (file basenames)
    QStringList logExtFilter("*.log");
    QStringList logFiles;
    logFiles << QDir(logDir).entryList(logExtFilter);
    QStringList wndNames;
    QStringList logFileNames;
    foreach (QString logBaseName, logFiles) {
        QString logFileName(logDir + "/" + logBaseName);
        logFileNames << logFileName;
        wndNames     << QFileInfo(logFileName).baseName();
    }

    // Clean up, removing those that match existing subwindows
    foreach (QMdiSubWindow * subw, ui->mdiArea->subWindowList()) {
        TextView * tv = qobject_cast<TextView *>(subw->widget());
        for (int i = logFileNames.size() - 1; i >= 0; --i) {
            const QString & bseName = wndNames.at(i);
            if (tv->logName() == bseName) {
                logFileNames.removeAt(i);
                wndNames.removeAt(i);
                continue;
            }
        }
    }
    
    // Create MDI window with the log file viewer
    for (int i = 0; i < logFileNames.size(); ++i) {
        const QString & logFileName = logFileNames.at(i);
        const QString & bseName     = wndNames.at(i);
      
        activeNodes << bseName;

        TextView * pltxted = new TextView;
        pltxted->setStyleSheet(FixedWidthStyle);
        pltxted->setLogName(bseName);
        LogWatcher * newLog = new LogWatcher(pltxted);
        newLog->setFile(logFileName);
        nodeLogs.append(newLog);
        QMdiSubWindow * subw = ui->mdiArea->addSubWindow(pltxted);
        subw->setWindowFlags(Qt::CustomizeWindowHint |
                            Qt::WindowTitleHint |
                            Qt::WindowMinMaxButtonsHint);
        subw->show();
        connect(newLog, SIGNAL(logUpdated()), this, SLOT(processPendingEvents()));
    }

    actHdl->updateWindowMenu();
}

//----------------------------------------------------------------------
// Slot: processPendingEvents
// Processes Application pending events
//----------------------------------------------------------------------
void MainWindow::processPendingEvents()
{
    qApp->processEvents();
}

//----------------------------------------------------------------------
// Slot: handleFinishedHMI
// Receives the signal of the HMI being finished
//----------------------------------------------------------------------
void MainWindow::handleFinishedHMI()
{
    statusBar()->showMessage(tr("HMI Finished..."), 10 * MessageDelay);
}

//----------------------------------------------------------------------
// Method: defineValidTransitions
// Define valid state transitions
//----------------------------------------------------------------------
void MainWindow::defineValidTransitions()
{
    defineState(ERROR,        ERROR_StateName);
    defineState(OFF,          OFF_StateName);
    defineState(INITIALISED,  INITIALISED_StateName);
    defineState(RUNNING,      RUNNING_StateName);
    defineState(OPERATIONAL,  OPERATIONAL_StateName);

    defineValidTransition(ERROR,        OFF);
    defineValidTransition(OFF,          INITIALISED);
    defineValidTransition(INITIALISED,  RUNNING);
    defineValidTransition(INITIALISED,  OFF);
    defineValidTransition(INITIALISED,  ERROR);
    defineValidTransition(RUNNING,      OPERATIONAL);
    defineValidTransition(RUNNING,      OFF);
    defineValidTransition(RUNNING,      ERROR);
    defineValidTransition(OPERATIONAL,  RUNNING);
    defineValidTransition(OPERATIONAL,  OFF);
    defineValidTransition(OPERATIONAL,  ERROR);

    setState(OFF);
}

//----------------------------------------------------------------------
// Method: getState
// Check DB to get the current system's state
//----------------------------------------------------------------------
QString MainWindow::getState()
{
    QString s = QString::fromStdString(cfg.sessionId);
    nodeStates.clear();
    //nodeStates = DBManager::getCurrentStates(s);

    std::map<std::string, std::string> ns = hmiNode->getNodeStates();
    for (auto & kv : ns) {
        nodeStates[QString::fromStdString(kv.first)] = QString::fromStdString(kv.second);
    }

    QString stateName = nodeStates["EvtMng"];
    isThereActiveCores = (stateName.toStdString() == getStateName(OPERATIONAL));
    return stateName;
}

//----------------------------------------------------------------------
// Method: showState
// Define valid state transitions
//----------------------------------------------------------------------
void MainWindow::showState()
{
    static std::map<int,std::string> stateColors
    {{OPERATIONAL,  "#008800"},
     {RUNNING,      "#0000C0"},
     {INITIALISED,  "#00C0C0"},
     {OFF,          "#101010"},
     {OFF,          "#FF1010"}};

    static std::map<int,std::string> stateStyle
    {{OPERATIONAL,  "QLabel { background-color : green; color : white; }"},
     {RUNNING,      "QLabel { background-color : lightgreen; color : blue; }"},
     {INITIALISED,  "QLabel { background-color : blue; color : lightgrey; }"},
     {OFF,          "QLabel { background-color : black; color : grey; }"},
     {ERROR,        "QLabel { background-color : red; color : orange; }"}};

    static QMap<QString,QString> prevNodeStates;

    // Show hosts and nodes
    std::string stateName = getState().toStdString();

    // Compare QMaps
    bool mapsAreEqual = nodeStates.size() == prevNodeStates.size();

    auto it  = nodeStates.constBegin();
    auto ite = nodeStates.constEnd();
    auto itte = prevNodeStates.constEnd();
    while ((it != ite) && mapsAreEqual) {
        auto k = it.key();
        QString s = ">>>" + k + " " + it.value() + ": ";
        auto itt = prevNodeStates.find(k);
        if (itt != itte) {
            s += itt.value();
            if (it.value() != itt.value()) {
                mapsAreEqual = false;
                hmiNode->log(s.toStdString());
            }
        } else {
            mapsAreEqual = false;
        }
        ++it;
    }

    if (!mapsAreEqual) {

        // Prepare master host line
        QString h = QString("<b>%1</b>").arg(masterAddress);
        QString p;
        for (auto & n : {"QPFHMI", "EvtMng", "LogMng", "DatMng", "TskOrc", "TskMng"}) {
            std::string ss = nodeStates[QString::fromStdString(n)].toStdString();
            int stateId = getStateIdx(ss);
            QString color = QString::fromStdString(stateColors[stateId]);
            p += QString("<font color=\"%1\">%2</font> ").arg(color).arg(QString::fromStdString(n));
        }
        
        // Now processing host lines
        int j = 1;
        for (auto & kv : cfg.network.processingNodes()) {
            QString pnName(QString::fromStdString(kv.first));
            bool storeInMaps = (agentsInHost.find(pnName) == agentsInHost.end());
            QStringList agList;
            h += QString("<br><b>%1</b>").arg(pnName);
            p += "<br>";
            int numOfTskAgents = kv.second;
            for (unsigned int i = 0; i < numOfTskAgents; ++i) {
                QString nn = QString("TskAgent_%1_%2")
                    .arg(j,2,10,QLatin1Char('0'))
                    .arg(i+1,2,10,QLatin1Char('0'));
                std::string ss = nodeStates[nn].toStdString();
                int stateId = getStateIdx(ss);
                QString color = QString::fromStdString(stateColors[stateId]);
                p += QString("<font color=\"%1\">%2</font> ").arg(color).arg(nn);
                
                if (storeInMaps) {
                    agList << nn;
                    hostForAgent[nn] = pnName;
                }
            }
            if (storeInMaps) {
                agentsInHost[pnName] = agList;
            }
            ++j;
        }
        
        for (auto & it : cfg.network.swarms()) {
            CfgGrpSwarm & swrm = it.second;
            if (swrm.serviceNodes().size() > 0) {
                h += QString("<br><b>%1</b>")
                    .arg(QString::fromStdString(swrm.serviceNodes().at(0)));
                p += "<br>";

                QString n = QString("Swarm_%1")
                    .arg(QString::fromStdString(swrm.name()));
                std::string ss = nodeStates[n].toStdString();
                int stateId = getStateIdx(ss);
                QString color = QString::fromStdString(stateColors[stateId]);
                p += QString("[%1] <font color=\"%2\">%3</font> ")
                    .arg(QString::fromStdString(it.first)).arg(color).arg(n);
            }
        }

        // Ended - show labels
        ui->lblHosts->setText(h);
        ui->lblNodes->setText(p);

        // Retrieve system state
        int stateId = getStateIdx(stateName);
        std::string stys = stateStyle[stateId];
        ui->lblSysStatus->setText(QString::fromStdString(stateName));
        ui->lblSysStatus->setStyleSheet(QString::fromStdString(stys));

        prevNodeStates = nodeStates;
    }
}

//----------------------------------------------------------------------
// Method: updateSystemView
// Send SIG_USR1 signal to qpf, to trigger the START message to be sent,
// or send STOP message, depending on the current status
//----------------------------------------------------------------------
void MainWindow::updateSystemView()
{
    static bool resTask = false;
    static bool resSysAlert = false;
    static bool resAlert = false;

    static const int LogTab = 0;
    static const int MsgsTab = -1;
    static const int TasksTab = 1;
    static const int LocalArchTab = 2;
    static const int ProcAlertsTab = 3;
    
    static const int ProcTasksTab = 0;
    static const int AgentsTab = 1;
    static const int SysAlertsTab = 2;

    static const int MsgContentCol = 5;
    static const int TaskDataCol = 9;

    //== 0. Ensure database connection is ready, and fetch state
    showState();

    actHdl->getAcQuitAll()->setEnabled(isThereActiveCores);

    //-- 0b. If received sessionId from master, add files to watch
    if (!newPathToWatch.isEmpty()) {
        addPathToLogWatch(newPathToWatch);
        newPathToWatch.clear();
    }

    if (isViewsUpdateActive) {
        
        int idx = ui->tabMainWgd->currentIndex();
        int idx2 = ui->tabWidget->currentIndex();
        
        switch (idx) {
            
        case LogTab:
            
            break;

        case MsgsTab:
            
            //== 5. Transmissions
            txModel->refresh();
            ui->tblvwTx->setColumnHidden(MsgContentCol, true);
            break;

        case TasksTab:
            
            switch (idx2) {
            case ProcTasksTab:
                //== 1. Processing tasks
                procTaskStatusModel->setFullUpdate(true);
                procTaskStatusModel->refresh();
                if ((!resTask) && (ui->tblvwTaskMonit->model()->rowCount() > 0)) {
                    ui->tblvwTaskMonit->resizeColumnsToContents();
                    resTask = true;
                }
                ui->tblvwTaskMonit->setColumnHidden(TaskDataCol, true);
                break;
                
            case AgentsTab:
                // 6. Agents Monit. Panel
                updateAgentsMonitPanel();
                break;
                
            case SysAlertsTab:
                //== 2. System Alerts
                sysAlertModel->refresh();
                if ((!resSysAlert) && (ui->tblvwSysAlerts->model()->rowCount() > 0)) {
                    ui->tblvwSysAlerts->resizeColumnsToContents();
                    resSysAlert = true;
                }
                break;
                
            default:
                break;            
            }
            break;
            
        case LocalArchTab:
            
            //== 4. Local Archive
            localarchViewUpdate();
            break;

        case ProcAlertsTab:
            
            //== 3. Diagnostics Alerts
            procAlertModel->refresh();
            if ((!resAlert) && (ui->tblvwAlerts->model()->rowCount() > 0)) {
                ui->tblvwAlerts->resizeColumnsToContents();
                resAlert = true;
            }
            break;
            
        default:
            break;

        }
        
    }
}

//----------------------------------------------------------------------
// SLOT: showSelectedInNav
//----------------------------------------------------------------------
void MainWindow::showSelectedInNav(QListWidgetItem* item)
{
    int itemRow = ui->lstwdgNav->currentRow();
    ui->tabMainWgd->setCurrentIndex(itemRow);
}

//----------------------------------------------------------------------
// SLOT: selectRowInNav
//----------------------------------------------------------------------
void MainWindow::selectRowInNav(int row)
{
    ui->lstwdgNav->setCurrentRow(row);
}

//----------------------------------------------------------------------
// SLOT: removeRowInNav
//----------------------------------------------------------------------
void MainWindow::removeRowInNav(int row)
{
    ui->lstwdgNav->takeItem(row);
}

//======================================================================
// Local Archive View configuration
//======================================================================

//----------------------------------------------------------------------
// METHOD: initLocalArchiveView
//----------------------------------------------------------------------
void MainWindow::initLocalArchiveView()
{
    actHdl->createLocalArchiveViewActions();

    // Filter initialisation
    FieldSet products;
    Field fld("product_id"          , STRING);   products[fld.name] = fld;
    fld = Field("product_type"      , STRING);   products[fld.name] = fld;
    fld = Field("product_version"   , STRING);   products[fld.name] = fld;
    fld = Field("signature"         , STRING);   products[fld.name] = fld;
    fld = Field("instrument_id"     , NUMBER);   products[fld.name] = fld;
    fld = Field("product_size"      , STRING);   products[fld.name] = fld;
    fld = Field("product_status_id" , NUMBER);   products[fld.name] = fld;
    fld = Field("creator_id"        , NUMBER);   products[fld.name] = fld;
    fld = Field("obsmode_id"        , NUMBER);   products[fld.name] = fld;
    fld = Field("start_time"        , DATETIME); products[fld.name] = fld;
    fld = Field("end_time"          , DATETIME); products[fld.name] = fld;
    fld = Field("registration_time" , DATETIME); products[fld.name] = fld;
    fld = Field("url"               , STRING);   products[fld.name] = fld;

    QStringList flds {"product_id", "product_type", "product_version", "signature",
                      "instrument_id", "product_size", "product_status_id", "creator_id",
                      "obsmode_id", "start_time", "end_time", "registration_time", "url"};
    QStringList hdrs {"Product Id", "Type", "Version", "Signature", "Instrument",
                      "Size", "Status", "Creator", "Obs.Mode", "Start", "End",
                      "Reg.Time", "URL"};
    ui->wdgFilters->setFields(products, flds, hdrs);
    ui->wdgFilters->setTableName("products_info");

    ui->wdgFilters->setVisible(false);
    connect(ui->wdgFilters, SIGNAL(filterIsDefined(QString,QStringList)),
            this, SLOT(setProductsFilter(QString,QStringList)));
    connect(ui->wdgFilters, SIGNAL(filterReset()),
            this, SLOT(restartProductsFilter()));

    isProductsCustomFilterActive = false;

    setUToolTasks();
}

//----------------------------------------------------------------------
// METHOD: setUToolTasks
//----------------------------------------------------------------------
void MainWindow::setUToolTasks()
{
    actHdl->getAcUserTools().clear();
    foreach (QString key, userDefTools.keys()) {
        const QUserDefTool & udt = userDefTools.value(key);
        QAction * ac = new QAction(key, ui->treevwArchive);
        ac->setStatusTip(udt.desc);
        connect(ac, SIGNAL(triggered()), this, SLOT(openWith()));
        actHdl->getAcUserTools()[key] = ac;
    }
}

//----------------------------------------------------------------------
// SLOT: localarchViewUpdate
// Updates the local archive view periodically
//----------------------------------------------------------------------
void MainWindow::localarchViewUpdate()
{
    if (updateProductsModel) {
        updateLocalArchModel();
    }
}

//----------------------------------------------------------------------
// SLOT: resizeLocalArch
// Updates the local archive model on demand
//----------------------------------------------------------------------
void MainWindow::resizeLocalArch()
{
    for (int i = 0; i < productsModel->columnCount(); ++i) {
        ui->treevwArchive->resizeColumnToContents(i);
    }
}

//----------------------------------------------------------------------
// SLOT: updateLocalArchModel
// Updates the local archive model on demand
//----------------------------------------------------------------------
void MainWindow::updateLocalArchModel()
{
    // Update context menus depending on config flags
    actHdl->getAcReprocess()->setEnabled(cfg.flags.allowReprocessing());
    
    // Update view
    productsModel->refresh();
    if (expandProductsModel) {
        ui->treevwArchive->expandAll();
    }
    if (resizeProductsModel) {
        resizeLocalArch();
    }
}

//----------------------------------------------------------------------
// SLOT: setAutomaticUpdateLocalArchModel
// Toggles automatic update of local arch model on/off
//----------------------------------------------------------------------
void MainWindow::setAutomaticUpdateLocalArchModel(bool b)
{
    updateProductsModel = b;
}

//----------------------------------------------------------------------
// SLOT: setAutomaticExpandLocalArchModel
// Toggles automatic expand of local arch model on/off upon updates
//----------------------------------------------------------------------
void MainWindow::setAutomaticExpandLocalArchModel(bool b)
{
    expandProductsModel = b;
}

//----------------------------------------------------------------------
// SLOT: setAutomaticResizeLocalArchModel
// Toggles automatic resize of local arch model on/off upon updates
//----------------------------------------------------------------------
void MainWindow::setAutomaticResizeLocalArchModel(bool b)
{
    resizeProductsModel = b;
}

//----------------------------------------------------------------------
// SLOT: setMultiselectProducts
// Toggles multiselection for Local Archive panel
//----------------------------------------------------------------------
void MainWindow::setMultiselectProducts(bool b)
{
    ui->treevwArchive->setSelectionMode(b ? QAbstractItemView::MultiSelection :
                                      QAbstractItemView::SingleSelection);
}

//----------------------------------------------------------------------
// SLOT: openWithDefaults
//----------------------------------------------------------------------
void MainWindow::openWithDefault()
{
    static const int NumOfURLCol = 10;

    QModelIndex m = ui->treevwArchive->currentIndex();
    QString url = m.model()->index(m.row(), NumOfURLCol, m.parent()).data().toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

//----------------------------------------------------------------------
// SLOT: openLocation
//----------------------------------------------------------------------
void MainWindow::openLocation()
{
    static const int NumOfURLCol = 10;

    QModelIndex m = ui->treevwArchive->currentIndex();
    QString url = m.model()->index(m.row(), NumOfURLCol, m.parent()).data().toString();
    QFileInfo fs(url.mid(7));
    url = fs.absolutePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

//----------------------------------------------------------------------
// SLOT: openWith
//----------------------------------------------------------------------
void MainWindow::openWith()
{
    static const int NumOfURLCol = 10;

    QAction * ac = qobject_cast<QAction*>(sender());
    QString key = ac->text();
    const QUserDefTool & udt = userDefTools.value(key);

    QModelIndex m = ui->treevwArchive->currentIndex();
    QString url = m.model()->index(m.row(), NumOfURLCol, m.parent()).data().toString();

    // Build arguments by replacing placeholders
    //   -   %f        Product file name (without path)
    //   -   %F        Product complete file name (with path)
    //   -   %p        Path where the product file is located
    //   -   %i        Product Id
    //   -   %o        Observation Id
    //   -   %s        Start time stamp
    //   -   %e        End time stamp
    //   -   %t        Product type
    //   -   %x        Product file extension
    //   -   %1 - %9   User-requested inputs
    QUrl archUrl(url);
    QString fileName = archUrl.path();
    QString args = udt.args;
    FileNameSpec fns;
    ProductMetadata md;
    fns.parseFileName(fileName.toStdString(), md);
    QFileInfo fs(fileName);

    args.replace("%f", fs.fileName());
    args.replace("%F", fileName);
    args.replace("%p", fs.absolutePath());
    args.replace("%i", QString::fromStdString(md.productId()));
    args.replace("%o", QString::fromStdString(md.signature()));
    args.replace("%s", QString::fromStdString(md.startTime()));
    args.replace("%e", QString::fromStdString(md.endTime()));
    args.replace("%t", QString::fromStdString(md.productType()));
    args.replace("%x", fs.suffix());

    // Count how many %n placeholders are
    int nph = 0;
    for (int i = 1; i < 10; ++i) {
        QString ph("%" + QString("%1").arg(i));
        if (args.contains(ph)) ++nph;
    }

    if (nph > 0) {

        // Build dialog
        QDialog dlg;
        QVBoxLayout vly;
        vly.addWidget(new QLabel("Current command line is:"));
        vly.addWidget(new QLabel(QString("\t%1 %2").arg(udt.exe).arg(args)));
        vly.addWidget(new QLabel("\nSpecify the strings to be used in the additional placeholders:"));

        QVector<QLineEdit*> edPh;
        for (int i = 1; i <= nph; ++i) {
            QLabel * l = new QLabel("%" + QString("%1: ").arg(i));
            QLineEdit * e = new QLineEdit;
            QHBoxLayout * hly = new QHBoxLayout;
            hly->addWidget(l);
            hly->addWidget(e);
            vly.addLayout(hly);
            edPh.append(e);
        }
        QPushButton * btnOk = new QPushButton("&Ok");
        QPushButton * btnCancel = new QPushButton("&Cancel");
        QHBoxLayout * hly = new QHBoxLayout;
        hly->addSpacerItem(new QSpacerItem(1, 1));
        hly->addWidget(btnOk);
        hly->addWidget(btnCancel);
        vly.addLayout(hly);
        dlg.setLayout(&vly);
        connect(btnOk, SIGNAL(pressed()), &dlg, SLOT(accept()));
        connect(btnCancel, SIGNAL(pressed()), &dlg, SLOT(reject()));

        // Show dialog, and substitute the placeholders %n in args with the
        // content of the line edit widgets
        if (dlg.exec()) {
            for (int i = 1; i <= nph; ++i) {
                QString ph("%" + QString("%1").arg(i));
                QLineEdit * e = edPh.at(i - 1);
                DMsg(("Trying to substitute '" + ph + "' with '" + e->text() + "'").toStdString());
                args.replace(ph, e->text());
            }
        }

    }

    //  Build command line and run the tool
    QString cmd(QString("%1 %2").arg(udt.exe).arg(args));
    QProcess tool;
    tool.startDetached(cmd);
}

//----------------------------------------------------------------------
// SLOT: reprocessProduct
//----------------------------------------------------------------------
void MainWindow::reprocessProduct()
{
    static const int NumOfURLCol = 10;

    QPoint p = actHdl->getAcReprocess()->property("clickedItem").toPoint();
    //QModelIndex m = ui->treevwArchive->indexAt(p);

    QStringList inProds;
    QModelIndexList list = ui->treevwArchive->selectionModel()->selectedIndexes();
    foreach (QModelIndex m, list) {
        if (m.column() == NumOfURLCol) {
            QString url = m.model()->index(m.row(), NumOfURLCol, m.parent()).data().toString();
            QUrl archUrl(url);
            QString fileName = archUrl.path();
            std::cerr << "Request of reprocessing: " << fileName.toStdString() << std::endl;
            inProds << fileName;
        }
    }
    
    std::string userWAType = QString::fromStdString(cfg.general.userAreaType()).toUpper().toStdString();
    DlgReproc::OutputsLocation out =
        ((userWAType == UserAreaName[UA_USER]) ?
         DlgReproc::DefUserArea : ((userWAType == UserAreaName[UA_LOCAL]) ?
                                   DlgReproc::CfgLocalDir : DlgReproc::CfgVOSpace));
    int flags = (cfg.flags.intermediateProducts() ?
                 DlgReproc::GenIntermProd : DlgReproc::NullFlags);

    DlgReproc dlg;
    dlg.setFields(inProds, out, flags);
    if (!dlg.exec()) { return; }

    QString outLocation;
    dlg.getFields(inProds, out, outLocation, flags);

    std::cerr << userWAType << ' ' << out << " : " << flags
              << " - " << outLocation.toStdString() << '\n';

    ProductList reprocProducts;
    FileNameSpec fns;
    ProductMetadata md;
    foreach (QString fileName, inProds) {
        fns.parseFileName(fileName.toStdString(), md);
        md["urlSpace"]       = ReprocessingSpace;
        md["procTargetType"] = (((out == DlgReproc::CfgLocalDir) ||
                                 (out == DlgReproc::LocalDir)) ?
                                UA_LOCAL : (((out == DlgReproc::CfgVOSpace) ||
                                             (out == DlgReproc::VOSpaceFolder)) ?
                                            UA_VOSPACE : UA_USER)); 
        md["procTarget"]     = outLocation.toStdString();
        reprocProducts.products.push_back(md);
    }

    std::cerr << md.urlSpace() << ' '
              << md.procTargetType() << ' '
              << md.procTarget() << ' '
              << '\n';

    hmiNode->sendReprocCmd(reprocProducts);
}

//----------------------------------------------------------------------
// SLOT: analyzeProduct
//----------------------------------------------------------------------
void MainWindow::analyzeProduct()
{
    static const int NumOfURLCol = 10;

    // Create product list

    QAction * caller = qobject_cast<QAction*>(sender());

    QPoint p = caller->property("clickedItem").toPoint();

    QModelIndexList list = ui->treevwArchive->selectionModel()->selectedIndexes();
    ProductList analyzeProducts;
    FileNameSpec fns;
    ProductMetadata md;

    foreach (QModelIndex m, list) {
        if (m.column() == NumOfURLCol) {
            QString url = m.model()->index(m.row(), NumOfURLCol, m.parent()).data().toString();
            fns.parseFileName(QUrl(url).path().toStdString(), md);
            md["urlSpace"]       = LocalArchSpace;
            md["procTargetType"] = UA_LOCAL; 
            md["procTarget"]     = cfg.connectivity.ipython.path();
            analyzeProducts.products.push_back(md);
        }
    }

    QString acName = caller->objectName();
    if (acName == "AnalyzeWithIPython") {
        
        // Link products to IPython working directory
        URLHandler urlh;
        for (auto & m : analyzeProducts.products) {
            urlh.setProduct(m);
            m = urlh.fromLocalArch2ExportLocation();
        }

        // Launch IPython session
        QProcess ipythonProcess;
        QString program = "xterm";
        QStringList arguments;
        arguments << "-e"
                  << QString::fromStdString(cfg.connectivity.ipython.cmd())
                  << "-i" << "-c"
                  << QString::fromStdString("'%cd " + cfg.connectivity.ipython.path() + "'");
        ipythonProcess.startDetached(program, arguments);       
        // ipythonProcess.waitForFinished();
        // QByteArray result = ipythonProcess.readAllStandardOutput();
        // const QString ipythonOutput(result);
        
    } else if (acName == "AnalyzeWithJupyter") {

        //TO DO: Send products to Jupyter remote host and open browser

    } else {

        //ERROR

    }
}

//----------------------------------------------------------------------
// SLOT: exportProduct
//----------------------------------------------------------------------
void MainWindow::exportProduct()
{
    static const int NumOfURLCol = 10;

    QPoint p = actHdl->getAcReprocess()->property("clickedItem").toPoint();
    //QModelIndex m = ui->treevwArchive->indexAt(p);

    QStringList inProds;
    QModelIndexList list = ui->treevwArchive->selectionModel()->selectedIndexes();
    foreach (QModelIndex m, list) {
        if (m.column() == NumOfURLCol) {
            QString url = m.model()->index(m.row(), NumOfURLCol, m.parent()).data().toString();
            QUrl archUrl(url);
            QString fileName = archUrl.path();
            std::cerr << "Request of reprocessing: " << fileName.toStdString() << std::endl;
            inProds << fileName;
        }
    }
    
    std::string userWAType = QString::fromStdString(cfg.general.userAreaType()).toUpper().toStdString();
    DlgReproc::OutputsLocation out =
        ((userWAType == UserAreaName[UA_USER]) ?
         DlgReproc::DefUserArea : ((userWAType == UserAreaName[UA_LOCAL]) ?
                                   DlgReproc::CfgLocalDir : DlgReproc::CfgVOSpace));
    int flags = (cfg.flags.intermediateProducts() ?
                 DlgReproc::GenIntermProd : DlgReproc::NullFlags);

    DlgReproc dlg;
    dlg.setLabels("Export products", "The following products will be exported",
                  "Export products to:", false);
    dlg.setFields(inProds, out, flags);
    if (!dlg.exec()) { return; }

    QString outLocation;
    dlg.getFields(inProds, out, outLocation, flags);

    std::cerr << userWAType << ' ' << out << " : " << flags
              << " - " << outLocation.toStdString() << '\n';

    ProductList exportProducts;
    FileNameSpec fns;
    ProductMetadata md;
    foreach (QString fileName, inProds) {
        fns.parseFileName(fileName.toStdString(), md);
        md["urlSpace"]       = LocalArchSpace;
        md["procTargetType"] = (((out == DlgReproc::CfgLocalDir) ||
                                 (out == DlgReproc::LocalDir)) ?
                                UA_LOCAL : (((out == DlgReproc::CfgVOSpace) ||
                                             (out == DlgReproc::VOSpaceFolder)) ?
                                            UA_VOSPACE : UA_USER)); 
        md["procTarget"]     = outLocation.toStdString();
        exportProducts.products.push_back(md);
    }

    std::cerr << md.urlSpace() << ' '
              << md.procTargetType() << ' '
              << md.procTarget() << ' '
              << '\n';

    // Copy products to export location
    URLHandler urlh;
    for (auto & m : exportProducts.products) {
        urlh.setProduct(m);
        m = urlh.fromLocalArch2ExportLocation();
    }
}

//----------------------------------------------------------------------
// SLOT: showJSONdata
//----------------------------------------------------------------------
void MainWindow::showJSONdata(QString title, QString & dataString)
{
    Json::Reader reader;
    Json::Value v;
    reader.parse(dataString.toStdString(), v);

    DlgShowTaskInfo dlg;
    dumpTaskInfoToTree(title, v, dlg.getTreeTaskInfo());
    dlg.setWindowTitle(title);
    dlg.setTaskInfo(dataString);
    dlg.exec();
}

//----------------------------------------------------------------------
// SLOT: openLocalArchiveElement
//----------------------------------------------------------------------
void MainWindow::openLocalArchiveElement(QModelIndex idx)
{
    static const int NumOfNameCol = 0;
    static const int NumOfURLCol = 10;

    int row = idx.row();
    TMsg(QString("(%1,%2)").arg(row).arg(idx.column()).toStdString());
    const QAbstractItemModel * model = idx.model();
    QModelIndex nameIdx = model->index(row, NumOfNameCol, idx.parent());
    QModelIndex urlIdx  = model->index(row, NumOfURLCol, idx.parent());

    QString url = urlIdx.data().toString().trimmed();
    if (url.left(7) == "file://") {
        url.remove(0, 7);
    } else if (url.left(7) == "http://") {
        // TODO Download file to temporary folder, and set url to temporary file
    } else if (url.left(8) == "https://") {
        // TODO Download file to temporary folder, and set url to temporary file
    }
    openLocalArchiveFullPath(url);
}

//----------------------------------------------------------------------
// SLOT: openLocalArchiveFullPath
//----------------------------------------------------------------------
void MainWindow::openLocalArchiveFullPath(QString fullPath)
{
    QFileInfo fs(fullPath);
    QString tabName = fs.fileName();
    QWidget * existingWdg = ui->tabMainWgd->findChild<QWidget*>(tabName);
    if (existingWdg != 0) {
        ui->tabMainWgd->setCurrentIndex(ui->tabMainWgd->indexOf(existingWdg));
        return;
    }

    QString iconRscName;
    QWidget * editor = 0;
    TMsg((fs.absoluteFilePath() + fs.suffix()).toStdString());
    if (fs.suffix() == "xml") {
        QFile file(fs.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            DMsg("Cannot open file");
            return;
        }
        QTextStream in(&file);
        QString content = in.readAll();
        QTextEdit * ed = new QTextEdit;
        XMLBasicSyntaxHighlighter * highlighter = new XMLBasicSyntaxHighlighter(ed);
        Q_UNUSED(highlighter);
        ed->setPlainText(content);
        ed->setReadOnly(true);
        editor = ed;
        iconRscName = ":/img/xml.png";
    } else if (fs.suffix() == "json") {
        QJsonModel * model = new QJsonModel;
        QTreeView * view = new QTreeView;
        view->setModel(model);
        view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        model->load(fs.absoluteFilePath());
        model->setIcon(QJsonValue::Bool, QIcon(":/img/bullet_black.png"));
        model->setIcon(QJsonValue::Double, QIcon(":/img/bullet_red.png"));
        model->setIcon(QJsonValue::String, QIcon(":/img/bullet_blue.png"));
        model->setIcon(QJsonValue::Array, QIcon(":/img/table.png"));
        model->setIcon(QJsonValue::Object, QIcon(":/img/brick.png"));
        attachJsonPopUpMenu(view);
        editor = view;
        iconRscName = ":/img/json.png";
    } else if (fs.suffix() == "fits") {
        QJsonModel * model = new QJsonModel;
        QTreeView * view = new QTreeView;
        view->setModel(model);
        view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        QString str;
        binaryGetFITSHeader(fs.absoluteFilePath(), str);
        QJsonDocument j = QJsonDocument::fromBinaryData(str.toLocal8Bit());
        if (j.isNull()) {
            DMsg("Null document!");
        }
        QFile js("js.json");
        if (js.open(QIODevice::ReadWrite)) {
            QTextStream stream(&js);
            stream << str << endl;
        }
        js.close();
        model->loadJson(str.toLocal8Bit());
        model->setIcon(QJsonValue::Bool, QIcon(":/img/bullet_black.png"));
        model->setIcon(QJsonValue::Double, QIcon(":/img/bullet_red.png"));
        model->setIcon(QJsonValue::String, QIcon(":/img/bullet_blue.png"));
        model->setIcon(QJsonValue::Array, QIcon(":/img/table.png"));
        model->setIcon(QJsonValue::Object, QIcon(":/img/brick.png"));
        attachJsonPopUpMenu(view);
        editor = view;
        iconRscName = ":/img/brick.png";
    } else if (fs.suffix() == "log") {
        QFile file(fs.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            DMsg("Cannot open file");
            return;
        }
        QTextStream in(&file);
        QString content = in.readAll();
        QTextEdit * ed = new QTextEdit;
        ed->setPlainText(content);
        ed->setReadOnly(true);
        editor = ed;
        iconRscName = ":/img/txt.png";
    } else {
        QFile file(fs.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            DMsg("Cannot open file");
            return;
        }
        QTextStream in(&file);
        QString content = in.readAll();
        QTextEdit * ed = new QTextEdit;
        ed->setPlainText(content);
        ed->setReadOnly(true);
        editor = ed;
        iconRscName = ":/img/bullet_blue.png";
    }

    // Ensure these tabs are closable (and only these)
    editor->setObjectName(tabName);
    int tabIdx = ui->tabMainWgd->addTab(editor, tabName);
    ui->tabMainWgd->setTabIcon(tabIdx, QIcon(iconRscName));
    ui->lstwdgNav->addItem(tabName);

    ui->tabMainWgd->setTabsClosable(true);
    for (int i = 0; i < 4; ++i) {
        ui->tabMainWgd->tabBar()->tabButton(i, QTabBar::RightSide)->hide();
        ui->tabMainWgd->tabBar()->tabButton(i, QTabBar::RightSide)->resize(0, 0);
    }
    QWidget * tabbtn = ui->tabMainWgd->tabBar()->tabButton(tabIdx, QTabBar::RightSide);
    QRect g = tabbtn->geometry();
    tabbtn->resize(7, 7);
    tabbtn->move(g.x() + 14, g.y() + 2);
}

//----------------------------------------------------------------------
// SLOT: showTabsListMenu
// Shows list of tabs, in order to select one to move focus on to
//----------------------------------------------------------------------
void MainWindow::showTabsListMenu()
{
    QWidget * w = qobject_cast<QWidget*>(sender());
    QPoint p(1, w->height() - 1);
    QList<QAction*> acList;
    for (int i = 0; i < ui->lstwdgNav->count(); ++i) {
        QAction * ac = new QAction(ui->lstwdgNav->item(i)->text(), this);
        ac->setData(QVariant(i));
        acList << ac;
        connect(ac, SIGNAL(triggered()), this, SLOT(selectTabFromList()));
    }
    QMenu menu(w);
    menu.addActions(acList);

    isViewsUpdateActive = false;
    menu.exec(w->mapToGlobal(p));
    isViewsUpdateActive = true;
}

//----------------------------------------------------------------------
// SLOT: selectTabFromList
// Selects hows list of tabs, in order to select one to move focus on to
//----------------------------------------------------------------------
void MainWindow::selectTabFromList()
{
    QAction * ac = qobject_cast<QAction*>(sender());
    ui->tabMainWgd->setCurrentIndex(ac->data().toInt());
}

//----------------------------------------------------------------------
// Method: attachJsonPopUpMenu
//----------------------------------------------------------------------
void MainWindow::attachJsonPopUpMenu(QWidget * w)
{
    w->setContextMenuPolicy(Qt::CustomContextMenu);
    //w->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(w, SIGNAL(customContextMenuRequested(const QPoint &)),
            actHdl, SLOT(showJsonContextMenu(const QPoint &)));
}

//----------------------------------------------------------------------
// SLOT: jsontreeExpand
//----------------------------------------------------------------------
void MainWindow::jsontreeExpand()
{
    QAction * ac = qobject_cast<QAction *>(sender());
    QTreeView * w = qobject_cast<QTreeView *>(ac->parent());
    QModelIndex idx = w->indexAt(pointOfAction);
    w->expand(idx);
}

//----------------------------------------------------------------------
// SLOT: jsontreeExpandSubtree
//----------------------------------------------------------------------
void MainWindow::jsontreeExpandSubtree()
{
    QAction * ac = qobject_cast<QAction *>(sender());
    QTreeView * w = qobject_cast<QTreeView *>(ac->parent());
    QModelIndex idx = w->indexAt(pointOfAction);
    QModelIndexList indices;
    getAllChildren(idx, indices);
    foreach (QModelIndex i, indices) {
        w->expand(i);
    }
}

//----------------------------------------------------------------------
// SLOT: jsontreeExpandAll
//----------------------------------------------------------------------
void MainWindow::jsontreeExpandAll()
{
    QAction * ac = qobject_cast<QAction *>(sender());
    QTreeView * w = qobject_cast<QTreeView *>(ac->parent());
    w->expandAll();

}

//----------------------------------------------------------------------
// SLOT: jsontreeCollapse
//----------------------------------------------------------------------
void MainWindow::jsontreeCollapse()
{
    QAction * ac = qobject_cast<QAction *>(sender());
    QTreeView * w = qobject_cast<QTreeView *>(ac->parent());
    QModelIndex idx = w->indexAt(pointOfAction);
    w->collapse(idx);
}

//----------------------------------------------------------------------
// SLOT: jsontreeCollapseSubtree
//----------------------------------------------------------------------
void MainWindow::jsontreeCollapseSubtree()
{
    QAction * ac = qobject_cast<QAction *>(sender());
    QTreeView * w = qobject_cast<QTreeView *>(ac->parent());
    QModelIndex idx = w->indexAt(pointOfAction);
    QModelIndexList indices;
    getAllChildren(idx, indices);
    foreach (QModelIndex i, indices) {
        w->collapse(i);
    }
}

//----------------------------------------------------------------------
// SLOT: jsontreeCollapseAll
//----------------------------------------------------------------------
void MainWindow::jsontreeCollapseAll()
{
    QAction * ac = qobject_cast<QAction *>(sender());
    QTreeView * w = qobject_cast<QTreeView *>(ac->parent());
    w->collapseAll();
}

//----------------------------------------------------------------------
// SLOT: getAllChildren
//----------------------------------------------------------------------
void MainWindow::getAllChildren(QModelIndex index, QModelIndexList &indices)
{
    indices.push_back(index);
    for (int i = 0; i != index.model()->rowCount(index); ++i)
        getAllChildren(index.child(i,0), indices);
}

//======================================================================
// Processing Tasks Monitoring View configuration
//======================================================================

//----------------------------------------------------------------------
// METHOD: initTasksMonitTree
//----------------------------------------------------------------------
void MainWindow::initTasksMonitView()
{
    static ProgressBarDelegate * progressBarDisplay = new ProgressBarDelegate(this, 7);

    actHdl->createTasksMonitViewActions();
    
    connect(ui->tblvwTaskMonit, SIGNAL(customContextMenuRequested(const QPoint &)),
            actHdl, SLOT(showTaskMonitContextMenu(const QPoint &)));

    connect(ui->tblvwTaskMonit->horizontalHeader(), SIGNAL(sectionClicked(int)),
            this, SLOT(sortTaskViewByColumn(int)));

    ui->tblvwTaskMonit->horizontalHeader()->setSectionsClickable(true);
    ui->tblvwTaskMonit->setSortingEnabled(false);
    ui->tblvwTaskMonit->setItemDelegateForColumn(7, progressBarDisplay);
}

//----------------------------------------------------------------------
// SLOT: sortTaskViewByColumn
//----------------------------------------------------------------------
void MainWindow::sortTaskViewByColumn(int c)
{
    ui->tblvwTaskMonit->sortByColumn(c);
}

//----------------------------------------------------------------------
// Method: showWorkDir
// Open file manager with task directory
//----------------------------------------------------------------------
void MainWindow::showWorkDir()
{
    QModelIndex idx        = ui->tblvwTaskMonit->currentIndex();
    QModelIndex nameExtIdx = ui->tblvwTaskMonit->model()->index(idx.row(), 3);
    QModelIndex dataIdx    = ui->tblvwTaskMonit->model()->index(idx.row(), 9);

    Json::Reader reader;
    Json::Value v;
    reader.parse(procTaskStatusModel->data(dataIdx).toString().toStdString(), v);

    QString localDir = QString::fromStdString(v["Mounts"][2]["Source"].asString());

    QFileInfo fs(localDir);
    if (fs.exists()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(localDir));
    } else {
        int ret = QMessageBox::warning(this, tr("Folder does not exist"),
                                       tr("The task folder does not exist in, "
                                          "or is not visible from this host.\n"
                                          "This problem normally appears when "
                                          "the task has been executed in a processing host "
                                          "that is not the host where the HMI "
                                          "is running"),
                                       QMessageBox::Ok);
    }
}

//----------------------------------------------------------------------
// Method: displayTaskInfo
// Show dialog with task information
//----------------------------------------------------------------------
void MainWindow::displayTaskInfo()
{
    QModelIndex idx = ui->tblvwTaskMonit->currentIndex();
    QModelIndex nameExtIdx = ui->tblvwTaskMonit->model()->index(idx.row(), 3);
    QModelIndex dataIdx = ui->tblvwTaskMonit->model()->index(idx.row(), 9);
    QString taskName = procTaskStatusModel->data(nameExtIdx).toString().trimmed();
    QString taskInfoString = procTaskStatusModel->data(dataIdx).toString().trimmed();

    showJSONdata("Task: " + taskName, taskInfoString);
}

//----------------------------------------------------------------------
// Method: restartTask
// Restarts selected (paused) task
//----------------------------------------------------------------------
// void MainWindow::restartTask()
// {
//     runDockerCmd(ui->tblvwTaskMonit->currentIndex(), "start");
// }

//----------------------------------------------------------------------
// Method: stopTask
// Stops selected task
//----------------------------------------------------------------------
// void MainWindow::stopTask()
// {
//     runDockerCmd(ui->tblvwTaskMonit->currentIndex(), "stop");
// }

//----------------------------------------------------------------------
// Method: doTaskPause
// TaskPause
//----------------------------------------------------------------------
void MainWindow::doTaskPause()
{
    QModelIndex idx        = ui->tblvwTaskMonit->currentIndex();
    QModelIndex statusIdx  = ui->tblvwTaskMonit->model()->index(idx.row(), 6);
    TaskStatus  status     = TaskStatusValue[procTaskStatusModel->data(statusIdx)
                                             .toString().toStdString()];

    if (status == TASK_RUNNING) {
        QModelIndex dataIdx    = ui->tblvwTaskMonit->model()->index(idx.row(), 9);
        
        Json::Reader reader;
        Json::Value v;
        reader.parse(procTaskStatusModel->data(dataIdx).toString().toStdString(), v);
        
        std::string taskId = v["Id"].asString();
        hmiNode->sendProcHdlCmd(PROC_TASK, taskId, PROC_HDL_PAUSE);
    }
}

//----------------------------------------------------------------------
// Method: doTaskResume
// TaskResume
//----------------------------------------------------------------------
void MainWindow::doTaskResume()
{
    QModelIndex idx        = ui->tblvwTaskMonit->currentIndex();
    QModelIndex statusIdx  = ui->tblvwTaskMonit->model()->index(idx.row(), 6);
    TaskStatus  status     = TaskStatusValue[procTaskStatusModel->data(statusIdx)
                                             .toString().toStdString()];

    if (status == TASK_PAUSED) {
        QModelIndex dataIdx    = ui->tblvwTaskMonit->model()->index(idx.row(), 9);
        
        Json::Reader reader;
        Json::Value v;
        reader.parse(procTaskStatusModel->data(dataIdx).toString().toStdString(), v);
        
        std::string taskId = v["Id"].asString();
        hmiNode->sendProcHdlCmd(PROC_TASK, taskId, PROC_HDL_RESUME);
    }
}

//----------------------------------------------------------------------
// Method: doTaskCancel
// TaskCancel
//----------------------------------------------------------------------
void MainWindow::doTaskCancel()
{
    QModelIndex idx        = ui->tblvwTaskMonit->currentIndex();
    QModelIndex statusIdx  = ui->tblvwTaskMonit->model()->index(idx.row(), 6);
    TaskStatus  status     = TaskStatusValue[procTaskStatusModel->data(statusIdx)
                                             .toString().toStdString()];

    if ((status == TASK_PAUSED) || (status == TASK_RUNNING)) {
        QModelIndex dataIdx    = ui->tblvwTaskMonit->model()->index(idx.row(), 9);
        
        Json::Reader reader;
        Json::Value v;
        reader.parse(procTaskStatusModel->data(dataIdx).toString().toStdString(), v);
        
        std::string taskId = v["Id"].asString();
        hmiNode->sendProcHdlCmd(PROC_TASK, taskId, PROC_HDL_CANCEL);
    }
}


//----------------------------------------------------------------------
// Method: doAgentSuspend
// AgentSuspend
//----------------------------------------------------------------------
void MainWindow::doAgentSuspend()
{
    QModelIndex idx        = ui->tblvwTaskMonit->currentIndex();
    QModelIndex dataIdx    = ui->tblvwTaskMonit->model()->index(idx.row(), 9);

    Json::Reader reader;
    Json::Value v;
    reader.parse(procTaskStatusModel->data(dataIdx).toString().toStdString(), v);
	
    std::string agentId = v["Info"]["Agent"].asString();
    QString qagentId = QString::fromStdString(agentId);
    if (agentProcStatus.find(qagentId) == agentProcStatus.end()) {
        agentProcStatus[qagentId] = TASK_RUNNING;
    }
    TaskStatus agentStatus = agentProcStatus[qagentId];
    TRC("Agent: " + agentId + " has status " + TaskStatusName[agentStatus]);
    if (agentStatus == TASK_RUNNING) {
        hmiNode->sendProcHdlCmd(PROC_AGENT, agentId, PROC_HDL_SUSPEND);
        agentProcStatus[qagentId] = TASK_PAUSED;
    }
}

//----------------------------------------------------------------------
// Method: doAgentStop
// AgentStop
//----------------------------------------------------------------------
void MainWindow::doAgentStop()
{
    QModelIndex idx        = ui->tblvwTaskMonit->currentIndex();
    QModelIndex dataIdx    = ui->tblvwTaskMonit->model()->index(idx.row(), 9);

    Json::Reader reader;
    Json::Value v;
    reader.parse(procTaskStatusModel->data(dataIdx).toString().toStdString(), v);
	
    std::string agentId = v["Info"]["Agent"].asString();
    QString qagentId = QString::fromStdString(agentId);
    if (agentProcStatus.find(qagentId) == agentProcStatus.end()) {
        agentProcStatus[qagentId] = TASK_RUNNING;
    }
    TaskStatus agentStatus = agentProcStatus[qagentId];
    if ((agentStatus == TASK_RUNNING) || (agentStatus == TASK_PAUSED)) {
        hmiNode->sendProcHdlCmd(PROC_AGENT, agentId, PROC_HDL_STOP);
        agentProcStatus[qagentId] = TASK_PAUSED;
    }
}

//----------------------------------------------------------------------
// Method: doAgentReactivate
// AgentReactivate
//----------------------------------------------------------------------
void MainWindow::doAgentReactivate()
{
    QModelIndex idx        = ui->tblvwTaskMonit->currentIndex();
    QModelIndex dataIdx    = ui->tblvwTaskMonit->model()->index(idx.row(), 9);

    Json::Reader reader;
    Json::Value v;
    reader.parse(procTaskStatusModel->data(dataIdx).toString().toStdString(), v);
	
    std::string agentId = v["Info"]["Agent"].asString();
    QString qagentId = QString::fromStdString(agentId);
    if (agentProcStatus.find(qagentId) == agentProcStatus.end()) {
        agentProcStatus[qagentId] = TASK_RUNNING;
        return;
    }
    TaskStatus agentStatus = agentProcStatus[qagentId];
    if (agentStatus == TASK_PAUSED) {
        hmiNode->sendProcHdlCmd(PROC_AGENT, agentId, PROC_HDL_REACTIVATE);
        agentProcStatus[qagentId] = TASK_RUNNING;
    }
}


//----------------------------------------------------------------------
// Method: doHostSuspend
// HostSuspend
//----------------------------------------------------------------------
void MainWindow::doHostSuspend()
{
    QModelIndex idx        = ui->tblvwTaskMonit->currentIndex();
    QModelIndex dataIdx    = ui->tblvwTaskMonit->model()->index(idx.row(), 9);

    Json::Reader reader;
    Json::Value v;
    reader.parse(procTaskStatusModel->data(dataIdx).toString().toStdString(), v);

    QString agName = QString::fromStdString(v["Info"]["Agent"].asString());
    QString qhostId = hostForAgent[agName];
    std::string hostId = qhostId.toStdString();
    if (hostProcStatus.find(qhostId) == hostProcStatus.end()) {
        hostProcStatus[qhostId] = TASK_RUNNING;
    }
    TaskStatus hostStatus = hostProcStatus[qhostId];
    if (hostStatus == TASK_RUNNING) {
        hmiNode->sendProcHdlCmd(PROC_HOST, hostId, PROC_HDL_SUSPEND);
        hostProcStatus[qhostId] = TASK_PAUSED;
    }
}

//----------------------------------------------------------------------
// Method: doHostStop
// HostStop
//----------------------------------------------------------------------
void MainWindow::doHostStop()
{
    QModelIndex idx        = ui->tblvwTaskMonit->currentIndex();
    QModelIndex dataIdx    = ui->tblvwTaskMonit->model()->index(idx.row(), 9);

    Json::Reader reader;
    Json::Value v;
    reader.parse(procTaskStatusModel->data(dataIdx).toString().toStdString(), v);
	
    QString agName = QString::fromStdString(v["Info"]["Agent"].asString());
    QString qhostId = hostForAgent[agName];
    std::string hostId = qhostId.toStdString();
    if (hostProcStatus.find(qhostId) == hostProcStatus.end()) {
        hostProcStatus[qhostId] = TASK_RUNNING;
    }
    TaskStatus hostStatus = hostProcStatus[qhostId];
    if ((hostStatus == TASK_RUNNING) || (hostStatus == TASK_PAUSED)) {
        hmiNode->sendProcHdlCmd(PROC_HOST, hostId, PROC_HDL_STOP);
        hostProcStatus[qhostId] = TASK_PAUSED;
    }
}

//----------------------------------------------------------------------
// Method: doHostReactivate
// HostReactivate
//----------------------------------------------------------------------
void MainWindow::doHostReactivate()
{
    QModelIndex idx        = ui->tblvwTaskMonit->currentIndex();
    QModelIndex dataIdx    = ui->tblvwTaskMonit->model()->index(idx.row(), 9);

    Json::Reader reader;
    Json::Value v;
    reader.parse(procTaskStatusModel->data(dataIdx).toString().toStdString(), v);
	
    QString agName = QString::fromStdString(v["Info"]["Agent"].asString());
    QString qhostId = hostForAgent[agName];
    std::string hostId = qhostId.toStdString();
    if (hostProcStatus.find(qhostId) == hostProcStatus.end()) {
        hostProcStatus[qhostId] = TASK_RUNNING;
        return;
    }
    TaskStatus hostStatus = hostProcStatus[qhostId];
    if (hostStatus == TASK_PAUSED) {
        hmiNode->sendProcHdlCmd(PROC_HOST, hostId, PROC_HDL_REACTIVATE);
        hostProcStatus[qhostId] = TASK_RUNNING;
    }
}

//----------------------------------------------------------------------
// Method: runDockerCmd
// Run command on selected task
//----------------------------------------------------------------------
bool MainWindow::runDockerCmd(QModelIndex idx, QString cmd)
{
    /*
      QString treeKey = item->data(0, Qt::UserRole).toString();
      const Json::Value & v = processedTasksInfo.value(treeKey);
      QString dId = QString::fromStdString(v["NameExtended"].asString());
    */

    QModelIndex dataIdx = ui->tblvwTaskMonit->model()->index(idx.row(), 9);
    QString taskInfoString = procTaskStatusModel->data(dataIdx).toString().trimmed();

    QJsonDocument doc = QJsonDocument::fromJson(taskInfoString.toUtf8());
    QJsonObject obj = doc.object();

    QString dId = obj["Id"].toString();
    QStringList args;
    args << cmd << dId;

    QProcess dockerCmd;
    dockerCmd.start("docker", args);
    return !dockerCmd.waitForFinished();
}


//----------------------------------------------------------------------
// Method: dumpTaskInfoToTree
// Puts task information in a tree form
//----------------------------------------------------------------------
void MainWindow::dumpTaskInfoToTree(QString taskName, const Json::Value & v, QTreeWidget * t)
{
    t->setColumnCount(2);
    QStringList hdrLabels;
    hdrLabels << "Item" << "Value";
    t->setHeaderLabels(hdrLabels);
    QTreeWidgetItem * root = new QTreeWidgetItem(t);
    root->setData(0, Qt::DisplayRole, taskName);
    dumpToTree(v, root);
    t->addTopLevelItem(root);
}

//----------------------------------------------------------------------
// Method: displayTaskInfo
// Dump a node of task info into tree form
//----------------------------------------------------------------------
void MainWindow::dumpToTree(const Json::Value & v, QTreeWidgetItem * t)
{
    t->setExpanded(true);
    if (v.size() > 0) {
        //        if (v.isArray()) {
        //        } else {
        for (Json::ValueIterator i = v.begin(); i != v.end(); ++i) {
            QTreeWidgetItem * chld = new QTreeWidgetItem(t);
            //DMsg("Show node key");
            //DMsg(i.key().asString().c_str());
            chld->setData(0, Qt::DisplayRole, QString::fromStdString(i.key().asString()));
            //DMsg("now dump children...");
            dumpToTree(*i, chld);
            t->addChild(chld);
            //            }
        }
    } else {
        QString s;
        if (v.isString() ) {
            s = QString::fromStdString(v.asString());
        } else if( v.isBool() ) {
            s = QString("%1").arg(v.asBool());
        } else if( v.isInt() ) {
            s = QString("%1").arg(v.asInt());
        } else if( v.isUInt() ) {
            s = QString("%1").arg(v.asUInt());
        } else if( v.isDouble() ) {
            s = QString("%1").arg(v.asDouble());
        } else  {
            s = "{}";//s = QString::fromStdString(v.asString());
        }
        t->setData(1, Qt::DisplayRole, s);
    }
}

//======================================================================
// Alerts view configuration
//======================================================================

//----------------------------------------------------------------------
// METHOD: initAlertsTables
//----------------------------------------------------------------------
void MainWindow::initAlertsTables()
{
    actHdl->createAlertsTablesActions();
    
    connect(ui->tblvwAlerts, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(jumpToAlertSource(const QModelIndex &)));
    
    ui->tblvwAlerts->setSortingEnabled(true);
    ui->tblvwSysAlerts->setSortingEnabled(true);

    // Filter initialisation
    QStringList a_sevs {"Warning", "Error", "Fatal"};
    QStringList a_typs {"Resource", "OutOfRange", "Diagnostic"};

    FieldSet alerts;
    Field fld("alert_id",   NUMBER);   alerts[fld.name] = fld;
    fld = Field("creation", DATETIME); alerts[fld.name] = fld;
    fld = Field("sev",      CHOICE);
    fld.setChoices(a_sevs);            alerts[fld.name] = fld;
    fld = Field("typ",      CHOICE);
    fld.setChoices(a_typs);            alerts[fld.name] = fld;
    fld = Field("origin",   STRING);   alerts[fld.name] = fld;
    fld = Field("msgs",     STRING);   alerts[fld.name] = fld;

    QStringList flds {"alert_id", "creation", "sev", "typ", "origin", "msgs"};
    QStringList hdrs {"ID", "Timestamp", "Severity", "Type", "Origin", "Information"};
    ui->wdgAlertFilters->setFields(alerts, flds, hdrs);
    ui->wdgAlertFilters->setTableName("alerts");

    ui->wdgAlertFilters->setVisible(false);
    connect(ui->wdgAlertFilters, SIGNAL(filterIsDefined(QString,QStringList)),
            this, SLOT(setAlertFilter(QString,QStringList)));
    connect(ui->wdgAlertFilters, SIGNAL(filterReset()),
            this, SLOT(restartAlertFilter()));

    isAlertsCustomFilterActive = false;
}

//----------------------------------------------------------------------
// Method: showAlertInfo
// Show dialog with alert information
//----------------------------------------------------------------------
void MainWindow::showAlertInfo(QTableView * tblvw, DBTableModel * model)
{
    QModelIndex idx = tblvw->currentIndex();
    Alert alert = model->getAlertAt(idx);
    DlgAlert dlg;
    dlg.setAlert(alert);
    dlg.exec();
    disconnect(actHdl->getAcShowAlert());
}

//----------------------------------------------------------------------
// Method: showSysAlertInfo
// Show dialog with system alert information
//----------------------------------------------------------------------
void MainWindow::showSysAlertInfo()
{
    showAlertInfo(ui->tblvwSysAlerts, sysAlertModel);
}

//----------------------------------------------------------------------
// Method: showProcAlertInfo
// Show dialog with system alert information
//----------------------------------------------------------------------
void MainWindow::showProcAlertInfo()
{
    showAlertInfo(ui->tblvwAlerts, procAlertModel);
}

//----------------------------------------------------------------------
// Method: showProcAlertInfo
// Show dialog with system alert information
//----------------------------------------------------------------------
void MainWindow::jumpToAlertSource(const QModelIndex & idx)
{
    Alert alert = procAlertModel->getAlertAt(idx);
    QString origin = QString::fromStdString(alert.getOrigin());
    std::cerr << origin.toStdString() << '\n';
    QStringList seq = origin.split('.');
    QString a = seq.takeFirst();
    QString b = seq.takeFirst();
    QString c = seq.takeFirst();
    QString d = seq.takeFirst();
    QString last = seq.takeLast();

    QString file = QString::fromStdString(alert.getFile());
    QFileInfo fs(file);
    QString tabName = fs.fileName();

    seq.prepend(tabName);
    seq << "diagnostics" << last;
    
    QWidget * existingWdg = ui->tabMainWgd->findChild<QWidget*>(tabName);
    if (existingWdg == 0) {
        openLocalArchiveFullPath(QString::fromStdString(cfg.storage.archive + "/") + file);
        existingWdg = ui->tabMainWgd->findChild<QWidget*>(tabName);
    }
    
    int tabIdx = ui->tabMainWgd->indexOf(existingWdg);
    QTreeView * wdg = (QTreeView*)(ui->tabMainWgd->widget(tabIdx));
    QJsonModel * mdl = (QJsonModel *)(wdg->model());
    QList<QModelIndex> idxs;

    std::function<void(const QModelIndex &x)> lExpand = [&](const QModelIndex &x) {
        int chldCount = x.model()->rowCount(x);
        for (int i = 0; i < chldCount; i++) { lExpand(x.child(i, 0)); }
        if (! wdg->isExpanded(x)) { wdg->expand(x); }
    };
    
    QModelIndex k;
    if (mdl->findSequence(mdl, seq, idxs)) {
        foreach (const QModelIndex & i, idxs) {
            k = mdl->index(i.row(), i.column(), k);
            wdg->expand(k);
        }
    }
    if (k != QModelIndex()) {
        lExpand(k);
        wdg->setCurrentIndex(k);
    }
    ui->tabMainWgd->setCurrentIndex(tabIdx);
}

//======================================================================
// Transmissions View configuration
//======================================================================

//----------------------------------------------------------------------
// METHOD: initTxView
//----------------------------------------------------------------------
void MainWindow::initTxView()
{
    actHdl->createTxViewActions();

    connect(ui->tblvwTx->horizontalHeader(), SIGNAL(sectionClicked(int)),
            this, SLOT(sortTxViewByColumn(int)));

    ui->tblvwTx->horizontalHeader()->setSectionsClickable(true);
    ui->tblvwTx->setSortingEnabled(true);
    ui->tblvwTx->setItemDelegate(new TxItemDelegate);
}

//----------------------------------------------------------------------
// SLOT: sortTxViewByColumn
//----------------------------------------------------------------------
void MainWindow::sortTxViewByColumn(int c)
{
    ui->tblvwTx->sortByColumn(c);
}

//----------------------------------------------------------------------
// Method: displayTxInfo
// Show dialog with task information
//----------------------------------------------------------------------
void MainWindow::displayTxInfo()
{
    QModelIndex idx     = ui->tblvwTx->currentIndex();
    QModelIndex fromIdx = ui->tblvwTx->model()->index(idx.row(), 2);
    QModelIndex toIdx   = ui->tblvwTx->model()->index(idx.row(), 3);
    QModelIndex typeIdx = ui->tblvwTx->model()->index(idx.row(), 4);
    QModelIndex dataIdx = ui->tblvwTx->model()->index(idx.row(), 5);
    QString msgName = (txModel->data(fromIdx).toString().trimmed() + "::[" +
                       txModel->data(typeIdx).toString().trimmed() + "]::" +
                       txModel->data(toIdx).toString().trimmed());
    QString contentString = txModel->data(dataIdx).toString().trimmed();

    showJSONdata(msgName, contentString);
}

//======================================================================
// Agents Monitoring Panel
//======================================================================

//----------------------------------------------------------------------
// METHOD: updateAgentsMonitPanel
//----------------------------------------------------------------------
void MainWindow::updateAgentsMonitPanel()
{
    static const int AgentsCol = 4;
    static const int StatusCol = 6;

    static int numOfRows = 0;

    // 2. Count tasks
    ProcTaskStatusModel * mdl = qobject_cast<ProcTaskStatusModel *>(ui->tblvwTaskMonit->model());
    int currentNumOfRows = mdl->rowCount();
    if (true) {  //currentNumOfRows > numOfRows) {
        QVector<double> loadAvgs = QVector<double>::fromStdVector(getLoadAvgs());
        for (auto & kv : taskAgentsInfo) {
            TaskAgentInfo * taInfo = kv.second;
            taInfo->restart();
            (*taInfo)["load1min"]   = loadAvgs.at(0) * 100;
            (*taInfo)["load5min"]   = loadAvgs.at(1) * 100;
            (*taInfo)["load15min"]  = loadAvgs.at(2) * 100;
        }
        for (int i = 0; i < currentNumOfRows; ++i) {
            std::string agent = mdl->index(i, AgentsCol).data().toString().toStdString();
            if (taskAgentsInfo.find(agent) != taskAgentsInfo.end()) {
                TaskAgentInfo * taInfo = taskAgentsInfo.find(agent)->second;
                std::string status = mdl->index(i, StatusCol).data().toString().toStdString();
                TaskStatus st = TaskStatusValue[status];
                switch (st) {
                case TASK_RUNNING:
                    (*taInfo)["running"]  = taInfo->running() + 1;;
                    break;
                case TASK_FINISHED:
                    (*taInfo)["finished"] = taInfo->finished() + 1;
                    break;
                case TASK_FAILED:
                    (*taInfo)["failed"]   = taInfo->failed() + 1;
                    break;
                case TASK_SCHEDULED:
                    (*taInfo)["waiting"]  = taInfo->waiting() + 1;
                    break;
                case TASK_PAUSED:
                    (*taInfo)["paused"]   = taInfo->paused() + 1;
                    break;
                case TASK_STOPPED:
                    (*taInfo)["stopped"]  = taInfo->stopped() + 1;
                    break;
                default:
                    break;
                }
                (*taInfo)["total"] = taInfo->total() + 1;
            }
        }
        numOfRows = currentNumOfRows;
    }
}

//----------------------------------------------------------------------
// SLOT: setPreoductsFilter
//----------------------------------------------------------------------
void MainWindow::setProductsFilter(QString qry, QStringList hdr)
{
    productsModel->defineQuery(qry);
    productsModel->defineHeaders(hdr);
    productsModel->skipColumns(0);
    productsModel->setCustomFilter(true);
    QAbstractItemDelegate * del = ui->treevwArchive->itemDelegate();
    qobject_cast<DBTreeBoldHeaderDelegate*>(del)->setCustomFilter(true);
    isProductsCustomFilterActive = true;
    productsModel->refresh();
}

//----------------------------------------------------------------------
// SLOT: restartProductsFilter
//----------------------------------------------------------------------
void MainWindow::restartProductsFilter()
{
    productsModel->setCustomFilter(false);
    QAbstractItemDelegate * del = ui->treevwArchive->itemDelegate();
    qobject_cast<DBTreeBoldHeaderDelegate*>(del)->setCustomFilter(false);
    productsModel->restart();
    isProductsCustomFilterActive = false;
}

//----------------------------------------------------------------------
// SLOT: setAlertFilter
//----------------------------------------------------------------------
void MainWindow::setAlertFilter(QString qry, QStringList hdr)
{
    procAlertModel->defineQuery(qry);
    procAlertModel->defineHeaders(hdr);
    procAlertModel->setFullUpdate(true);
    isAlertsCustomFilterActive = true;
    procAlertModel->refresh();
}

//----------------------------------------------------------------------
// SLOT: restartAlertFilter
//----------------------------------------------------------------------
void MainWindow::restartAlertFilter()
{
    procAlertModel->restart();
    isAlertsCustomFilterActive = false;
}

//======================================================================
// Utility methods
//======================================================================

//----------------------------------------------------------------------
// METHOD: binaryGetFITSHeader
//----------------------------------------------------------------------
void MainWindow::binaryGetFITSHeader(QString fileName, QString & str)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);    // read the data serialized from the file
    const int BUF_LEN = 80;
    const int TAG_LEN = 8;
    char buffer[BUF_LEN];
    int bytes, tbytes = 0;
    str = "{ \"Header\": { ";
    bool inHdr = true;
    bool isStart = true;
    int numOfExtensions = 0;

    while (!file.atEnd()) {
        bytes = in.readRawData(buffer, BUF_LEN);
        if (bytes < 0) { break; }
        tbytes += bytes;
        QString tag = QString::fromLocal8Bit(buffer, TAG_LEN);
        if (tag == "END     ") {
            str += "}";
            inHdr = false;
        } else {
            QString content = QString::fromLocal8Bit(buffer + TAG_LEN, BUF_LEN - TAG_LEN);
            if (content.at(0) == '=') {
                content.remove(0, 1);
            } else {
                content = "\"" + content + "\"";
            }
            if (tag == "XTENSION") {
                inHdr = true;
                content.replace(QRegExp("[ ]*/ .*$"), "");
                str += QString((numOfExtensions < 1) ? ", \"Extensions\": [ " : ", ") +
                    " { \"" + tag + "\": " + content;
                numOfExtensions++;
            } else {
                if (inHdr) {
                    content.replace(" T ", " true ");
                    content.replace(" F ", " false ");
                    content.replace(QRegExp("[ ]*/ .*$"), "");
                    if (!isStart) { str += ", "; }
                    str += "\"" + tag + "\": " + content;
                }
            }
        }
        isStart = false;
    }
    file.close();

    str += (numOfExtensions > 0) ? "] }": "}";
    //str.replace("'", "\"");
    str.replace(QRegExp("[ ]+\""), "\"");
    str.replace(QRegExp("[ ]+'"), "'");

    QRegExp * rx = new QRegExp("(\"HISTORY\"):(\".*\"),(\"HISTORY\":)");
    rx->setMinimal(true);
    int pos = 0;
    while ((pos = rx->indexIn(str, pos)) != -1) {
        QString srx(rx->cap(1) + ":" + rx->cap(2) + ",");
        str.replace(rx->cap(0), srx);
    }
    delete rx;
    rx = new QRegExp("(\"HISTORY\"):(\".*\")[}]");
    rx->setMinimal(true);
    pos = 0;
    while ((pos = rx->indexIn(str, pos)) != -1) {
        QString srx(rx->cap(1) + ": [" + rx->cap(2) + "] }");
        str.replace(rx->cap(0), srx);
        pos += srx.length();
    }
    delete rx;

    str.replace(",\"\":\"\"", "");
    str.replace("\":'", "\":\"");
    str.replace(QRegExp("'[ ]*,"), "\",");
}

//----------------------------------------------------------------------
// Method: convertQUTools2UTools
// Convert Qt map of user def tools to std map
//----------------------------------------------------------------------
void MainWindow::storeQUTools2Cfg(MapOfUserDefTools qutmap)
{
    json uts;

    QMap<QString, QUserDefTool>::const_iterator it  = qutmap.constBegin();
    auto end = qutmap.constEnd();
    int i = 0;
    while (it != end) {
        const QUserDefTool & t = it.value();

        uts[i]["name"]         = t.name.toStdString();
        uts[i]["description"]  = t.desc.toStdString();
        uts[i]["executable"]   = t.exe.toStdString();
        uts[i]["arguments"]    = t.args.toStdString();
        uts[i]["productTypes"] = t.prod_types.join(",").toStdString();

        ++it;
        ++i;
    }

    cfg.userDefTools.fromStr(JValue(uts).str());
}

//----------------------------------------------------------------------
// Method: commandFromHMIPxy
// Provides a way to do a HMI action ordered by HMI Proxy
//----------------------------------------------------------------------
void MainWindow::commandFromHMIPxy(std::string command, std::string arg)
{
    if (command == "linkSessionLogs") {
        // Regenerate log file watchers
        newPathToWatch = QString::fromStdString(Log::getLogBaseDir());
        newPathToWatch.replace(QString::fromStdString(cfg.sessionId),
                               QString::fromStdString(arg));
        TRC("Now checking =====> " + newPathToWatch.toStdString());
    }
}

/*
//----------------------------------------------------------------------
// Method: convertQUTools2UTools
// Convert Qt map of user def tools to std map
//----------------------------------------------------------------------
void MainWindow::convertQUTools2UTools(MapOfUserDefTools qutmap,
                                       std::map<std::string, UserDefTool> & utmap)
{
    utmap.clear();
    //QMap<QString, QUserDefTool>::
    QMap<QString, QUserDefTool>::const_iterator it  = qutmap.constBegin();
    auto end = qutmap.constEnd();
    while (it != end) {
        const QUserDefTool & t = it.value();
        UserDefTool ut;
        ut.name = t.name.toStdString();
        ut.args = t.args.toStdString();
        ut.desc = t.desc.toStdString();
        ut.exe  = t.exe.toStdString();
        foreach (const QString & s, t.prod_types) {
            ut.prod_types.push_back(s.toStdString());
        }
        utmap[ut.name] = ut;
    }
}
*/
}
