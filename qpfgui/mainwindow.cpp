/******************************************************************************
 * File:    mainwindow.cpp
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

#include <sys/stat.h>
#include <sys/types.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "version.h"

#include "tools.h"
#include "dbhdlpostgre.h"
#include "except.h"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include <QDebug>
#include <QDir>
#include <QtConcurrent/QtConcurrentRun>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QDomDocument>
#include <QDesktopServices>

#include <thread>

#include "logwatcher.h"
#include "progbardlg.h"
#include "txitemdlg.h"

#include "dlgshowtaskinfo.h"
#include "configtool.h"
#include "dbbrowser.h"
#include "exttoolsdef.h"
#include "verbleveldlg.h"
//#include "testrundlg.h"

#include "dlgalert.h"

namespace QPF {

static const int MessageDelay = 2000;
static const char * FixedWidthStyle = "font: 8pt \"Droid Sans Mono\";";

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
MainWindow::MainWindow(QString dbUrl, QString sessionName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setSessionTag(sessionName.toStdString());

    // Read QPF Configuration
    readConfig(dbUrl);

    // Set-up UI
    ui->setupUi(this);
    manualSetupUI();
    defineValidTransitions();

    // Prepare DBManager
    if (QSqlDatabase::connectionNames().isEmpty()) {
        QString databaseName ( QPF::Configuration::DBName.c_str() );
        QString userName     ( QPF::Configuration::DBUser.c_str() );
        QString password     ( QPF::Configuration::DBPwd.c_str() );
        QString hostName     ( QPF::Configuration::DBHost.c_str() );
        QString port         ( QPF::Configuration::DBPort.c_str() );

        DBManager::DBConnection connection = { "QPSQL",        // .driverName
                                               databaseName,
                                               userName, password,
                                               hostName, port.toInt() };
        DBManager::addConnection("qpfdb", connection);
    }

    //transitTo(INITIALISED);
    showState();

    statusBar()->showMessage(tr("QPF HMI Ready . . ."),
                             MessageDelay);


    // Launch automatic view update timer
    QTimer * updateSystemViewTimer = new QTimer(this);
    connect(updateSystemViewTimer, SIGNAL(timeout()), this, SLOT(updateSystemView()));
    updateSystemViewTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//----------------------------------------------------------------------
// Method: manualSetupUI
// Additional (manual) GUI setup actions
//----------------------------------------------------------------------
void MainWindow::manualSetupUI()
{
//    ui->tabpgEvtInj->setEnabled(false);
//    ui->btnStopMultInDataEvt->hide();

//    ui->actionActivate_Debug_Info->setEnabled(false);
//    ui->chkDebugInfo->setEnabled(false);

    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateMenus()));
    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(setActiveSubWindow(QWidget*)));

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updateMenus();

    // Read settings and set title
    readSettings();
    setWindowTitle(tr("QLA Processing Framework"));
    setUnifiedTitleAndToolBarOnMac(true);
    ui->lblUptime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

    QFrame * frm = new QFrame(0);
    vlyFrmAgents = new QVBoxLayout;
    frm->setLayout(vlyFrmAgents);
    ui->scrollAreaAgents->setWidget(frm);
    spacerFrmAgents = new QSpacerItem(10, 10,
                                      QSizePolicy::Minimum,
                                      QSizePolicy::Expanding);
    vlyFrmAgents->addSpacerItem(spacerFrmAgents);

    // Show hosts and nodes
    QString h, p;
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();
    for (auto & s : cfgInfo.machines) {
        if (! h.isEmpty()) { h += "\n"; }
        h += QString::fromStdString(s);
        if (! p.isEmpty()) { p += "\n"; }
        p += ":  ";
        for (auto & n : cfgInfo.machineNodes[s]) {
            p += QString::fromStdString(n) + " ";
        }
    }
    ui->lblHosts->setText(h);
    ui->lblNodes->setText(p);

    //== Set log file watchers ========================================

    setLogWatch();

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

    // 5. Transmissions Model
    txModel = new TxTableModel(nodeNames);
    ui->tblvwTx->setModel(txModel);
}

//----------------------------------------------------------------------
// Method: readConfig
// Reads configuration file
//----------------------------------------------------------------------
void MainWindow::readConfig(QString dbUrl)
{
    // Create configuration object and read configuration from DB
    cfg = new Configuration(dbUrl.toStdString().c_str());
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    putToSettings("lastCfgFile", QVariant(QString::fromStdString(cfgInfo.cfgFileName)));

    // Get the name of the different Task Agents
    taskAgentsInfo.clear();
    for (unsigned int i = 0; i < cfgInfo.peersCfg.size(); ++i) {
        const Peer & peer = cfgInfo.peersCfg.at(i);
        nodeNames << QString::fromStdString(peer.name);
        if (peer.type == "taskagent") {
            TaskAgentInfo * taInfo = new TaskAgentInfo;
            taInfo->name = peer.name;
            taInfo->client = peer.clientAddr;
            taInfo->server = peer.serverAddr;

            // Create task info structure for agent
            taskAgentsInfo[peer.name] = taInfo;
        }
    }

    Log::setLogBaseDir(Configuration::PATHRun);
/*
    qDebug() << QString::fromStdString(Configuration::PATHRun);

    for (auto & kv : taskAgentsInfo ) {
        qDebug() << kv.first.c_str()
                 << kv.second->name.c_str()
                 << kv.second->client.c_str()
                 << kv.second->server.c_str()
                 << kv.second->total << kv.second->running << kv.second->waiting
                 << kv.second->failed << kv.second->finished << kv.second->maxnum;
    }
*/
    QString lastAccess = QDateTime::currentDateTime().toString("yyyyMMddTHHmmss");
    cfg->setLastAccess(lastAccess.toStdString());
    putToSettings("lastAccess", QVariant(lastAccess));
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
   QMessageBox::about(this, tr("About " APP_NAME),
            tr("This is the " APP_PURPOSE " v " APP_RELEASE "\n"
                APP_COMPANY "\n"
                APP_DATE));
}

