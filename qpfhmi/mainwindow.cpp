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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "log.h"
using LibComm::Log;

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include <QDebug>
#include <QPlainTextEdit>
#include <QDir>
#include <QtConcurrent/QtConcurrentRun>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

#include <thread>

#include "logwatcher.h"
#include "progbardlg.h"
#include "dlgshowtaskinfo.h"

namespace QPF {

static const int MessageDelay = 2000;
static const char * FixedWidthStyle = "font: 7pt \"Droid Sans Mono\";";

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timerMultInData(0)
{
    // Set-up UI
    ui->setupUi(this);

    ui->tabpgEvtInj->setEnabled(false);
    ui->btnStopMultInDataEvt->hide();
    multInDataContentValues.clear();

    ui->actionActivate_Debug_Info->setEnabled(false);

    // Read QPF Configuration
    readConfig();

    // Launch HMI Proxy
    //init();

    // Create additional connections
    connect(ui->btnStart, SIGNAL(pressed()), this, SLOT(commandSystem()));
    connect(this, SIGNAL(goToOperational()), this, SLOT(transitToOperational()));

    connect(ui->btnSendInDataEvt, SIGNAL(pressed()), this, SLOT(sendInData()));
    connect(ui->btnSendMultInDataEvt, SIGNAL(pressed()), this, SLOT(prepareSendMultInData()));
    connect(ui->btnSendMultInDataEvtFile, SIGNAL(pressed()), this, SLOT(prepareSendInDataFromFile()));
    connect(ui->btnSelectInDataParamFile, SIGNAL(pressed()), this, SLOT(selectInDataParamsFile()));

    connect(ui->btnStopMultInDataEvt, SIGNAL(pressed()), this, SLOT(stopSendingMultInData()));
}

//----------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}

//----------------------------------------------------------------------
// Method: readConfig
// Reads configuration file
//----------------------------------------------------------------------
void MainWindow::readConfig()
{
    cfg = new Configuration(0);
    cfg->getGeneralInfo(qpfCfg.appName, qpfCfg.appVersion, qpfCfg.lastAccess);
    cfg->getProductTypes(qpfCfg.productTypes);

    for (int i = 0; i < cfg->getNumProcs(); ++i) {
        ProcessingElement * pe = new ProcessingElement;
        cfg->getProc(pe->name, pe->exePath, pe->inPath, pe->outPath);
        qpfCfg.procElems[pe->name] = pe;
    }

    // Save HMI & EvtMng Peer configuration
    qpfCfg.evtMngCfg.name = "EvtMng";
    cfg->getNodeByName("EvtMng", qpfCfg.evtMngCfg.type,
                       qpfCfg.evtMngCfg.clientAddr, qpfCfg.evtMngCfg.serverAddr);

    qpfCfg.qpfhmiCfg.name = "QPFHMI";
    cfg->getNodeByName("QPFHMI", qpfCfg.qpfhmiCfg.type,
                       qpfCfg.qpfhmiCfg.clientAddr, qpfCfg.qpfhmiCfg.serverAddr);

    cfg->setLastAccess(QDateTime::currentDateTime()
                       .toString("yyyyMMddTHHmmss").toStdString());

    // Modify HMI with the product types obtained from configuration
    ui->cboxProdType->clear();
    for (unsigned int i = 0; i < qpfCfg.productTypes.size(); ++i) {
        ui->cboxProdType->addItem(QString::fromStdString(qpfCfg.productTypes.at(i)));
    }
    ui->lstProductTypes->clear();
    for (unsigned int i = 0; i < qpfCfg.productTypes.size(); ++i) {
        ui->lstProductTypes->addItem(QString::fromStdString(qpfCfg.productTypes.at(i)));
    }
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

    static const char evtmngGoFile[] = "/tmp/EventManager.GO.msg";

    if (isStart) {
        if (firstTime) {
            // Non-GUI (communications) Initializations
//            init();
//            hmiNode->getStartSignal();
            firstTime = false;
        } else {
            this->close();
            qApp->quit();
            exit(0);
        }

        // System START signal
        std::ofstream fEvtMngGoAhead(evtmngGoFile);
        fEvtMngGoAhead.close();
        //pid_t parentPid = getppid();
        //kill(parentPid, SIGUSR1);
        setLogWatch();

        init();

        QString newCSS = ui->btnStart->styleSheet()
                .append("background-color: lightgreen;");
        ui->btnStart->setStyleSheet(newCSS);
        ui->btnStart->setText("STOP SYSTEM");
        ui->tabpgEvtInj->setEnabled(true);

        // Activate Archive Monitoring
        archHdl = new ArchiveModel(ui->tblvwArchive);

        statusBar()->showMessage(tr("START Signal sent to all elements . . ."),
                                 MessageDelay);

    } else {
        stopSendingMessages();
        hmiNode->requestStop();

        ui->btnStart->setStyleSheet("");
        ui->btnStart->setText("QUIT");
        ui->tabpgEvtInj->setEnabled(false);

        statusBar()->showMessage(tr("STOP Signal sent to all elements . . ."),
                                 MessageDelay);
    }

    isStart = !isStart;
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
    hmiNode = new HMIProxy(qpfCfg.qpfhmiCfg.name.c_str());

    qDebug() << hmiNode->getCommNodeName().c_str() << "started . . .";

    // CommNode connection to Event Manager
    hmiNode->addPeer(&(qpfCfg.qpfhmiCfg), true);
    hmiNode->addPeer(&(qpfCfg.evtMngCfg));

    // Initialize and create node part as a separate thread
    qDebug() << "Initializing QPFHMI...";
    hmiNode->initialize();
    qDebug() << "Trying to concurrentRun QPFHMI...";
    hmiPxyThread = std::thread(&HMIProxy::concurrentRun, hmiNode);
    //hmiResult = std::async(&HMIProxy::concurrentRun, hmiNode);
    qDebug() << "QPFHMI should be concurrentRunning...";

    // Add log tab
    QString qpfhmiLog = QString::fromStdString(Log::getLogBaseDir() + "/" +
                                               hmiNode->getCommNodeName() + ".log");
    QPlainTextEdit * pltxted = new QPlainTextEdit;
    pltxted->setStyleSheet(FixedWidthStyle);
    LogWatcher * newLog = new LogWatcher(pltxted);
    newLog->setFile(qpfhmiLog);
    nodeLogs.append(newLog);
    ui->tabLogs->insertTab(0, pltxted,
                           QString::fromStdString(hmiNode->getCommNodeName()));

    ui->actionActivate_Debug_Info->setEnabled(true);
    ui->actionActivate_Debug_Info->setChecked(hmiNode->getDebugInfo());
}

//----------------------------------------------------------------------
// METHOD: initLogWatch
//----------------------------------------------------------------------
void MainWindow::initLogWatch()
{
    for (int i = ui->tabLogs->count() - 1; i >= 0; --i) {
        QWidget * w = ui->tabLogs->widget(i);
        ui->tabLogs->removeTab(i);
        delete w;
    }
    ui->tabLogs->clear();
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
    QString logsDir = QString::fromStdString(Log::getLogBaseDir());
    qDebug() << "Looking at dir " << logsDir;
    QDir msgCommDir(logsDir);
    msgCommDir.setFilter(QDir::Files);
    QStringList filters;
    filters << "*.log";
    msgCommDir.setNameFilters(filters);

    foreach (const QFileInfo & fs, msgCommDir.entryInfoList()) {
        QString logFileName = fs.absoluteFilePath();
        qDebug() << "Watching file " << logFileName;
        QPlainTextEdit * pltxted = new QPlainTextEdit;
        pltxted->setStyleSheet(FixedWidthStyle);
        LogWatcher * newLog = new LogWatcher(pltxted);
        newLog->setFile(logFileName);
        nodeLogs.append(newLog);
        if ((fs.baseName() == "EvtMng") || ((fs.baseName() == "QPFHMI"))) {
            ui->tabLogs->insertTab(0, pltxted, fs.baseName());
        } else {
            ui->tabLogs->addTab(pltxted, fs.baseName());
        }
    }
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
// Method: sendInDatapushButton
// Send sample InData message to Event Manager
//----------------------------------------------------------------------
void MainWindow::sendInData()
{
    QString prodType = ui->edProdId->text();
    QString instrument = prodType.left(prodType.indexOf("_"));
    ProductMetadata m;

    m.startTime      = ui->dtStart->text().toStdString();
    m.endTime        = ui->dtEnd->text().toStdString();
    m.instrument     = instrument.toStdString();
    m.productId      = prodType.toStdString();
    m.productType    = ui->cboxProdType->currentText().toStdString();
    m.productVersion = ui->edProdVer->text().toStdString();
    m.productStatus  = ui->cboxProdStatus->currentText().toStdString();
    m.productSize    = 12345678;
    m.url            = ui->edURL->text().toStdString();

    ui->tabpgLog->setFocus();

    qDebug() << "Trying to send InData";
    hmiNode->sendInData(m);
    qDebug() << " >> back from hmiNode->sendInData";
    statusBar()->showMessage(tr("Message INDATA sent to Event Manager..."),
                             MessageDelay);
}

//----------------------------------------------------------------------
// Method: prepareSendMultInData
// Prepare a set of sample InData messages to be sent to Event Manager
//----------------------------------------------------------------------
void MainWindow::prepareSendMultInData()
{
    // Avoid execution when a previous set of messages is
    // still being sent
    if (timerMultInData != 0) { return; }

    QDateTime startDateTime = ui->dtStartMult->dateTime();
    QDateTime lastDateTime = ui->dtEndMult->dateTime();
    QDateTime endDateTime = startDateTime;

    multInDataContentValues.clear();
    bool stillInPeriod = true;

    while (startDateTime < lastDateTime) {

        // Loop on Product Types
        for (int j = 0; (j < ui->lstProductTypes->count() && stillInPeriod); ++j) {

            QListWidgetItem * itj = ui->lstProductTypes->item(j);
            if (! itj->isSelected()) {
                ++j;
                continue;
            }
            QString prodType(itj->text());

            // Loop on Product Status
            for (int k = 0; (k < ui->lstProductStatus->count() && stillInPeriod); ++k) {

                QListWidgetItem * itk = ui->lstProductStatus->item(k);
                if (! itk->isSelected()) {
                    ++k;
                    continue;
                }
                QString prodStatus(itk->text());

                if (startDateTime < lastDateTime) {
                    // Compute endTime
                    endDateTime = startDateTime.addSecs(ui->cboxStep->currentText().toInt() * 3600 - 1);

                    // Build product id.
                    QString sDateTime(startDateTime.toString("yyyyMMddTHHmmss"));
                    QString eDateTime(endDateTime.toString("yyyyMMddTHHmmss"));
                    QString prodId = QString("%1_%2_%3-%4_%5")
                            .arg("EUCL").arg(prodType)
                            .arg(sDateTime).arg(eDateTime).arg("10");
                    QString instrument = prodType.left(prodType.indexOf("_"));

                    // Set new content for InData Message
                    ProductMetadata m;

                    m.startTime      = sDateTime.toStdString();
                    m.endTime        = eDateTime.toStdString();
                    m.instrument     = instrument.toStdString();
                    m.productId      = prodId.toStdString();
                    m.productType    = prodType.toStdString();
                    m.productVersion = "1";
                    m.productStatus  = prodStatus.toStdString();
                    m.productSize    = 12345678;
                    m.url            = "http://euclid.esa.int/data/" + prodId.toStdString() + ".zip";

                    // Append to InData message list
                    multInDataContentValues.append(m);

                    // Step startTime
                    startDateTime = endDateTime.addSecs(1);
                } else {
                    stillInPeriod = false;
                    break;
                }

            }

        }

        if (startDateTime == endDateTime) { return; }

    }

    if (multInDataContentValues.size() > 0) {
        // Prepare sending timer
        timerMultInData = new QTimer(this);
        connect(timerMultInData, SIGNAL(timeout()), this, SLOT(sendMultInData()));
        timerMultInData->start(ui->cboxFreq->currentText().toInt() * 1000);

        ui->tabEventsToInject->setEnabled(false);
        ui->btnStopMultInDataEvt->show();

        // Activate task monitoring
        taskMonitTimer = new QTimer(this);
        connect(taskMonitTimer, SIGNAL(timeout()), this, SLOT(checkForTaskRes()));
        taskMonitTimer->start(3333);
    }
}


//----------------------------------------------------------------------
// Method: selectInDataParamsFile
// Select user-defined InData parameters file
//----------------------------------------------------------------------
void MainWindow::selectInDataParamsFile()
{
    QString fileName =
      QFileDialog::getOpenFileName(this,
                                   tr("Select InData parameters file"),
                                   getenv("HOME"),
                                   QString("QPF InData Parameter files (*.lst)"));
    if (fileName.isEmpty()) { return; }
    ui->edInDataParamFile->setText(fileName);
    fileInDataParams = fileName;
}

//----------------------------------------------------------------------
// Method: prepareSendInDataFromFile
// Send a set of sample InData messages to Event Manager, according to
// a user-defined file
//----------------------------------------------------------------------
void MainWindow::prepareSendInDataFromFile()
{
    if (fileInDataParams.isEmpty()) { return; }

    QFile file(fileInDataParams);
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(" ");

            // Build product id.
            int     offset   = fields.at(0).toInt();
            QString sDateTime = fields.at(1);
            QString eDateTime = fields.at(1);
            QString prodType  = fields.at(3);
            QString productStatus = fields.at(4);;
            QString prodId    = QString("%1_%2_%3-%4_%5")
                    .arg("EUCL").arg(prodType)
                    .arg(sDateTime).arg(eDateTime).arg("10");
            QString instrument = prodType.left(prodType.indexOf("_"));

            // Set new content for InData Message
            ProductMetadata m;

            m.startTime      = sDateTime.toStdString();
            m.endTime        = eDateTime.toStdString();
            m.instrument     = instrument.toStdString();
            m.productId      = prodId.toStdString();
            m.productType    = prodType.toStdString();
            m.productVersion = "1.0";
            m.productStatus  = productStatus.toStdString();
            m.productSize    = 12345678;
            m.url            = ("http://euclid-data.esa.int/data/" + prodId + ".bin").toStdString();

            // Append to InData message list
            multInDataContentValues.append(m);

            // Append to InData message list
            QTimer::singleShot(offset * 1000, Qt::PreciseTimer, this, SLOT(sendInDataFromFile()));
        }
        file.close();

        if (multInDataContentValues.size() > 0) {
            ui->tabEventsToInject->setEnabled(false);
            ui->btnStopMultInDataEvt->show();
        }
    } else {
        QMessageBox::information(0, "Error reading input file",
                                 file.errorString());
    }
}

