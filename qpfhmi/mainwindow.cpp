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

#include "log.h"
using LibComm::Log;
#include "tools.h"

#include "version.h"

#include "init.h"

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

#include <thread>

#include "logwatcher.h"
#include "progbardlg.h"
#include "dlgshowtaskinfo.h"
#include "configtool.h"
#include "dbbrowser.h"
#include "exttoolsdef.h"
#include "verbleveldlg.h"
#include "testrundlg.h"

#include <QProcess>


namespace QPF {

// Valid Manager states
static const int ERROR        = -1;
static const int OFF          =  0;
static const int INITIALISED  =  1;
static const int RUNNING      =  2;

static const int MessageDelay = 2000;
static const char * FixedWidthStyle = "font: 7pt \"Droid Sans Mono\";";

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent, Configuration * cfgHdl) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cfg(cfgHdl)
{
    // Set-up UI
    ui->setupUi(this);
    manualSetupUI();
    defineValidTransitions();

    // Read QPF Configuration
    readConfig();

    // Automatic start-up
    commandSystem();
}

//----------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------
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
    ui->tabpgEvtInj->setEnabled(false);
    ui->btnStopMultInDataEvt->hide();

    ui->actionActivate_Debug_Info->setEnabled(false);
    ui->chkDebugInfo->setEnabled(false);

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

    QFrame * frm = new QFrame(0);
    vlyFrmAgents = new QVBoxLayout;
    frm->setLayout(vlyFrmAgents);
    ui->scrollAreaAgents->setWidget(frm);
    spacerFrmAgents = new QSpacerItem(10, 10,
                                      QSizePolicy::Minimum,
                                      QSizePolicy::Expanding);
    vlyFrmAgents->addSpacerItem(spacerFrmAgents);

    simInData = new SimInData;

    // Create additional connections
    connect(ui->btnStart, SIGNAL(pressed()), this, SLOT(commandSystem()));
    connect(this, SIGNAL(goToOperational()), this, SLOT(transitToOperational()));

    connect(simInData, SIGNAL(simInDataSent(int)), this, SLOT(sentInData(int)));
    connect(simInData, SIGNAL(endOfInData()), this, SLOT(endOfInDataMsgs()));

    connect(ui->btnSelectInboxPath, SIGNAL(pressed()), this, SLOT(selectInboxPath()));
    connect(ui->btnProcessInbox, SIGNAL(pressed()), this, SLOT(processInbox()));

    connect(this, SIGNAL(stopSendingMessages()), simInData, SLOT(stopSendingInData()));

    connect(ui->btnStopMultInDataEvt, SIGNAL(pressed()), this, SLOT(stopSendingMultInData()));

    // Read settings and set title
    readSettings();
    setWindowTitle(tr("QLA Processing Framework"));
    setUnifiedTitleAndToolBarOnMac(true);
    ui->lblUptime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
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

    restartAct = new QAction(tr("&Restart"), this);
    //restartAct->setShortcuts(QKeySequence::Quit);
    restartAct->setStatusTip(tr("Restart the application"));
    connect(restartAct, SIGNAL(triggered()), this, SLOT(restart()));

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

    dbgInfoAct = new QAction(tr("Activate debug"), this);
    dbgInfoAct->setStatusTip(tr("Activate debug information"));
    dbgInfoAct->setCheckable(true);
    dbgInfoAct->setChecked(true);
    connect(dbgInfoAct, SIGNAL(toggled(bool)), this, SLOT(setDebugInfo(bool)));
    connect(dbgInfoAct, SIGNAL(toggled(bool)), ui->chkDebugInfo, SLOT(setChecked(bool)));
    connect(ui->chkDebugInfo, SIGNAL(toggled(bool)), dbgInfoAct, SLOT(setChecked(bool)));

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
    fileMenu->addSeparator();
    fileMenu->addAction(restartAct);
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
    sessionInfoMenu->addAction(dbgInfoAct);

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
// Method: readConfig
// Reads configuration file
//----------------------------------------------------------------------
void MainWindow::readConfig()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    putToSettings("lastCfgFile", QVariant(QString::fromStdString(cfgInfo.cfgFileName)));

    // Get the name of the different Task Agents
    taskAgentsInfo.clear();
    for (unsigned int i = 0; i < cfgInfo.peersCfg.size(); ++i) {
        const Peer & peer = cfgInfo.peersCfg.at(i);
        if (peer.type == "taskagent") {
            TaskAgentInfo * taInfo = new TaskAgentInfo;
            taInfo->name = peer.name;
            taInfo->client = peer.clientAddr;
            taInfo->server = peer.serverAddr;

            // Create task info structure for agent
            taskAgentsInfo[peer.name] = taInfo;
        }
    }

    for (auto & kv : taskAgentsInfo ) {
        qDebug() << kv.first.c_str()
                 << kv.second->name.c_str()
                 << kv.second->client.c_str()
                 << kv.second->server.c_str()
                 << kv.second->total << kv.second->running << kv.second->waiting
                 << kv.second->failed << kv.second->finished << kv.second->maxnum;
    }

    QString lastAccess = QDateTime::currentDateTime().toString("yyyyMMddTHHmmss");
    cfg->setLastAccess(lastAccess.toStdString());
    putToSettings("lastAccess", QVariant(lastAccess));

    // Modify HMI with the product datatypes obtained from configuration
    ui->edInboxPath->setText(QString(cfgInfo.storage.inbox.path.c_str()));
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
// Method: execTestRun
// Shows execution of test run confirmation dialog
//----------------------------------------------------------------------
void MainWindow::execTestRun()
{
    static QString testFileName("EUC_VIS_INFO_W-91100-2_@TIMESTAMP@Z.xml");
    static QString testData("<?xml version=\"1.0\"?>"
      "<ns1:DpdVisObservationFrame xmlns:ns1=\"http://euclid.esa.org/schema/dpd/vis\">"
      "<Header><ProductId>91100</ProductId><ProductType>SCIENCE</ProductType><Software"
      "Name>LE1</SoftwareName><SoftwareRelease>0.0.1</SoftwareRelease><PipelineRun>1</"
      "PipelineRun><ExitStatusCode>a</ExitStatusCode><DataModelVersion>0.0.1</DataMode"
      "lVersion><MinDataModelVersion>0.0.1</MinDataModelVersion><ScientificCustodian>V"
      "IS</ScientificCustodian><AccessRights><EuclidConsortiumRead>true</EuclidConsort"
      "iumRead><EuclidConsortiumWrite>false</EuclidConsortiumWrite><ScientificGroupRea"
      "d>true</ScientificGroupRead><ScientificGroupWrite>false</ScientificGroupWrite><"
      "/AccessRights><Curator><Name>Curator</Name></Curator><Creator>SOC</Creator><Cre"
      "ationDate>2020-07-07T15:18:11.356298</CreationDate></Header><Data><ExposureTime"
      "><Value>565.0</Value><Unit>s</Unit></ExposureTime><ImgType><Category>SCIENCE</C"
      "ategory><FirstType>STD</FirstType><SecondType>SKY</SecondType><ThirdType>WIDE</"
      "ThirdType><Technique>IMAGE</Technique></ImgType><ImgNumber>144</ImgNumber><Axis"
      "Number>2</AxisNumber><AxisLengths>2118 2066</AxisLengths><DataSize>16</DataSize"
      "><DataLength>4375788</DataLength><ImgStatistics><Min>0.0</Min><Max>0.0</Max><Me"
      "an>0.0</Mean><Stdev>0.0</Stdev><Median>0.0</Median><MinX>0</MinX><MinY>0</MinY>"
      "<MaxX>2118</MaxX><MaxY>2066</MaxY><NPix>4375788</NPix><XLower>51</XLower><YLowe"
      "r>0</YLower><XUpper>2048</XUpper><YUpper>2066</YUpper></ImgStatistics><Instrume"
      "nt><InstrumentName>VISIns</InstrumentName><TelescopeName>EUCLID</TelescopeName>"
      "</Instrument><ObservationDateTime><OBT>2020-07-07T15:18:11.356298</OBT><UTC>202"
      "0-07-07T15:18:11.356298Z</UTC><MJD>56923.0</MJD></ObservationDateTime><Observat"
      "ionSequence><FieldId>0</FieldId><ObservationId>91100</ObservationId><DitherObse"
      "rvation>10</DitherObservation><Exposure>1</Exposure><TotSequence>1</TotSequence"
      "><ObjectId>XE34409</ObjectId></ObservationSequence><InstrumentMode>Science</Ins"
      "trumentMode><ObservationMode>ScienceWide</ObservationMode><CompressionAlgorithm"
      ">CCSDS121</CompressionAlgorithm><FrameChecksum>1</FrameChecksum><CommandedFPAPo"
      "inting type=\"Center\"><RA>195.4258</RA><Dec>36.2739</Dec><Orientation>27.0</Or"
      "ientation></CommandedFPAPointing><ReconsFPAPointing type=\"Center\"><RA>195.425"
      "8</RA><Dec>36.2739</Dec><Orientation>27.0</Orientation></ReconsFPAPointing><Pre"
      "dictedOrbit><Position>1100000.0 960000.0 1300000.0</Position><Velocity>100.0 90"
      ".0 200.0</Velocity><SolarAspectAngle>1.6</SolarAspectAngle></PredictedOrbit><Re"
      "consOrbit><Position>1100000.0 960000.0 1300000.0</Position><Velocity>100.0 90.0"
      " 200.0</Velocity><SolarAspectAngle>1.6</SolarAspectAngle></ReconsOrbit><Readout"
      "><ReadoutModeMethod>NominalScience</ReadoutModeMethod><StartTime>2020-07-07T15:"
      "18:11.356298</StartTime><ParallelRegFrequency>0.5</ParallelRegFrequency><Serial"
      "RegFrequency>0.5</SerialRegFrequency></Readout><ShutterUnit><Status>CLOSED</Sta"
      "tus></ShutterUnit><CalibUnit><Status>false</Status></CalibUnit><ChargedInduced>"
      "<Status>false</Status><IntensityLevel>9.0</IntensityLevel></ChargedInduced><Fra"
      "meFitsFile format=\"le1.visRawImage\" version=\"0.1\"><DataContainer filestatus"
      "=\"ARCHIVED\"><FileName>EUC_VIS_RAW_W-91100-2_@TIMESTAMP@Z.fits</FileName><"
      "/DataContainer></FrameFitsFile><Parameters><SoftwareVersion>1.0.0</SoftwareVers"
      "ion><PipelineCodeVIS>1</PipelineCodeVIS><Status><Name>MOD</Name><Code>0</Code><"
      "ModuleSpecificCode>0</ModuleSpecificCode><SoftwareVersion>1.0</SoftwareVersion>"
      "</Status></Parameters></Data></ns1:DpdVisObservationFrame>");

    TestRunDlg dlg;
    if (!dlg.exec()) { return; }

    QString newFileName(testFileName);
    QString newData(testData);

    QString nowTimeStamp(QDateTime::currentDateTime().toString("yyyyMMddTHHmmss"));
    newFileName.replace("@TIMESTAMP@", nowTimeStamp);
    newData.replace("@TIMESTAMP@", nowTimeStamp);

    // Get config info
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    // Create input file
    QString runPath = QString::fromStdString(cfgInfo.storage.base);
    QString testInputPath = runPath + "/testRunInput";
    mkdir(qPrintable(testInputPath), 0777);

    // Clean test run folder
    QDirIterator it(testInputPath, QDirIterator::Subdirectories);

    QStringList illegalFileTypes;
    illegalFileTypes << "xml" << "sdc";

    while (it.hasNext()) {
        it.next();
        foreach (QString illegalType, illegalFileTypes) {
            if (it.fileInfo().absoluteFilePath().endsWith(illegalType, Qt::CaseInsensitive)) {
                QDir dir;
                dir.remove(it.filePath());
                break;
            }
        }
    }

    QString testInputFile = testInputPath + "/" + newFileName;
#define USE_STDSTRING
#undef  USE_STDSTRING
#ifdef USE_STDSTRING
    std::string filename(testInputFile.toStdString());
    std::ofstream dataFile;
    dataFile.open(filename);
    if (dataFile.good()) {
        dataFile << qPrintable(newData) << std::endl;
        dataFile.close();
    } else {
        std::cerr << "Cannot create file " + filename  << std::endl;
    }
#else
    QDomDocument document;
    document.setContent(newData);

    QFile file(testInputFile);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    document.save(out, 4);
    file.close();
#endif
    // Process folder with test data
    QString metadata;
    ui->spbxInjFreq->setValue(10);
    ui->edInboxPath->setText(testInputPath);

    statusBar()->showMessage(tr("Processing Test File ") + testInputFile,
                             MessageDelay);
    processInbox();
}