//----------------------------------------------------------------------
// Method: updateMenus
// Reads configuration file
//----------------------------------------------------------------------
void MainWindow::updateMenus()
{
    bool hasTextView = (activeTextView() != 0);

    saveAsAct->setEnabled(hasTextView);
#ifndef QT_NO_CLIPBOARD
    pasteAct->setEnabled(hasTextView);
#endif
    closeAct->setEnabled(hasTextView);
    closeAllAct->setEnabled(hasTextView);
    tileAct->setEnabled(hasTextView);
    cascadeAct->setEnabled(hasTextView);
    nextAct->setEnabled(hasTextView);
    previousAct->setEnabled(hasTextView);
    separatorAct->setVisible(hasTextView);

#ifndef QT_NO_CLIPBOARD
    bool hasSelection = (activeTextView() &&
                         activeTextView()->getTextEditor()->textCursor().hasSelection());
    cutAct->setEnabled(hasSelection);
    copyAct->setEnabled(hasSelection);
#endif
}

//----------------------------------------------------------------------
// Method: updateWindowMenu
// Updates the Window menu according to the current set of logs displayed
//----------------------------------------------------------------------
void MainWindow::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(separatorAct);

    QList<QMdiSubWindow *> windows = ui->mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        TextView *child = qobject_cast<TextView *>(windows.at(i)->widget());

        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1).arg(child->logName());
        } else {
            text = tr("%1 %2").arg(i + 1).arg(child->logName());
        }
        QAction *action  = windowMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeTextView());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    }
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
    connect(child, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
    connect(child, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
#endif

    return child;
}

//----------------------------------------------------------------------
// Method: createActions
// Create actions to be included in mainwindow menus
//----------------------------------------------------------------------
void MainWindow::createActions()
{
    // File menu
    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

//    restartAct = new QAction(tr("&Restart"), this);
//    //restartAct->setShortcuts(QKeySequence::Quit);
//    restartAct->setStatusTip(tr("Restart the application"));
//    connect(restartAct, SIGNAL(triggered()), this, SLOT(restart()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(quitApp()));

    // Edit menu
#ifndef QT_NO_CLIPBOARD
    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));