//----------------------------------------------------------------------
// Method: sendMultInData
// Send a set of sample InData messages to Event Manager
//----------------------------------------------------------------------
void MainWindow::sendMultInData()
{
    // Build and send message from the list
    hmiNode->sendInData(multInDataContentValues.takeFirst());

    QString msg = QString("Message INDATA sent to Event Manager (%1 left) ...")
            .arg(multInDataContentValues.count());
    statusBar()->showMessage(msg, MessageDelay);

    // If the list is empty, stop the timer and delete it
    if (multInDataContentValues.isEmpty()) {
        timerMultInData->stop();
        delete timerMultInData;
        timerMultInData = 0;
        ui->tabEventsToInject->setEnabled(true);
    }
}

//----------------------------------------------------------------------
// Method: sendInDataFromFile
// Send a set of sample InData messages to Event Manager, as specified
// in a used-created file
//----------------------------------------------------------------------
void MainWindow::sendInDataFromFile()
{
    // Build and send message from the list
    hmiNode->sendInData(multInDataContentValues.takeFirst());

    QString msg = QString("Message INDATA sent to Event Manager (%1 left) ...")
            .arg(multInDataContentValues.count());
    statusBar()->showMessage(msg, MessageDelay);

    // If the list is empty, stop the timer abreaknd delete it
    if (multInDataContentValues.isEmpty()) {
        ui->tabEventsToInject->setEnabled(true);
    }
}