//----------------------------------------------------------------------
// Method: commandSystem
// Send SIG_USR1 signal to qpf, to trigger the START message to be sent,
// or send STOP message, depending on the current status
//----------------------------------------------------------------------
void MainWindow::commandSystem()
{
    static bool firstTime = true;
    static bool isStart = true;

    if (isStart) {

        if (firstTime) {
            // Non-GUI (communications) Initializations
            //            init();
            //            hmiNode->getStartSignal();
            firstTime = false;
        } else {
            qApp->quit();
            exit(0);
        }

        init();

        // System START signal
        std::ofstream fEvtMngGoAhead(EvtMngGoFile);
        fEvtMngGoAhead << '.';
        fEvtMngGoAhead.close();
        //pid_t parentPid = getppid();
        //kill(parentPid, SIGUSR1);
        setLogWatch();

        QString newCSS = ui->btnStart->styleSheet()
                .append("background-color: lightgreen;");
        ui->btnStart->setStyleSheet(newCSS);
        ui->btnStart->setText("STOP SYSTEM");
        ui->tabpgEvtInj->setEnabled(true);

        // Activate Archive Monitoring
//#define OLD_CODE
#ifdef OLD_CODE
        archHdl = new ArchiveModel(ui->tblvwArchive, ui->treevwArchive);
        archHdl->setupModel("products_info");
#else
        archHdl = new ArchiveModel();
        archHdl->setupModel(ui->tblvwArchive, "products_info");

        ArchiveModel * archTreeHdl = new ArchiveModel();
        QList<QString> headers = QList<QString>()
            << "Signature" << "Prod.Id" << "Prod.Type" << "Version"
            << "Size" << "Status" << "Creator" << "ObsMode"
            << "Start" << "End" << "RegTime" << "URL";
        archTreeHdl->setupModel(ui->treevwArchive,
                                "SELECT  "
                                "    concat(instruments.instrument, ':', products_info.signature) AS Signature,  "
                                "    products_info.product_id as Id,  "
                                "    products_info.product_type as Type,  "
                                "    products_info.product_version as Version,  "
                                "    products_info.product_size as Size,  "
                                "    product_status.status_desc as Status,  "
                                "    creators.creator_desc as Creator,  "
                                "    observation_modes.obsmode_desc as ObsMode,  "
                                "    products_info.start_time as Start,  "
                                "    products_info.end_time as End,  "
                                "    products_info.registration_time as RegTime,  "
                                "    products_info.url as URL "
                                "FROM products_info  "
                                "INNER JOIN instruments ON products_info.instrument_id = instruments.instrument_id  "
                                "INNER JOIN product_status ON products_info.product_status_id = product_status.product_status_id  "
                                "INNER JOIN creators ON products_info.creator_id = creators.creator_id  "
                                "INNER JOIN observation_modes ON products_info.obsmode_id = observation_modes.obsmode_id  "
                                "ORDER BY concat(instruments.instrument, '.',  "
                                "                products_info.signature, '.',  "
                                "                right(concat('00000000000000000000', products_info.ID), 20));",
                                headers);
#endif

        transitTo(INITIALISED);
        showState();
        statusBar()->showMessage(tr("START Signal sent to all elements . . ."),
                                 MessageDelay);

    } else {
        stopSendingMultInData();

        hmiNode->requestStop();

        ui->btnStart->setStyleSheet("");
        ui->btnStart->setText("QUIT");
        ui->tabpgEvtInj->setEnabled(false);

        transitTo(OFF);
        showState();
        statusBar()->showMessage(tr("STOP Signal sent to all elements . . ."),
                                 MessageDelay);
    }

    isStart = !isStart;
}