#endif

    // Tools menu
    configToolAct = new QAction(tr("&Configuration Tool ..."), this);
    configToolAct->setStatusTip(tr("Open Configuration Tool with current configuration"));
    connect(configToolAct, SIGNAL(triggered()), this, SLOT(showConfigTool()));

    browseDBAct = new QAction(tr("&Browse System DB ..."), this);
    browseDBAct->setStatusTip(tr("Open System Database Browser"));
    connect(browseDBAct, SIGNAL(triggered()), this, SLOT(showDBBrowser()));

    extToolsAct = new QAction(tr("&Define External Tools ..."), this);
    extToolsAct->setStatusTip(tr("Define external tools to open data products"));
    connect(extToolsAct, SIGNAL(triggered()), this, SLOT(showExtToolsDef()));

    verbosityAct = new QAction(tr("&Define Verbosity Level ..."), this);
    verbosityAct->setStatusTip(tr("Define verbosity level to be used in this session"));
    connect(verbosityAct, SIGNAL(triggered()), this, SLOT(showVerbLevel()));

    execTestRunAct = new QAction(tr("&Execute test run ..."), this);
    execTestRunAct->setStatusTip(tr("Execute a test run processing on dummy data"));
    connect(execTestRunAct, SIGNAL(triggered()), this, SLOT(execTestRun()));

    // Window menu
    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, SIGNAL(triggered()),
            ui->mdiArea, SLOT(closeActiveSubWindow()));

    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, SIGNAL(triggered()),
            ui->mdiArea, SLOT(closeAllSubWindows()));

    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, SIGNAL(triggered()), ui->mdiArea, SLOT(tileSubWindows()));

    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, SIGNAL(triggered()), ui->mdiArea, SLOT(cascadeSubWindows()));

    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAct, SIGNAL(triggered()),
            ui->mdiArea, SLOT(activateNextSubWindow()));

    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous "
                                 "window"));
    connect(previousAct, SIGNAL(triggered()),
            ui->mdiArea, SLOT(activatePreviousSubWindow()));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    // Help menu
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

//----------------------------------------------------------------------
// Method: createMenus
// Create mainwindow menus
//----------------------------------------------------------------------
void MainWindow::createMenus()
{
    // File menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(saveAsAct);
//    fileMenu->addSeparator();
//    fileMenu->addAction(restartAct);
    fileMenu->addSeparator();
//    QAction *action = fileMenu->addAction(tr("Switch layout direction"));
//    connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));
    fileMenu->addAction(quitAct);

    // Edit menu
    editMenu = menuBar()->addMenu(tr("&Edit"));
#ifndef QT_NO_CLIPBOARD
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
#endif

    // Tools menu
    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(configToolAct);
    toolsMenu->addAction(browseDBAct);
    toolsMenu->addAction(extToolsAct);
    toolsMenu->addSeparator();

    sessionInfoMenu = toolsMenu->addMenu(tr("&Session Information"));
    sessionInfoMenu->addAction(verbosityAct);

    toolsMenu->addSeparator();
    toolsMenu->addAction(execTestRunAct);

    // Window menu
    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

    // Help menu
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

//----------------------------------------------------------------------
// Method: closeEvent
// Reads configuration file
//----------------------------------------------------------------------
void MainWindow::createToolBars()
{
//    fileToolBar = addToolBar(tr("File"));
//    fileToolBar->addAction(quitAct);

//#ifndef QT_NO_CLIPBOARD
//    editToolBar = addToolBar(tr("Edit"));
//    editToolBar->addAction(cutAct);
//    editToolBar->addAction(copyAct);2000
//    editToolBar->addAction(pasteAct);
//#endif
}

//----------------------------------------------------------------------
// Method: createStatusBar
// Shows final Ready message in status bar
//----------------------------------------------------------------------
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
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
    getUserToolsFromSettings();
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
    putUserToolsToSettings();
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
    QSettings settings(APP_SYS_NAME, APP_NAME);
    int size = settings.beginReadArray("user_tools");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        UserDefTool udt;
        udt.name = settings.value("name").toString();
        udt.desc = settings.value("description").toString();
        udt.exe  = settings.value("executable").toString();
        udt.args = settings.value("arguments").toString();
        udt.prod_types = settings.value("product_types").toStringList();
        userDefTools[udt.name] = udt;
    }
    settings.endArray();
    userDefProdTypes = settings.value("product_types").toStringList();
}