//----------------------------------------------------------------------
// Method: stopSendingMultInData
// Stop sending INDATA messages
//----------------------------------------------------------------------
void MainWindow::stopSendingMultInData()
{
    stopSendingMessages();
    ui->btnStopMultInDataEvt->hide();
    ui->tabEventsToInject->setEnabled(true);
}

//----------------------------------------------------------------------
// Method: stopSendingMessages
// Stop any timed message transmission
//----------------------------------------------------------------------
void MainWindow::stopSendingMessages()
{
    multInDataContentValues.clear();
    if (timerMultInData != 0) {
        timerMultInData->stop();
        delete timerMultInData;
        timerMultInData = 0;
    }
}

//----------------------------------------------------------------------
// Method: checkForTaskRes
// Checks if there is Task Res Info and gather any info available
//----------------------------------------------------------------------
void MainWindow::checkForTaskRes()
{
    if (hmiNode->isThereTaskResInfo()) {
        std::map<std::string, Json::Value> newTaskResInfo;
        int numOfTaskResMsgs = hmiNode->getTaskResInfo(newTaskResInfo);
        statusBar()->showMessage(QString("Received information from %1 tasks")
                                 .arg(numOfTaskResMsgs),
                                 MessageDelay);
        // Transform to Qt types
        std::map<std::string, Json::Value>::const_iterator i = newTaskResInfo.begin();
        while (i != newTaskResInfo.end()) {
            QString key = QString::fromStdString(i->first);
            taskResInfo[key] = i->second;
            ++i;
        }
        showTaskRes();
    }
}