//----------------------------------------------------------------------
// Method: restart
// Confirme and perform restart of the application
//----------------------------------------------------------------------
void MainWindow::restart()
{
    statusBar()->showMessage(tr("Restart application?"), 2 * MessageDelay);
    int ret = QMessageBox::warning(this, tr("Restart " APP_NAME),
                                   tr("Do you really want to restart the application?"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No);

    if (ret != QMessageBox::Yes) { return; }

    qApp->exit( MainWindow::EXIT_CODE_RESTART );
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
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();
    hmiNode = new HMIProxy(cfgInfo.qpfhmiCfg.name.c_str());
    hmiNode->setSizeOfLogBuffer(0);

    qDebug() << hmiNode->getCommNodeName().c_str() << "started . . .";

    // CommNode connection to Event Manager
    hmiNode->addPeer(&(cfgInfo.qpfhmiCfg), true);
    hmiNode->addPeer(&(cfgInfo.evtMngCfg));

    // Add log tab
    QString qpfhmiLog = QString::fromStdString(Log::getLogBaseDir() + "/log/" +
                                               hmiNode->getCommNodeName() + ".log");
    TextView * pltxted = new TextView;
    pltxted->setStyleSheet(FixedWidthStyle);
    pltxted->setLogName(QString::fromStdString(hmiNode->getCommNodeName()));
    LogWatcher * newLog = new LogWatcher(pltxted);
    newLog->setFile(qpfhmiLog);
    nodeLogs.append(newLog);
    ui->mdiArea->addSubWindow(pltxted);

    // Initialize and create node part as a separate thread
    qDebug() << "Initializing QPFHMI...";
    hmiNode->initialize();
    qDebug() << "Trying to concurrentRun QPFHMI...";
    hmiPxyThread = std::thread(&HMIProxy::concurrentRun, hmiNode);
    //hmiResult = std::async(&HMIProxy::concurrentRun, hmiNode);
    qDebug() << "QPFHMI should be concurrentRunning...";

    // Propagate HMIProxy to SimInData object
    simInData->setHmiNode(hmiNode);

    ui->actionActivate_Debug_Info->setEnabled(true);
    ui->chkDebugInfo->setEnabled(true);
    ui->actionActivate_Debug_Info->setChecked(hmiNode->getDebugInfo());
}

//----------------------------------------------------------------------
// METHOD: initLogWatch
//----------------------------------------------------------------------
void MainWindow::initLogWatch()
{
    QList<QMdiSubWindow *> swList = ui->mdiArea->subWindowList();
    for (int i = swList.count() - 1; i >= 0; --i) {
        QWidget * w = (QWidget*)(swList.at(i));
        ui->mdiArea->removeSubWindow(w);
        delete w;
    }
    nodeLogs.clear();
}

//----------------------------------------------------------------------
// METHOD: setLogWatch
//----------------------------------------------------------------------
void MainWindow::setLogWatch()
{
    statusBar()->showMessage(tr("Establishing log monitoring capture..."),
                             MessageDelay);

    // Establish log monitoring for all components
    initLogWatch();

    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    QFileInfo fs;
    for (auto & kv : cfgInfo.machineNodes) {
        std::string machine(kv.first);
        bool masterMach = (machine == cfgInfo.masterMachine);
        std::string logsDir = Log::getLogBaseDir() + ((masterMach) ? "/log/" : "/rlog/");
        for (auto & n : kv.second) {
            fs.setFile(QString::fromStdString(logsDir + n + ".log"));
            QString logFileName = fs.absoluteFilePath();
            qDebug() << QString::fromStdString(machine)
                     << QString::fromStdString(cfgInfo.masterMachine)
                     << masterMach
                     << logFileName;
            if (!fs.exists()) {
                QFile file(logFileName);
                //file.resize(0);
                file.open(QIODevice::WriteOnly | QIODevice::Text);
                //QTextStream out(&file);
                //out << "";
                file.close();
            }
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
// Method: processInbox
// Generates INDATA messages into the system, to process all the
// products in the Inbox directory selected
//----------------------------------------------------------------------
void MainWindow::processInbox()
{
    QString metadata;
    simInData->setInjectionFrequency(ui->spbxInjFreq->value());
    bool ok = simInData->processInbox(ui->edInboxPath->text(), metadata);

    if (ok) {
        //ui->tabEventsToInject->setEnabled(false);
        ui->btnStopMultInDataEvt->show();

        ui->pltxtInboxProducts->setPlainText(metadata);

        // Activate task monitoring
        taskMonitTimer = new QTimer(this);
        connect(taskMonitTimer, SIGNAL(timeout()), this, SLOT(checkForTaskRes()));
        taskMonitTimer->start(1000);

        transitTo(RUNNING);
        showState();
    }
}

//----------------------------------------------------------------------
// Method: selectInboxPath
// Select directory where the incoming products are being stored
//----------------------------------------------------------------------
void MainWindow::selectInboxPath()
{
    QString currText = ui->edInboxPath->text();
    if (currText.isEmpty()) {
        currText = "/";
    }
    QString dirName =
            QFileDialog::getExistingDirectory(this,
                                              tr("Select incoming products folder"),
                                              currText,
                                              QFileDialog::ShowDirsOnly);
    if (dirName.isEmpty()) { return; }
    ui->edInboxPath->setText(dirName);
    inboxDirName = dirName;

    QString metadata;
    bool ok = simInData->processInbox(inboxDirName, metadata);
    if (ok) {
        ui->pltxtInboxProducts->setPlainText(metadata);
    } else {
        ui->pltxtInboxProducts->setPlainText(tr("ERROR: Problems when generating "
                                                "the inbox input files metadata"));
    }

}

//----------------------------------------------------------------------
// Method: sentInData
// Display a message everytime an INDATA message has been sent
//----------------------------------------------------------------------
void MainWindow::sentInData(int msgsLeft)
{
    QString msg = QString("Message INDATA sent to Event Manager (%1 left) ...")
            .arg(msgsLeft);
    statusBar()->showMessage(msg, MessageDelay);
}

//----------------------------------------------------------------------
// Method: endOfInDataMsgs
// Called when there is no more simulated INDATA messages to be sent
//----------------------------------------------------------------------
void MainWindow::endOfInDataMsgs()
{
    //ui->tabEventsToInject->setEnabled(true);
    ui->btnStopMultInDataEvt->hide();

    transitTo(INITIALISED);
    showState();
}

//----------------------------------------------------------------------
// Method: stopSendingMultInData
// Stop sending INDATA messages
//----------------------------------------------------------------------
void MainWindow::stopSendingMultInData()
{
    emit stopSendingMessages();
    //endOfInDataMsgs();
}

//----------------------------------------------------------------------
// Method: checkForTaskRes
// Checks if there is Task Res Info and gather any info available
//----------------------------------------------------------------------
void MainWindow::checkForTaskRes()
{
    qApp->processEvents();

    if (hmiNode->isThereTaskResInfo()) {
        std::map<std::string, Json::Value> newTaskResInfo;
        int numOfTaskResMsgs = hmiNode->getTaskResInfo(newTaskResInfo);
        statusBar()->showMessage(QString("Received information from %1 tasks")
                                 .arg(numOfTaskResMsgs),
                                 MessageDelay);
        // Transform to Qt datatypes
        std::map<std::string, Json::Value>::const_iterator i = newTaskResInfo.begin();
        while (i != newTaskResInfo.end()) {
            QString key = QString::fromStdString(i->first);
            Json::Value v = i->second;
            v["ZUpdatable"] = true;
            //qDebug() << "Adding TaskRes with key " << key;
            taskResInfo[key] = v;
            ++i;
        }
        showTaskRes();
    }
}

//----------------------------------------------------------------------
// METHOD: showTaskRes
// Shows any available Task Result Info data
//----------------------------------------------------------------------
void MainWindow::showTaskRes()
{
    static bool firstTime = true;
    int nCols = 8;

    // Process pending events from Qt events loop
    qApp->processEvents();

    if (firstTime) {
        // Set up context menu
        initTasksMonitTree(nCols);
        initAlertsTable();
        initArchiveTable();

        firstTime = false;
    }

    // Update Tasks Monitorization Tree
    updateTasksMonitTree(nCols);
    updateAlertsTree();

    //if (firstTime) {
    for (int i = 0; i < nCols; ++i) {
        ui->treeTaskMonit->resizeColumnToContents(i);
    }
    for (int i = 0; i < 5; ++i) {
        ui->treeAlerts->resizeColumnToContents(i);
    }
    // firstTime = false;
    //}

    // Now, update information for Task Agents Status Info.
    updateAgentsMonitPanel();

    // Activate sorting - we are done
    ui->treeTaskMonit->setSortingEnabled(true);

    // Update last update label
    ui->lblLastUpdate->setText(QDateTime::currentDateTime().toString());
}

//----------------------------------------------------------------------
// METHOD: initTasksMonitTree
//----------------------------------------------------------------------
void MainWindow::initTasksMonitTree(int nCols)
{
    static QStringList hdrLabels;
    static ProgressBarDelegate* progressBarDisplay = 0;

    hdrLabels << "Started at" << "Finished at"
              << "Task Name" << "Agent" << "Proc.Element" << "Status"
              << "Progress" << "Exit Code";
    progressBarDisplay = new ProgressBarDelegate(this);

    ui->treeTaskMonit->setContextMenuPolicy(Qt::CustomContextMenu);

    acWorkDir      = new QAction(tr("Open task working directory..."), ui->treeTaskMonit);
    acShowTaskInfo = new QAction(tr("Display task information"), ui->treeTaskMonit);
    acPauseTask    = new QAction(tr("Pause"), ui->treeTaskMonit);
    acResumeTask   = new QAction(tr("Resume"), ui->treeTaskMonit);
    acStopTask     = new QAction(tr("Stop"), ui->treeTaskMonit);

    connect(acWorkDir,      SIGNAL(triggered()), this, SLOT(showWorkDir()));
    connect(acShowTaskInfo, SIGNAL(triggered()), this, SLOT(displayTaskInfo()));
    connect(acPauseTask,    SIGNAL(triggered()), this, SLOT(pauseTask()));
    connect(acResumeTask,   SIGNAL(triggered()), this, SLOT(resumeTask()));
    connect(acStopTask,     SIGNAL(triggered()), this, SLOT(stopTask()));

    connect(ui->treeTaskMonit, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showTaskMonitContextMenu(const QPoint &)));

    connect(ui->treeTaskMonit->header(), SIGNAL(sectionClicked(int)),
            this, SLOT(sortTaskViewByColumn(int)));

    ui->treeTaskMonit->header()->setSectionsClickable(true);
    taskResItems.clear();

    ui->treeTaskMonit->clear();
    ui->treeTaskMonit->setSortingEnabled(false);
    ui->treeTaskMonit->setColumnCount(nCols);
    ui->treeTaskMonit->setHeaderLabels(hdrLabels);
    ui->treeTaskMonit->setItemDelegateForColumn(6, progressBarDisplay);
}

//----------------------------------------------------------------------
// METHOD: initAlertsTable
//----------------------------------------------------------------------
void MainWindow::initAlertsTable()
{
    static QStringList hdrLabels;
    hdrLabels << "Time Stamp" << "ID" << "Severity" << "Component" << "Description";

    ui->treeAlerts->setContextMenuPolicy(Qt::CustomContextMenu);

    acShowAlert = new QAction(tr("Show alert information"), ui->treeAlerts);
    acAckAlert = new QAction(tr("Acknowledge alert"), ui->treeAlerts);
    connect(acShowAlert,     SIGNAL(triggered()), this, SLOT(showAlertInfo()));
    // connect(acAckAlert,     SIGNAL(triggered()), this, SLOT(alertAck()));

    connect(ui->treeAlerts, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showAlertsContextMenu(const QPoint &)));

    ui->treeAlerts->header()->setSectionsClickable(true);
    alerts.clear();
    Alert sampleAlert;
    sampleAlert.timeStamp = QDateTime::currentDateTime();
    sampleAlert.id = "QPFHMI_1";
    sampleAlert.severity = "NO_ALERT";
    sampleAlert.component = "QPFHMI";
    sampleAlert.description = "This is just a sample alert";
    alerts.append(sampleAlert);

    ui->treeAlerts->clear();
    ui->treeAlerts->setSortingEnabled(false);
    ui->treeAlerts->setColumnCount(5);
    ui->treeAlerts->setHeaderLabels(hdrLabels);
}

//----------------------------------------------------------------------
// METHOD: initArchiveTable
//----------------------------------------------------------------------
void MainWindow::initArchiveTable()
{
    ui->tblvwArchive->setContextMenuPolicy(Qt::CustomContextMenu);

    //acArchiveShow    = new QAction(tr("Open task working directory..."), ui->tblvwArchive);
    acArchiveOpenExt = new QMenu(tr("Open with external tool..."), ui->tblvwArchive);
    QAction * acTool1 = new QAction(tr("GEdit"), acArchiveOpenExt);
    acTool1->setStatusTip(tr("Gnome editor"));
    acArchiveOpenExt->addAction(acTool1);
    //acArchiveOpenExtTools.append(acArchiveOpenExt->menu);

//    connect(acArchiveShow,    SIGNAL(triggered()), this, SLOT(showArchInfo()));
    connect(acTool1,          SIGNAL(triggered()), this, SLOT(runTool1()));

    connect(ui->tblvwArchive, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showArchiveTableContextMenu(const QPoint &)));
}

void MainWindow::runTool1()
{
    QModelIndex item = ui->tblvwArchive->currentIndex();
    QModelIndex urlItem = item.model()->index(item.row(), 11, item);
    QString fileName = urlItem.data().toString().replace("file://","");
    QProcess *process = new QProcess(this);
    QString program = "gedit";
    process->start(program, QStringList() << fileName);
}

//----------------------------------------------------------------------
// METHOD: updateTasksMonitTree
//----------------------------------------------------------------------
void MainWindow::updateTasksMonitTree(int nCols)
{
    QMap<QString, Json::Value>::const_iterator it = taskResInfo.constBegin();
    auto end = taskResInfo.constEnd();

    QTreeWidgetItem * treeItem = 0;
    QMap<QString, QTreeWidgetItem *>::iterator treeIt;
    bool itemExists = false;

    //qDebug() << "Processing " << taskResInfo.size() << " TaskRes chunks";

    for (;it != end; ++it) {

        Json::Value & v = const_cast<Json::Value &>(it.value());

        if (v["ZUpdatable"].asBool() == false) {
            //qDebug() << "    - Not updatable";
            continue;
        }

        QString name     = QString::fromStdString(v["NameExtended"].asString());
        QString ag       = QString::fromStdString(v["TaskAgent"].asString());
        QString pe       = QString::fromStdString(v["Args"][0].asString());
        QString start    = QString::fromStdString(v["State"]["StartedAt"].asString());
        QString finish   = QString::fromStdString(v["State"]["FinishedAt"].asString());
        QString progress = QString::fromStdString(v["State"]["Progress"].asString());
        QString exitCode = QString::fromStdString(v["State"]["ExitCode"].asString());

        QString tKey     = QString::fromStdString(v["Key"].asString());
        QString tRegKey  = QString::fromStdString(v["RegKey"].asString());

        TaskStatus st = static_cast<TaskStatus>(v["State"]["TaskStatus"].asInt());

        if (st != TASK_SCHEDULED) {
            bool running = v["State"]["Running"].asBool();
            bool paused = v["State"]["Paused"].asBool();
            st = (paused ? TASK_PAUSED :
                                  (running ? TASK_RUNNING : TASK_FINISHED));
            if (exitCode.toInt() > 0) { st = TASK_FAILED; }
        }

        QString status   = QString::fromStdString(TaskStatusName[st]);
        QString treeKey("");

        // Select item to modify, or create new item
        // Two cases: Key is empty or it is non-empty
        if (tKey.isEmpty()) {
            // 1. Still not running, is it already registered?
            treeIt = taskResItems.find(tRegKey);
            itemExists = (treeIt != taskResItems.end());
            if (itemExists) {
                // 1.1 Item found, so take that tree item
                treeItem = treeIt.value();
                treeKey = tRegKey;
            } else {
                // 1.2 Not found, initial registration
                treeItem = new QTreeWidgetItem(ui->treeTaskMonit);
                taskResItems.insert(tRegKey, treeItem);
                treeKey = tRegKey;
            }
        } else {
            // 2. Key is non empty, so it has been at least started,
            // so we look for it
            treeIt = taskResItems.find(tKey);
            itemExists = (treeIt != taskResItems.end());
            if (itemExists) {
                // 2.1 Great, it is an update of a previously existing task
                treeItem = treeIt.value();
                treeKey = tKey;
            } else {
                // 2.2 Oops, not found, so it should be registered with RegKey
                treeIt = taskResItems.find(tRegKey);
                itemExists = (treeIt != taskResItems.end());
                if (itemExists) {
                    // 2.2.1 Found, so it is an already registered task, that
                    // just started to run, so we take the existing treeItem,
                    // BUT change the map indexing key to the taskKey
                    treeItem = taskResItems.take(tRegKey);
                    taskResItems.insert(tKey, treeItem);
                    processedTasksInfo.insert(tKey, v);
                    treeKey = tKey;
                } else {
                    // 2.2.2 OOPS.  This should not happen!
                    qDebug() << "OOPS!";
                    continue;
                }
            }
        }

        //qDebug() << "    - " << tKey << "," << tRegKey << " => " << treeKey;

        //treeItem->setFlags(treeItem->flags() | Qt::ItemIsEditable);
        treeItem->setText(0, start);
        treeItem->setText(1, finish);
        treeItem->setText(2, name);
        treeItem->setText(3, ag);
        treeItem->setText(4, pe);
        treeItem->setText(5, status);
        treeItem->setText(6, progress);
        treeItem->setText(7, exitCode);

        treeItem->setData(0, Qt::UserRole, treeKey);
        //treeItem->setData(1, Qt::UserRole, inspectInfo);

        switch (st) {
        case TASK_RUNNING: // FG: blue
            for (int col = 0; col < nCols; ++col) {
                treeItem->setData(col, Qt::ForegroundRole, QColor(Qt::blue));
            }
            break;
        case TASK_FINISHED: // BG: green
            for (int col = 0; col < nCols; ++col) {
                treeItem->setData(col, Qt::ForegroundRole, QColor(Qt::darkGreen));
                treeItem->setData(col, Qt::BackgroundRole, QColor(Qt::green));
            }
            v["ZUpdatable"] = false;
            break;
        case TASK_FAILED: // BG: red
            for (int col = 0; col < nCols; ++col) {
                treeItem->setData(col, Qt::ForegroundRole, QColor(Qt::white));
                treeItem->setData(col, Qt::BackgroundRole, QColor(Qt::red));
            }
            v["ZUpdatable"] = false;
            break;
        case TASK_SCHEDULED: // FG: gray
            for (int col = 0; col < nCols; ++col) {
                treeItem->setData(col, Qt::ForegroundRole, QColor(Qt::gray));
            }
            break;
        case TASK_STOPPED: // BG: gray
            for (int col = 0; col < nCols; ++col) {
                treeItem->setData(col, Qt::ForegroundRole, QColor(Qt::black));
                treeItem->setData(col, Qt::BackgroundRole, QColor(Qt::darkGray));
            }
            v["ZUpdatable"] = false;
            break;
        case TASK_PAUSED: // FG: darkYellow
            for (int col = 0; col < nCols; ++col) {
                treeItem->setData(col, Qt::ForegroundRole, QColor(Qt::darkYellow));
            }
            break;
        default:
            break;
        }

        if (!itemExists) {
            ui->treeTaskMonit->addTopLevelItem(treeItem);
        }

//        SDC::FastWriter w;
//        qDebug() << progress << st << status << QString::fromStdString(w.write(v));
    }

}

//----------------------------------------------------------------------
// METHOD: updateAlertsTree
//----------------------------------------------------------------------
void MainWindow::updateAlertsTree()
{
    static int numOfAlerts = 0;

    if (numOfAlerts < alerts.count()) {

        for (int i = numOfAlerts; i < alerts.count(); ++i) {
            QTreeWidgetItem * treeItem = new QTreeWidgetItem(ui->treeAlerts);
            const Alert & alert = alerts.at(i);

            treeItem->setText(0, alert.timeStamp.toString("yyyy-MM-dd hh:mm:ss.zzz"));
            treeItem->setText(1, alert.id);
            treeItem->setText(2, alert.severity);
            treeItem->setText(3, alert.component);
            treeItem->setText(4, alert.description);
            ui->treeAlerts->addTopLevelItem(treeItem);
        }

        numOfAlerts = alerts.count();
    }
}

//----------------------------------------------------------------------
// METHOD: updateAgentsMonitPanel
//----------------------------------------------------------------------
void MainWindow::updateAgentsMonitPanel()
{
    QVector<double> loadAvgs = QVector<double>::fromStdVector(LibComm::getLoadAvgs());

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

    // 2. Count tasks
    for (int i = 0; i < ui->treeTaskMonit->topLevelItemCount(); ++i) {
        QTreeWidgetItem * treeItem = ui->treeTaskMonit->topLevelItem(i);

        std::string agent = treeItem->text(3).toStdString();

        if (taskAgentsInfo.find(agent) != taskAgentsInfo.end()) {
            TaskAgentInfo * taInfo = taskAgentsInfo.find(agent)->second;

            TaskStatus st = TaskStatusValue[treeItem->text(5).toStdString()];
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

    // Process pending events from Qt events loop
    qApp->processEvents();

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

//----------------------------------------------------------------------
// SLOT: sortTaskViewByColumn
//----------------------------------------------------------------------
void MainWindow::sortTaskViewByColumn(int c)
{
    ui->treeTaskMonit->sortByColumn(c);
}

//----------------------------------------------------------------------
// SLOT: showTaskMonitContextMenu
//----------------------------------------------------------------------
void MainWindow::showTaskMonitContextMenu(const QPoint & p)
{
    QModelIndex m = ui->treeTaskMonit->currentIndex();
    if (m.parent() != QModelIndex()) { return; }

    QList<QAction *> actions;
    if (ui->treeTaskMonit->indexAt(p).isValid()) {
        actions.append(acWorkDir);
        actions.append(acShowTaskInfo);
        actions.append(acPauseTask);
        actions.append(acResumeTask);
        actions.append(acStopTask);
    }
    if (actions.count() > 0) {
        QMenu::exec(actions, ui->treeTaskMonit->mapToGlobal(p));
    }
}

//----------------------------------------------------------------------
// SLOT: showAlertsContextMenu
//----------------------------------------------------------------------
void MainWindow::showAlertsContextMenu(const QPoint & p)
{
    QModelIndex m = ui->treeAlerts->currentIndex();
    if (m.parent() != QModelIndex()) { return; }

    QList<QAction *> actions;
    if (ui->treeAlerts->indexAt(p).isValid()) {
        actions.append(acShowAlert);
        actions.append(acAckAlert);
    }
    if (actions.count() > 0) {
        QMenu::exec(actions, ui->treeAlerts->mapToGlobal(p));
    }
}

//----------------------------------------------------------------------
// SLOT: showArchiveTableContextMenu
//----------------------------------------------------------------------
void MainWindow::showArchiveTableContextMenu(const QPoint & p)
{
    QModelIndex m = ui->tblvwArchive->currentIndex();
    if (m.parent() != QModelIndex()) { return; }

    QList<QAction *> actions;
    if (ui->tblvwArchive->indexAt(p).isValid()) {
        //actions.append(acArchiveShow);
        //actions.append(acArchiveOpenExtTools);
        QMenu menu(this);
        //menu.addAction(acArchiveShow);
        menu.addMenu(acArchiveOpenExt);
        menu.exec(ui->tblvwArchive->mapToGlobal(p));
    }
}

//----------------------------------------------------------------------
// Method: showWorkDir
// Open file manager with task directory
//----------------------------------------------------------------------
void MainWindow::showWorkDir()
{
    QTreeWidgetItem * item = ui->treeTaskMonit->currentItem();
    QString treeKey = item->data(0, Qt::UserRole).toString();
    const Json::Value & v  = processedTasksInfo.value(treeKey);
    QString bind = QString::fromStdString(v["HostConfig"]["Binds"][0].asString());
    QString localDir = bind.left(bind.indexOf(":"));
    QString cmdLine = "nautilus " + localDir + " &";
    system(cmdLine.toStdString().c_str());
}

//----------------------------------------------------------------------
// Method: displayTaskInfo
// Show dialog with task information
//----------------------------------------------------------------------
void MainWindow::displayTaskInfo()
{
    QTreeWidgetItem * item = ui->treeTaskMonit->currentItem();
    QString treeKey = item->data(0, Qt::UserRole).toString();
    const Json::Value & v = processedTasksInfo.value(treeKey);
    Json::StyledWriter jsonWriter;
    QString taskName = QString::fromStdString(v["NameExtended"].asString());
    QString taskInfoString = QString::fromStdString(jsonWriter.write(v));
    DlgShowTaskInfo dlg;
    dumpTaskInfoToTree(taskName, v, dlg.getTreeTaskInfo());
    dlg.setWindowTitle("Information for task" + taskName);
    dlg.setTaskInfo(taskInfoString);
    dlg.exec();
}

//----------------------------------------------------------------------
// Method: showAlertInfo
// Show dialog with alert information
//----------------------------------------------------------------------
void MainWindow::showAlertInfo()
{
    QModelIndex idx = ui->treeAlerts->currentIndex();
    Alert alert = alerts[idx.row()];
    DlgAlert dlg;
    dlg.setAlert(alert);
    dlg.exec();
}

//----------------------------------------------------------------------
// Method: showArchInfo
// Show information about archive items
//----------------------------------------------------------------------
void MainWindow::showArchInfo()
{
}

//----------------------------------------------------------------------
// Method: pauseTask
// Pauses selected task
//----------------------------------------------------------------------
void MainWindow::pauseTask()
{
    runDockerCmd(ui->treeTaskMonit->currentItem(), "pause");
}

//----------------------------------------------------------------------
// Method: resumeTask
// Resumes selected (paused) task
//----------------------------------------------------------------------
void MainWindow::resumeTask()
{
    runDockerCmd(ui->treeTaskMonit->currentItem(), "unpause");
}

//----------------------------------------------------------------------
// Method: stopTask
// Stops selected task
//----------------------------------------------------------------------
void MainWindow::stopTask()
{
    runDockerCmd(ui->treeTaskMonit->currentItem(), "stop");
}

//----------------------------------------------------------------------
// Method: runDockerCmd
// Run command on selected task
//----------------------------------------------------------------------
bool MainWindow::runDockerCmd(QTreeWidgetItem * item, QString cmd)
{
    QString treeKey = item->data(0, Qt::UserRole).toString();
    const Json::Value & v = processedTasksInfo.value(treeKey);
    QString dId = QString::fromStdString(v["NameExtended"].asString());

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

//----------------------------------------------------------------------
// Method: setDebugInfo
// Sets flag for output of debug information
//----------------------------------------------------------------------
void MainWindow::setDebugInfo(bool b)
{
    hmiNode->setDebugInfo(b);
}

//----------------------------------------------------------------------
// Method: clearLayout
// Delete (recursively) all items in a layout
//----------------------------------------------------------------------
void MainWindow::clearLayout(QLayout *layout)
{
    if (layout->count() < 1) { return; }

    QLayoutItem *item;
    while ((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
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

    defineValidTransition(ERROR,        OFF);
    defineValidTransition(OFF,          INITIALISED);
    defineValidTransition(INITIALISED,  RUNNING);
    defineValidTransition(INITIALISED,  OFF);
    defineValidTransition(INITIALISED,  ERROR);
    defineValidTransition(RUNNING,      OFF);
    defineValidTransition(RUNNING,      ERROR);
    defineValidTransition(RUNNING,      INITIALISED);

    setState(ERROR);
    transitTo(OFF);
    showState();
}

//----------------------------------------------------------------------
// Method: showState
// Define valid state transitions
//----------------------------------------------------------------------
void MainWindow::showState()
{
    const int & state = getState();
    std::string stateName = stateNames[state];
    ui->lblSysStatus->setText(QString::fromStdString(stateName));
    QString stys;
    switch (state) {
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
        stys = "QLabel { background-color : green; color : white; }";
        break;
    default:
        break;
    }
    ui->lblSysStatus->setStyleSheet(stys);
}

int const MainWindow::EXIT_CODE_RESTART = -9991;

}