//----------------------------------------------------------------------
// Method: putUserToolsToSettings
// Retrieves user defined tools from settings file
//----------------------------------------------------------------------
void MainWindow::putUserToolsToSettings()
{
    QSettings settings(APP_SYS_NAME, APP_NAME);
    settings.beginWriteArray("user_tools");
    int i = 0;
    foreach (QString key, userDefTools.keys()) {
        const UserDefTool & udt = userDefTools.value(key);
        settings.setArrayIndex(i);
        settings.setValue("name", udt.name);
        settings.setValue("description", udt.desc);
        settings.setValue("executable", udt.exe);
        settings.setValue("arguments", udt.args);
        settings.setValue("product_types", udt.prod_types);
        i++;
    }
    settings.endArray();
    settings.setValue("product_types", userDefProdTypes);
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
    ConfigTool cfgTool;

    cfgTool.readConfig();
    cfgTool.exec();
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
        putUserToolsToSettings();
    }
}

//----------------------------------------------------------------------
// Method: showVerbLevel
// Shows verbosity level selector dialog
//----------------------------------------------------------------------
void MainWindow::showVerbLevel()
{
    VerbLevelDlg dlg;

    dlg.exec();

    ui->lblVerbosity->setText(dlg.getVerbosityLevelName());
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

    //transitTo(OFF);
    showState();
    statusBar()->showMessage(tr("STOP Signal sent to all elements . . ."),
                             MessageDelay);

    qApp->closeAllWindows();
    qApp->quit();
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
    //ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    // Initialize and create node part as a separate thread
    qDebug() << "Initializing QPFHMI...";
    //hmiNode->initialize();
    qDebug() << "Trying to concurrentRun QPFHMI...";
    //hmiPxyThread = std::thread(&HMIProxy::concurrentRun, hmiNode);
    qDebug() << "QPFHMI should be concurrentRunning...";
}

//----------------------------------------------------------------------
// METHOD: setLogWatch
//----------------------------------------------------------------------
void MainWindow::setLogWatch()
{
    statusBar()->showMessage(tr("Establishing log monitoring capture..."),
                             MessageDelay);

    // Clear MDI area
    QList<QMdiSubWindow *> swList = ui->mdiArea->subWindowList();
    for (int i = swList.count() - 1; i >= 0; --i) {
        QWidget * w = (QWidget*)(swList.at(i));
        ui->mdiArea->removeSubWindow(w);
        delete w;
    }
    nodeLogs.clear();

    // Get all log files from each log dir (log & rlog), and create a
    // MDI with the log viewer for each of them
    QStringList logExtFilter("*.log");
    foreach (QString logDirName, QStringList({"log", "rlog"})) {
        QString logDir = QString::fromStdString(Log::getLogBaseDir()) + "/" + logDirName;
        QStringList logFiles;
        logFiles << QDir(logDir).entryList(logExtFilter);
        // Create MDI window with the log file viewer
        foreach (QString logBaseName, logFiles) {
            QString logFileName(logDir + "/" + logBaseName);
            TextView * pltxted = new TextView;
            pltxted->setStyleSheet(FixedWidthStyle);
            pltxted->setLogName(QFileInfo(logFileName).baseName());
            LogWatcher * newLog = new LogWatcher(pltxted);
            newLog->setFile(logFileName);
            nodeLogs.append(newLog);
            QMdiSubWindow * subw = ui->mdiArea->addSubWindow(pltxted);
            subw->setWindowFlags(Qt::CustomizeWindowHint |
                                 Qt::WindowTitleHint |
                                 Qt::WindowMinMaxButtonsHint);
            connect(newLog, SIGNAL(logUpdated()), this, SLOT(processPendingEvents()));
        }
    }
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
    defineState(ERROR,        "ERROR");
    defineState(OFF,          "OFF");
    defineState(INITIALISED,  "INITIALISED");
    defineState(RUNNING,      "RUNNING");
    defineState(OPERATIONAL,  "OPERATIONAL");

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
// Method: showState
// Define valid state transitions
//----------------------------------------------------------------------
void MainWindow::showState()
{
    // Retrieve system state
    QString stateName = DBManager::getState();
    int currentState = getStateIdx(stateName.toStdString());

    QString stys;
    switch (currentState) {
    case ERROR:
        stys = "QLabel { background-color : red; color : orange; }";
        break;
    case OFF:
        stys = "QLabel { background-color : black; color : grey; }";
        break;
    case INITIALISED:
        stys = "QLabel { background-color : blue; color : lightgrey; }";
        break;
    case RUNNING:
        stys = "QLabel { background-color : lightgreen; color : blue; }";
        break;
    case OPERATIONAL:
        stys = "QLabel { background-color : green; color : white; }";
        break;
    default:
        break;
    }
    ui->lblSysStatus->setText(stateName);
    ui->lblSysStatus->setStyleSheet(stys);
}

//----------------------------------------------------------------------
// Method: updateSystemView
// Send SIG_USR1 signal to qpf, to trigger the START message to be sent,
// or send STOP message, depending on the current status
//----------------------------------------------------------------------
void MainWindow::updateSystemView()
{
    //== 0. Ensure database connection is ready, and fetch state
    showState();

    //== 1. Processing tasks
    procTaskStatusModel->refresh();
    ui->tblvwTaskMonit->resizeColumnsToContents();
    const int TaskDataCol = 9;
    ui->tblvwTaskMonit->setColumnHidden(TaskDataCol, true);

    //== 2. System Alerts
    sysAlertModel->refresh();
    ui->tblvwSysAlerts->resizeColumnsToContents();

    //== 3. Diagnostics Alerts
    procAlertModel->refresh();
    ui->tblvwAlerts->resizeColumnsToContents();

    //== 3. Local Archive
    productsModel->refresh();
    for (int i = 0; i < productsModel->columnCount(); ++i) {
        ui->treevwArchive->resizeColumnToContents(i);
    }

    //== 5. Transmissions
    txModel->refresh();
    const int MsgContentCol = 5;
    ui->tblvwTx->setColumnHidden(MsgContentCol, true);

    // 6. Agents Monit. Panel
    updateAgentsMonitPanel();
}

//======================================================================
// Local Archive View configuration
//======================================================================

//----------------------------------------------------------------------
// METHOD: initLocalArchiveView
//----------------------------------------------------------------------
void MainWindow::initLocalArchiveView()
{
    // Create pop-up menu with user defined tools
    ui->treevwArchive->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treevwArchive, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showArchiveTableContextMenu(const QPoint &)));

    acArchiveOpenExt = new QMenu(tr("Open with ..."), ui->treevwArchive);

    acArchiveShow = new QAction("Show location in local archive", ui->treevwArchive);
    connect(acArchiveShow, SIGNAL(triggered()), this, SLOT(openWith()));

    acDefault = new QAction("System Default", ui->treevwArchive);
    connect(acDefault, SIGNAL(triggered()), this, SLOT(openWithDefault()));

    foreach (QString key, userDefTools.keys()) {
        const UserDefTool & udt = userDefTools.value(key);
        QAction * ac = new QAction(key, ui->treevwArchive);
        ac->setStatusTip(udt.desc);
        connect(ac, SIGNAL(triggered()), this, SLOT(openWith()));
        acUserTools[key] = ac;
    }
}