//----------------------------------------------------------------------
// Method: showTaskRes
// Shows any available Task Result Info data
//----------------------------------------------------------------------
void MainWindow::showTaskRes()
{
    static bool firstTime = true;

    if (firstTime) {
        // Set up context menu
        ui->treeTaskMonit->setContextMenuPolicy(Qt::CustomContextMenu);

        acWorkDir = new QAction(tr("Open task working directory..."), ui->treeTaskMonit);
        acDisplayTaskInfo = new QAction(tr("Display task information"), ui->treeTaskMonit);
        connect(acWorkDir, SIGNAL(triggered()), this, SLOT(showWorkDir()));
        connect(acDisplayTaskInfo, SIGNAL(triggered()), this, SLOT(displayTaskInfo()));
        //acAddPipeline = new QAction(tr("Add pipeline to queue"), ui->treeTaskmonit);
        //connect(acAddPipeline, SIGNAL(triggered()), this, SLOT(pipelineAddToQueue()));

        /*
        menuDatasets = new QMenu("Queue pipeline with dataset...", ui->treeTaskmonit);
        for (int i = 0; i < ui->lstDatasets->count(); ++i) {
          QAction * acDataset = new QAction(ui->lstDatasets->item(i)->text(), menuDatasets);
          connect(acDataset, SIGNAL(triggered()), this, SLOT(pipelineAddToQueueWithDataset()));
          menuDatasets->addAction(acDataset);
        }
        acAddPipeline = menuDatasets->menuAction();
        */
        connect(ui->treeTaskMonit, SIGNAL(customContextMenuRequested(const QPoint &)),
                this, SLOT(showTaskMonitContextMenu(const QPoint &)));

        QHeaderView * hdrView = ui->treeTaskMonit->header();
        hdrView->setSectionsClickable(true);
        connect(hdrView, SIGNAL(sectionClicked(int)),
                this, SLOT(sortTaskViewByColumn(int)));
        ui->treeTaskMonit->setSortingEnabled(false);

    }

    int nCols = 8;

    ui->treeTaskMonit->clear();
    ui->treeTaskMonit->setSortingEnabled(false);
    ui->treeTaskMonit->setColumnCount(nCols);
    QStringList hdrLabels;
    hdrLabels << "Started at" << "Finished at"
              << "Task Name" << "Agent" << "Proc.Element" << "Status"
              << "Progress" << "Exit Code";
    ui->treeTaskMonit->setHeaderLabels(hdrLabels);

    ui->treeTaskMonit->setItemDelegateForColumn(6, new ProgressBarDelegate(this));

    foreach (Json::Value v, taskResInfo) {
        Json::Value & v0 = v;

        QString name     = QString::fromStdString(v0["NameExtended"].asString());
        QString ag       = QString::fromStdString(v0["TaskAgent"].asString());
        QString pe       = QString::fromStdString(v0["Args"][0].asString());
        QString start    = QString::fromStdString(v0["State"]["StartedAt"].asString());
        QString finish   = QString::fromStdString(v0["State"]["FinishedAt"].asString());
        QString progress = QString::fromStdString(v0["State"]["Progress"].asString());
        QString exitCode = QString::fromStdString(v0["State"]["ExitCode"].asString());
        bool running = v0["State"]["Running"].asBool();
        bool paused = v0["State"]["Paused"].asBool();
        TaskStatus st = (paused ? TASK_PAUSED :
                                  (running ? TASK_RUNNING : TASK_FINISHED));
        if (exitCode.toInt() > 0) { st = TASK_FAILED; }
        QString status   = QString::fromStdString(TaskStatusName[st]);

        QTreeWidgetItem * treeItem = new QTreeWidgetItem(ui->treeTaskMonit);
        //treeItem->setFlags(treeItem->flags() | Qt::ItemIsEditable);
        treeItem->setText(0, start);
        treeItem->setText(1, finish);
        treeItem->setText(2, name);
        treeItem->setText(3, ag);
        treeItem->setText(4, pe);
        treeItem->setText(5, status);
        treeItem->setText(6, progress);
        treeItem->setText(7, exitCode);

        switch (st) {
        case TASK_RUNNING:
            for (int col = 0; col < nCols; ++col) {
                treeItem->setData(col, Qt::ForegroundRole, QColor(Qt::blue));
            }
            break;
        case TASK_FINISHED:
            for (int col = 0; col < nCols; ++col) {
                treeItem->setData(col, Qt::BackgroundRole, QColor(Qt::green));
            }
            break;
        case TASK_FAILED:
            for (int col = 0; col < nCols; ++col) {
                treeItem->setData(col, Qt::BackgroundRole, QColor(Qt::red));
            }
            break;
        case TASK_PAUSED:
            for (int col = 0; col < nCols; ++col) {
                treeItem->setData(col, Qt::ForegroundRole, QColor(Qt::gray));
            }
            break;
        default:
            break;
        }

        ui->treeTaskMonit->addTopLevelItem(treeItem);
    }

    if (firstTime) {
        firstTime = false;
        for (int i = 0; i < nCols; ++i) {
            ui->treeTaskMonit->resizeColumnToContents(i);
        }
    }

    ui->treeTaskMonit->setSortingEnabled(true);

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
        actions.append(acDisplayTaskInfo);
    }
    if (actions.count() > 0) {
        QMenu::exec(actions, ui->treeTaskMonit->mapToGlobal(p));
    }
}

//----------------------------------------------------------------------
// Method: showWorkDir
// Open file manager with task directory
//----------------------------------------------------------------------
void MainWindow::showWorkDir()
{
    QString taskId = ui->treeTaskMonit->currentItem()->data(0, Qt::DisplayRole).toString();
    Json::Value v = taskResInfo.value(taskId);
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
    QString taskId = ui->treeTaskMonit->currentItem()->data(0, Qt::DisplayRole).toString();
    Json::Value v = taskResInfo.value(taskId);
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
// Method: displayTaskInfo
// Show dialog with task information
//----------------------------------------------------------------------
void MainWindow::dumpTaskInfoToTree(QString taskName, Json::Value & v, QTreeWidget * t)
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
// Show dialog with task information
//----------------------------------------------------------------------
void MainWindow::dumpToTree(Json::Value & v, QTreeWidgetItem * t)
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


}