//----------------------------------------------------------------------
// SLOT: openWithDefaults
//----------------------------------------------------------------------
void MainWindow::openWithDefault()
{
    QModelIndex m = ui->treevwArchive->currentIndex();
    qDebug() << m;
    QString url = m.model()->index(m.row(), 11, m.parent()).data().toString();
    qDebug() << url;
    QDesktopServices::openUrl(QUrl(url));
}

//----------------------------------------------------------------------
// SLOT: openWith
//----------------------------------------------------------------------
void MainWindow::openWith()
{
    QAction * ac = qobject_cast<QAction*>(sender());
    QString key = ac->text();
    const UserDefTool & udt = userDefTools.value(key);

    QModelIndex m = ui->treevwArchive->currentIndex();
    qDebug() << m;
    QString url = m.model()->index(m.row(), 11, m.parent()).data().toString();
    qDebug() << url;

    QUrl archUrl(url);
    QString fileName = archUrl.path() + "/" + archUrl.fileName();
    QString args = udt.args;
    args.replace("%F", fileName);
    QString cmd(QString("%1 %2").arg(udt.exe).arg(args));
    QProcess tool;
    tool.startDetached(cmd);
}

//----------------------------------------------------------------------
// SLOT: showArchiveTableContextMenu
//----------------------------------------------------------------------
void MainWindow::showArchiveTableContextMenu(const QPoint & p)
{
    QModelIndex m = ui->treevwArchive->indexAt(p);
    if (m.parent() == QModelIndex()) { return; }

    QModelIndex m2 = m.model()->index(m.row(), 2, m.parent());
    if (!m2.data().isValid()) { return; }
    QString productType = m2.data().toString();
    QList<QAction *> actions;

    if (ui->treevwArchive->indexAt(p).isValid()) {
        foreach (QString key, userDefTools.keys()) {
            const UserDefTool & udt = userDefTools.value(key);
            if (udt.prod_types.contains(productType) || true) {
                QAction * ac = acUserTools[key];
                actions.append(ac);
            }
        }
        acArchiveOpenExt->addAction(acDefault);
        acArchiveOpenExt->addSeparator();
        acArchiveOpenExt->addActions(actions);
        QMenu menu(this);
        menu.addAction(acArchiveShow);
        menu.addSeparator();
        menu.addMenu(acArchiveOpenExt);
        menu.exec(ui->treevwArchive->mapToGlobal(p));
        //QMenu::exec(actions, ui->treevwArchive->mapToGlobal(p));
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

//======================================================================
// Processing Tasks Monitoring View configuration
//======================================================================

//----------------------------------------------------------------------
// METHOD: initTasksMonitTree
//----------------------------------------------------------------------
void MainWindow::initTasksMonitView()
{
    static ProgressBarDelegate * progressBarDisplay = new ProgressBarDelegate(this, 7);

    ui->tblvwTaskMonit->setContextMenuPolicy(Qt::CustomContextMenu);

    acWorkDir      = new QAction(tr("Open task working directory..."), ui->tblvwTaskMonit);
    acShowTaskInfo = new QAction(tr("Display task information"), ui->tblvwTaskMonit);
    acPauseTask    = new QAction(tr("Pause"), ui->tblvwTaskMonit);
    acResumeTask   = new QAction(tr("Resume"), ui->tblvwTaskMonit);
    acStopTask     = new QAction(tr("Stop"), ui->tblvwTaskMonit);

    connect(acWorkDir,      SIGNAL(triggered()), this, SLOT(showWorkDir()));
    connect(acShowTaskInfo, SIGNAL(triggered()), this, SLOT(displayTaskInfo()));
    connect(acPauseTask,    SIGNAL(triggered()), this, SLOT(pauseTask()));
    connect(acResumeTask,   SIGNAL(triggered()), this, SLOT(resumeTask()));
    connect(acStopTask,     SIGNAL(triggered()), this, SLOT(stopTask()));

    connect(ui->tblvwTaskMonit, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showTaskMonitContextMenu(const QPoint &)));

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
// SLOT: showTaskMonitContextMenu
//----------------------------------------------------------------------
void MainWindow::showTaskMonitContextMenu(const QPoint & p)
{
    QList<QAction *> actions;
    if (ui->tblvwTaskMonit->indexAt(p).isValid()) {
        actions.append(acWorkDir);
        actions.append(acShowTaskInfo);
        actions.append(acPauseTask);
        actions.append(acResumeTask);
        actions.append(acStopTask);
    }
    if (actions.count() > 0) {
        QMenu::exec(actions, ui->tblvwTaskMonit->mapToGlobal(p));
    }
}

//----------------------------------------------------------------------
// Method: showWorkDir
// Open file manager with task directory
//----------------------------------------------------------------------
void MainWindow::showWorkDir()
{
    QModelIndex idx = ui->tblvwTaskMonit->currentIndex();
/*
    QString treeKey = item->data(0, Qt::UserRole).toString();
    const Json::Value & v  = processedTasksInfo.value(treeKey);
    QString bind = QString::fromStdString(v["HostConfig"]["Binds"][0].asString());
    QString localDir = bind.left(bind.indexOf(":"));
    QString cmdLine = "nautilus " + localDir + " &";
    system(cmdLine.toStdString().c_str());
*/
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
    QString taskName = procTaskStatusModel->data(nameExtIdx).toString();
    QString taskInfoString = procTaskStatusModel->data(dataIdx).toString();

    showJSONdata("Task: " + taskName, taskInfoString);
}

//----------------------------------------------------------------------
// Method: pauseTask
// Pauses selected task
//----------------------------------------------------------------------
void MainWindow::pauseTask()
{
    runDockerCmd(ui->tblvwTaskMonit->currentIndex(), "pause");
}

//----------------------------------------------------------------------
// Method: resumeTask
// Resumes selected (paused) task
//----------------------------------------------------------------------
void MainWindow::resumeTask()
{
    runDockerCmd(ui->tblvwTaskMonit->currentIndex(), "unpause");
}

//----------------------------------------------------------------------
// Method: stopTask
// Stops selected task
//----------------------------------------------------------------------
void MainWindow::stopTask()
{
    runDockerCmd(ui->tblvwTaskMonit->currentIndex(), "stop");
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
    QString dId;
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
    if (v.size() > 0) {
//        if (v.isArray()) {
//        } else {
        for (Json::ValueIterator i = v.begin(); i != v.end(); ++i) {
            QTreeWidgetItem * chld = new QTreeWidgetItem(t);
            qDebug() << "Show node key";
            qDebug() << i.key().asString().c_str();
            chld->setData(0, Qt::DisplayRole, QString::fromStdString(i.key().asString()));
            qDebug() << "now dump children...";
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
    ui->tblvwAlerts->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tblvwSysAlerts->setContextMenuPolicy(Qt::CustomContextMenu);

    acShowAlert = new QAction(tr("Show alert information"), this);
    acAckAlert = new QAction(tr("Acknowledge alert"), this);
    connect(acShowAlert,     SIGNAL(triggered()), this, SLOT(showAlertInfo()));
    // connect(acAckAlert,     SIGNAL(triggered()), this, SLOT(alertAck()));

    connect(ui->tblvwAlerts, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showAlertsContextMenu(const QPoint &)));
    connect(ui->tblvwSysAlerts, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showAlertsContextMenu(const QPoint &)));

    ui->tblvwAlerts->setSortingEnabled(false);
    ui->tblvwSysAlerts->setSortingEnabled(false);
}

//----------------------------------------------------------------------
// SLOT: showAlertsContextMenu
//----------------------------------------------------------------------
void MainWindow::showAlertsContextMenu(const QPoint & p)
{
    QList<QAction *> actions;
    QTableView * tblvw = qobject_cast<QTableView*>(sender());
    if (tblvw->indexAt(p).isValid()) {
        actions.append(acShowAlert);
        actions.append(acAckAlert);
        if (tblvw == ui->tblvwSysAlerts) {
            connect(acShowAlert, SIGNAL(triggered()), this, SLOT(showSysAlertInfo()));
        } else {
            connect(acShowAlert, SIGNAL(triggered()), this, SLOT(showSysAlertInfo()));
        }
    }
    if (actions.count() > 0) {
        QMenu::exec(actions, tblvw->mapToGlobal(p));
    }
}

//----------------------------------------------------------------------
// Method: showAlertInfo
// Show dialog with alert information
//----------------------------------------------------------------------
void MainWindow::showAlertInfo(QTableView * tblvw)
{
    QModelIndex idx = tblvw->currentIndex();
    Alert alert = sysAlertModel->getAlertAt(idx);
    DlgAlert dlg;
    dlg.setAlert(alert);
    dlg.exec();
    disconnect(acShowAlert);
}

//----------------------------------------------------------------------
// Method: showSysAlertInfo
// Show dialog with system alert information
//----------------------------------------------------------------------
void MainWindow::showSysAlertInfo()
{
    showAlertInfo(ui->tblvwSysAlerts);
}

//----------------------------------------------------------------------
// Method: showProcAlertInfo
// Show dialog with system alert information
//----------------------------------------------------------------------
void MainWindow::showProcAlertInfo()
{
    showAlertInfo(ui->tblvwAlerts);
}

//======================================================================
// Transmissions View configuration
//======================================================================

//----------------------------------------------------------------------
// METHOD: initTxView
//----------------------------------------------------------------------
void MainWindow::initTxView()
{
    ui->tblvwTx->setContextMenuPolicy(Qt::CustomContextMenu);

    acShowMsgInfo = new QAction(tr("Display message content"), ui->tblvwTx);

    connect(acShowMsgInfo, SIGNAL(triggered()), this, SLOT(displayTxInfo()));

    connect(ui->tblvwTx, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showTxContextMenu(const QPoint &)));

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
// SLOT: showTxContextMenu
//----------------------------------------------------------------------
void MainWindow::showTxContextMenu(const QPoint & p)
{
    QList<QAction *> actions;
    if (ui->tblvwTx->indexAt(p).isValid()) {
        actions.append(acShowMsgInfo);

    }
    if (actions.count() > 0) {
        QMenu::exec(actions, ui->tblvwTx->mapToGlobal(p));
    }
}

//----------------------------------------------------------------------
// Method: displayTxInfo
// Show dialog with task information
//----------------------------------------------------------------------
void MainWindow::displayTxInfo()
{
//     QModelIndex idx = ui->tblvwTx->currentIndex();
//     QModelIndex nameExtIdx = ui->tblvwTx->model()->index(idx.row(), 3);
//     QString taskName = procTaskStatusModel->data(nameExtIdx).toString();
// /*
//     const Json::Value & v = processedTasksInfo.value(treeKey);
//     Json::StyledWriter jsonWriter;
//     QString taskName = QString::fromStdString(v["NameExtended"].asString());
//     QString taskInfoString = QString::fromStdString(jsonWriter.write(v));
// */
//     QString taskInfoString = "{}";
//     DlgShowTxInfo dlg;
//     dumpTxInfoToTree(taskName, Json::Value(), dlg.getTreeTxInfo());
//     dlg.setWindowTitle("Information for task" + taskName);
//     dlg.setTxInfo(taskInfoString);
//     dlg.exec();
    QModelIndex idx = ui->tblvwTx->currentIndex();
    QModelIndex fromIdx = ui->tblvwTx->model()->index(idx.row(), 2);
    QModelIndex toIdx = ui->tblvwTx->model()->index(idx.row(), 3);
    QModelIndex typeIdx = ui->tblvwTx->model()->index(idx.row(), 4);
    QModelIndex dataIdx = ui->tblvwTx->model()->index(idx.row(), 5);
    QString msgName = (txModel->data(fromIdx).toString() + "::[" +
                       txModel->data(typeIdx).toString() + "]::" +
                       txModel->data(toIdx).toString());
    QString contentString = procTaskStatusModel->data(dataIdx).toString();

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

    QVector<double> loadAvgs = QVector<double>::fromStdVector(LibComm::getLoadAvgs());

    if (numOfRows == 0) {
        for (auto & kv : taskAgentsInfo) {
            TaskAgentInfo * taInfo = kv.second;
            taInfo->total      = 0;
            taInfo->maxnum     = 3;
            taInfo->running    = 0;
            taInfo->waiting    = 0;
            taInfo->paused     = 0;
            taInfo->stopped    = 0;
            taInfo->failed     = 0;
            taInfo->finished   = 0;
            taInfo->load1min   = loadAvgs.at(0) * 100;
            taInfo->load5min   = loadAvgs.at(1) * 100;
            taInfo->load15min  = loadAvgs.at(2) * 100;
            taInfo->uptimesecs = 0;
        }
    }

    // 2. Count tasks
    ProcTaskStatusModel * mdl = qobject_cast<ProcTaskStatusModel *>(ui->tblvwTaskMonit->model());
    int currentNumOfRows = mdl->rowCount();
    if (currentNumOfRows > numOfRows) {
        for (int i = numOfRows; i < currentNumOfRows; ++i) {
            std::string agent = mdl->index(i, AgentsCol).data().toString().toStdString();
            if (taskAgentsInfo.find(agent) != taskAgentsInfo.end()) {
                TaskAgentInfo * taInfo = taskAgentsInfo.find(agent)->second;
                std::string status = mdl->index(i, StatusCol).data().toString().toStdString();
                TaskStatus st = TaskStatusValue[status];
                switch (st) {
                case TASK_RUNNING:
                    taInfo->running++;
                    break;
                case TASK_FINISHED:
                    taInfo->finished++;
                    break;
                case TASK_FAILED:
                    taInfo->failed++;
                    break;
                case TASK_SCHEDULED:
                    taInfo->waiting++;
                    break;
                case TASK_PAUSED:
                    taInfo->paused++;
                    break;
                case TASK_STOPPED:
                    taInfo->stopped++;
                    break;
                default:
                    break;
                }
                taInfo->total++;
            }
        }
        numOfRows = currentNumOfRows;
    }

    // 3. Update view
    for (auto & kv : taskAgentsInfo) {
        TaskAgentInfo * taInfo = kv.second;
        FrmAgentStatus * panel = 0;
        std::map<std::string, FrmAgentStatus*>::iterator it = taskAgentsInfoPanel.find(kv.first);
        if (it == taskAgentsInfoPanel.end()) {
             panel = new FrmAgentStatus(0);
             taskAgentsInfoPanel[kv.first] = panel;
             vlyFrmAgents->removeItem(spacerFrmAgents);
             vlyFrmAgents->addWidget(panel);
             vlyFrmAgents->addSpacerItem(spacerFrmAgents);
        } else {
             panel = it->second;
        }
        panel->updateInfo(*taInfo);
    }
}

}
