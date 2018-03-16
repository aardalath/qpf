/******************************************************************************
 * File:    acthdl.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.qpfhmi.acthdl
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
 *   Implement ActionHandler class
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

#include "acthdl.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

using Configuration::cfg;

namespace QPF {

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
ActionHandler::ActionHandler(QWidget *parent) 
{
    mw = qobject_cast<MainWindow*>(parent);
}

//----------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------
ActionHandler::~ActionHandler()
{
}

//--- Accessor to acQuit
QAction * ActionHandler::getAcQuit() { return acQuit; }

//--- Accessor to acQuitAll
QAction * ActionHandler::getAcQuitAll() { return acQuitAll; }

//--- Accessor to acUserTools
QMap<QString, QAction *> & ActionHandler::getAcUserTools() { return acUserTools; }

//--- Accessor to acReprocess
QAction * ActionHandler::getAcReprocess() { return acReprocess; }

//----------------------------------------------------------------------
// Method: updateMenus
// Reads configuration file
//----------------------------------------------------------------------
void ActionHandler::updateMenus()
{
    bool hasTextView = (mw->activeTextView() != 0);

    acSaveAs->setEnabled(hasTextView);
#ifndef QT_NO_CLIPBOARD
    acPaste->setEnabled(hasTextView);
#endif
    acClose->setEnabled(hasTextView);
    acCloseAll->setEnabled(hasTextView);
    acTile->setEnabled(hasTextView);
    acCascade->setEnabled(hasTextView);
    acNext->setEnabled(hasTextView);
    acPrevious->setEnabled(hasTextView);
    acSeparator->setVisible(hasTextView);

#ifndef QT_NO_CLIPBOARD
    bool hasSelection = (mw->activeTextView() &&
                         mw->activeTextView()->getTextEditor()->textCursor().hasSelection());
    acCut->setEnabled(hasSelection);
    acCopy->setEnabled(hasSelection);
#endif
}

//----------------------------------------------------------------------
// Method: updateWindowMenu
// Updates the Window menu according to the current set of logs displayed
//----------------------------------------------------------------------
void ActionHandler::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(acNavig);
    windowMenu->addSeparator();
    windowMenu->addAction(acClose);
    windowMenu->addAction(acCloseAll);
    windowMenu->addSeparator();
    windowMenu->addAction(acTile);
    windowMenu->addAction(acCascade);
    windowMenu->addSeparator();
    windowMenu->addAction(acNext);
    windowMenu->addAction(acPrevious);
    windowMenu->addAction(acSeparator);

    QList<QMdiSubWindow *> windows = mw->ui->mdiArea->subWindowList();
    acSeparator->setVisible(!windows.isEmpty());

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
        action ->setChecked(child == mw->activeTextView());
        connect(action, SIGNAL(triggered()), mw->windowMapper, SLOT(map()));
        mw->windowMapper->setMapping(action, windows.at(i));
    }

}

//----------------------------------------------------------------------
// Method: createActions
// Create actions to be included in acthdl menus
//----------------------------------------------------------------------
void ActionHandler::createActions()
{
    // File menu
    acSaveAs = new QAction(tr("Save &As..."), this);
    acSaveAs->setShortcuts(QKeySequence::SaveAs);
    acSaveAs->setStatusTip(tr("Save the document under a new name"));
    connect(acSaveAs, SIGNAL(triggered()), mw, SLOT(saveAs()));

    acProcessPath = new QAction(QIcon(":/img/img/orchestration.png"), tr("Pr&ocess products in folder..."), this);
    acProcessPath->setShortcuts(QKeySequence::Open);
    acProcessPath->setStatusTip(tr("Specify a user selected folder and process all products inside"));
    connect(acProcessPath, SIGNAL(triggered()), mw, SLOT(processPath()));

//    acRestart = new QAction(tr("&Restart"), this);
//    //acRestart->setShortcuts(QKeySequence::Quit);
//    acRestart->setStatusTip(tr("Restart the application"));
//    connect(acRestart, SIGNAL(triggered()), mw, SLOT(restart()));

    acQuit = new QAction(QIcon(":/img/close.png"), tr("Close HMI"), this);
    //acQuit->setShortcuts(QKeySequence::Close);
    acQuit->setStatusTip(tr("Quit the QPF HMI application"));
    connect(acQuit, SIGNAL(triggered()), mw, SLOT(quitApp()));
 
    acQuitAll = new QAction(QIcon(":/img/quit.png"), tr("Quit all"), this);
    //acQuitAll->setShortcuts(QKeySequence::Quit);
    acQuitAll->setStatusTip(tr("Quit the QLA Processing Framework"));
    connect(acQuitAll, SIGNAL(triggered()), mw, SLOT(quitAllQPF()));
 
    // Edit menu
#ifndef QT_NO_CLIPBOARD
    acCut = new QAction(QIcon(":/img/cut.png"), tr("Cu&t"), this);
    acCut->setShortcuts(QKeySequence::Cut);
    acCut->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(acCut, SIGNAL(triggered()), mw, SLOT(cut()));

    acCopy = new QAction(QIcon(":/img/copy.png"), tr("&Copy"), this);
    acCopy->setShortcuts(QKeySequence::Copy);
    acCopy->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(acCopy, SIGNAL(triggered()), mw, SLOT(copy()));

    acPaste = new QAction(QIcon(":/img/paste.png"), tr("&Paste"), this);
    acPaste->setShortcuts(QKeySequence::Paste);
    acPaste->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(acPaste, SIGNAL(triggered()), mw, SLOT(paste()));
#endif

    // Tools menu
    acConfigTool = new QAction(QIcon(":/img/img/general.png"), tr("&Configuration Tool ..."), this);
    acConfigTool->setStatusTip(tr("Open Configuration Tool with current configuration"));
    connect(acConfigTool, SIGNAL(triggered()), mw, SLOT(showConfigTool()));

    acBrowseDB = new QAction(QIcon(":/img/img/storage.png"), tr("&Browse System DB ..."), this);
    acBrowseDB->setStatusTip(tr("Open System Database Browser"));
    connect(acBrowseDB, SIGNAL(triggered()), mw, SLOT(showDBBrowser()));

    acExtTools = new QAction(QIcon(":/img/img/exttools.png"), tr("&Define External Tools ..."), this);
    acExtTools->setStatusTip(tr("Define external tools to open data products"));
    connect(acExtTools, SIGNAL(triggered()), mw, SLOT(showExtToolsDef()));

    acVerbosity = new QAction(QIcon(":/img/img/verblevel.png"), tr("&Define Verbosity Level ..."), this);
    acVerbosity->setStatusTip(tr("Define verbosity level to be used in this session"));
    connect(acVerbosity, SIGNAL(triggered()), mw, SLOT(showVerbLevel()));

    acExecTestRun = new QAction(tr("&Execute test run ..."), this);
    acExecTestRun->setStatusTip(tr("Execute a test run processing on dummy data"));
    //connect(acExecTestRun, SIGNAL(triggered()), mw, SLOT(execTestRun()));

    // Window menu
    acNavig = new QAction(tr("Show &navigator panel"), this);
    acNavig->setStatusTip(tr("Shows or hides the navigator panel"));
    acNavig->setCheckable(true);
    connect(acNavig, SIGNAL(toggled(bool)),
            mw->ui->dockNavigator, SLOT(setVisible(bool)));
    connect(mw->ui->dockNavigator, SIGNAL(visibilityChanged(bool)),
            acNavig, SLOT(setChecked(bool)));
    mw->ui->dockNavigator->setVisible(false);

    acClose = new QAction(tr("Cl&ose"), this);
    acClose->setStatusTip(tr("Close the active window"));
    connect(acClose, SIGNAL(triggered()),
            mw->ui->mdiArea, SLOT(closeActiveSubWindow()));

    acCloseAll = new QAction(tr("Close &All"), this);
    acCloseAll->setStatusTip(tr("Close all the windows"));
    connect(acCloseAll, SIGNAL(triggered()),
            mw->ui->mdiArea, SLOT(closeAllSubWindows()));

    acTile = new QAction(QIcon(":/img/tile.png"), tr("&Tile"), this);
    acTile->setStatusTip(tr("Tile the windows"));
    connect(acTile, SIGNAL(triggered()), mw->ui->mdiArea, SLOT(tileSubWindows()));

    acCascade = new QAction(QIcon(":/img/cascade.png"), tr("&Cascade"), this);
    acCascade->setStatusTip(tr("Cascade the windows"));
    connect(acCascade, SIGNAL(triggered()), mw->ui->mdiArea, SLOT(cascadeSubWindows()));

    acNext = new QAction(tr("Ne&xt"), this);
    acNext->setShortcuts(QKeySequence::NextChild);
    acNext->setStatusTip(tr("Move the focus to the next window"));
    connect(acNext, SIGNAL(triggered()),
            mw->ui->mdiArea, SLOT(activateNextSubWindow()));

    acPrevious = new QAction(tr("Pre&vious"), this);
    acPrevious->setShortcuts(QKeySequence::PreviousChild);
    acPrevious->setStatusTip(tr("Move the focus to the previous "
                                 "window"));
    connect(acPrevious, SIGNAL(triggered()),
            mw->ui->mdiArea, SLOT(activatePreviousSubWindow()));

    acSeparator = new QAction(this);
    acSeparator->setSeparator(true);

    // Help menu
    acAbout = new QAction(tr("&About"), this);
    acAbout->setStatusTip(tr("Show the application's About box"));
    connect(acAbout, SIGNAL(triggered()), mw, SLOT(about()));

    acAboutQt = new QAction(tr("About &Qt"), this);
    acAboutQt->setStatusTip(tr("Show the Qt library's About box"));
    connect(acAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // Tab-related actions
    acTabClose = new QAction(tr("&Close"), this);
    acTabClose->setStatusTip(tr("Close this tab"));
    connect(acTabClose, SIGNAL(triggered()), this, SLOT(closeTabAction()));

    acTabCloseAll = new QAction(tr("Close all"), this);
    acTabCloseAll->setStatusTip(tr("Close all tabs"));
    connect(acTabCloseAll, SIGNAL(triggered()), this, SLOT(closeAllTabAction()));

    acTabCloseOther = new QAction(tr("Close &other"), this);
    acTabCloseOther->setStatusTip(tr("Close all other tabs"));
    connect(acTabCloseOther, SIGNAL(triggered()), this, SLOT(closeOtherTabAction()));

}

//----------------------------------------------------------------------
// Method: createMenus
// Create acthdl menus
//----------------------------------------------------------------------
void ActionHandler::createMenus()
{
    // File menu
    fileMenu = mw->menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(acProcessPath);
    fileMenu->addSeparator();
//    fileMenu->addAction(acSaveAs);
//    fileMenu->addSeparator();
//    fileMenu->addAction(acRestart);
//    fileMenu->addSeparator();
//    QAction *action = fileMenu->addAction(tr("Switch layout direction"));
//    connect(action, SIGNAL(triggered()), mw, SLOT(switchLayoutDirection()));
    fileMenu->addAction(acQuit);
    fileMenu->addAction(acQuitAll);

    // Edit menu
    editMenu = mw->menuBar()->addMenu(tr("&Edit"));
#ifndef QT_NO_CLIPBOARD
    editMenu->addAction(acCut);
    editMenu->addAction(acCopy);
    editMenu->addAction(acPaste);
#endif

    // Tools menu
    toolsMenu = mw->menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(acConfigTool);
    toolsMenu->addAction(acBrowseDB);
    toolsMenu->addAction(acExtTools);
    // toolsMenu->addSeparator();
    // sessionInfoMenu = toolsMenu->addMenu(tr("&Session Information"));
    // sessionInfoMenu->addAction(acVerbosity);
    toolsMenu->addAction(acVerbosity);

    //toolsMenu->addSeparator();
    //toolsMenu->addAction(acExecTestRun);

    // Window menu
    windowMenu = mw->menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    mw->menuBar()->addSeparator();

    // Help menu
    helpMenu = mw->menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(acAbout);
    helpMenu->addAction(acAboutQt);
}

//----------------------------------------------------------------------
// Method: createToolBars
// Create mainwindow tool bar components
//----------------------------------------------------------------------
void ActionHandler::createToolBars()
{
    delete mw->ui->mainToolBar;
    
    fileToolBar = mw->addToolBar(tr("File"));
    fileToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    fileToolBar->addAction(acProcessPath); acProcessPath->setIconText("Process folder files");
    fileToolBar->addSeparator();
    fileToolBar->addAction(acQuit);        acQuit->setIconText("");
    fileToolBar->addAction(acQuitAll);     acQuitAll->setIconText("");

    panelsToolBar = mw->addToolBar(tr("Panels"));
    panelsToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QAction * ac1 = new QAction(QIcon(":/img/logs.png"), tr("Logs"), this);    
    //QAction * ac2 = new QAction(QIcon(":/img/messages.png"), tr("Messages"), this);
    QAction * ac3 = new QAction(QIcon(":/img/monit.png"), tr("Tasks"), this);   
    QAction * ac4 = new QAction(QIcon(":/img/storage2.png"), tr("L.Archive"), this);
    QAction * ac5 = new QAction(QIcon(":/img/alerts.png"), tr("Alerts"), this);
    for (auto & ac: {ac1, ac3, ac4, ac5}) { panelsToolBar->addAction(ac); }
    
    connect(ac1, &QAction::triggered, [=]() { mw->ui->tabMainWgd->setCurrentIndex(0); });
    //connect(ac2, &QAction::triggered, [=]() { mw->ui->tabMainWgd->setCurrentIndex(1); });
    connect(ac3, &QAction::triggered, [=]() { mw->ui->tabMainWgd->setCurrentIndex(1); });
    connect(ac4, &QAction::triggered, [=]() { mw->ui->tabMainWgd->setCurrentIndex(2); });
    connect(ac5, &QAction::triggered, [=]() { mw->ui->tabMainWgd->setCurrentIndex(3); });

    toolsToolBar = mw->addToolBar(tr("Tools"));
    toolsToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolsToolBar->addAction(acConfigTool); acConfigTool->setIconText("Config.Tool"); 
    toolsToolBar->addAction(acBrowseDB);   acBrowseDB->setIconText("DB");   
    toolsToolBar->addAction(acExtTools);   acExtTools->setIconText("User Tools");   
    toolsToolBar->addAction(acVerbosity);  acVerbosity->setIconText("Verbosity");
}

//----------------------------------------------------------------------
// Method: createStatusBar
// Shows final Ready message in status bar
//----------------------------------------------------------------------
void ActionHandler::createStatusBar()
{
    mw->statusBar()->showMessage(tr("Ready"));
}

//----------------------------------------------------------------------
// Method: createTextView
// Create new subwindow
//----------------------------------------------------------------------
void ActionHandler::activateClipboardFor(TextView * child)
{
    connect(child, SIGNAL(copyAvailable(bool)), acCut, SLOT(setEnabled(bool)));
    connect(child, SIGNAL(copyAvailable(bool)), acCopy, SLOT(setEnabled(bool)));
}
    
//----------------------------------------------------------------------
// SLOT: closeTab
//----------------------------------------------------------------------
void ActionHandler::closeTab(int n)
{
    mw->removeRowInNav(n);
    delete mw->ui->tabMainWgd->widget(n);
}

//----------------------------------------------------------------------
// SLOT: closeTabAction
// Close the selected tab
//----------------------------------------------------------------------
void ActionHandler::closeTabAction()
{
    static const int NumOfFixedTabs = 5;
    int nTab;
    if (isMenuForTabWidget) {
        nTab = mw->ui->tabMainWgd->tabBar()->tabAt(menuPt);
    } else {
        nTab = mw->ui->lstwdgNav->currentRow();
    }
    if (nTab >= NumOfFixedTabs) {
        closeTab(nTab);
    }
}

//----------------------------------------------------------------------
// SLOT: closeAllTabAction
// Close all tabs
//----------------------------------------------------------------------
void ActionHandler::closeAllTabAction()
{
    static const int NumOfFixedTabs = 5;
    for (int i = mw->ui->lstwdgNav->count() - 1; i >= NumOfFixedTabs; --i) {
        closeTab(i);
    }
}

//----------------------------------------------------------------------
// SLOT: closeOtherTabAction
// Close all but the selected tab
//----------------------------------------------------------------------
void ActionHandler::closeOtherTabAction()
{
    static const int NumOfFixedTabs = 5;
    int nTab;
    if (isMenuForTabWidget) {
        nTab = mw->ui->tabMainWgd->tabBar()->tabAt(menuPt);
    } else {
        nTab = mw->ui->lstwdgNav->currentRow();
    }
    for (int i = mw->ui->lstwdgNav->count() - 1; i >= NumOfFixedTabs; --i) {
        if (i != nTab) {
            closeTab(i);
        }
    }
}

//----------------------------------------------------------------------
// METHOD: initLocalArchiveView
//----------------------------------------------------------------------
void ActionHandler::createLocalArchiveViewActions()
{
    // Create pop-up menu with user defined tools
    mw->ui->treevwArchive->setContextMenuPolicy(Qt::CustomContextMenu);
    mw->ui->treevwArchive->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(mw->ui->treevwArchive, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showArchiveTableContextMenu(const QPoint &)));

    connect(mw->ui->treevwArchive, SIGNAL(doubleClicked(QModelIndex)),
            mw, SLOT(openLocalArchiveElement(QModelIndex)));

    acArchiveOpenExt = new QMenu(tr("Open with ..."), mw->ui->treevwArchive);

    acArchiveShow = new QAction("Show location in local archive", mw->ui->treevwArchive);
    connect(acArchiveShow, SIGNAL(triggered()), mw, SLOT(openLocation()));

    acDefault = new QAction("System Default", mw->ui->treevwArchive);
    connect(acDefault, SIGNAL(triggered()), mw, SLOT(openWithDefault()));

    acReprocess = new QAction("Reprocess data product", mw->ui->treevwArchive);
    connect(acReprocess, SIGNAL(triggered()), mw, SLOT(reprocessProduct()));

    acAnalyzeIPython = new QAction("Analyze within IPython", mw->ui->treevwArchive);
    acAnalyzeIPython->setObjectName("AnalyzeWithIPython");
    connect(acAnalyzeIPython, SIGNAL(triggered()), mw, SLOT(analyzeProduct()));

    acAnalyzeJupyter = new QAction("Analyze within Jupyter Lab", mw->ui->treevwArchive);
    acAnalyzeJupyter->setObjectName("AnalyzeWithJupyter");
    acAnalyzeJupyter->setEnabled(false);
    connect(acAnalyzeJupyter, SIGNAL(triggered()), mw, SLOT(analyzeProduct()));

    acExport = new QAction("Export the selected product(s) ...", mw->ui->treevwArchive);
    connect(acExport, SIGNAL(triggered()), mw, SLOT(exportProduct()));
    /*
    acExportRemote = new QAction("Export to a remote folder", mw->ui->treevwArchive);
    connect(acExportRemote, SIGNAL(triggered()), mw, SLOT(exportProduct()));

    acExportVOSpace = new QAction("Export to configured VOSpace folder", mw->ui->treevwArchive);
    connect(acExportVOSpace, SIGNAL(triggered()), mw, SLOT(exportProduct()));

    acExportVOSpaceOther = new QAction("Export to another VOSpace folder", mw->ui->treevwArchive);
    connect(acExportVOSpaceOther, SIGNAL(triggered()), mw, SLOT(exportProduct()));
    */

}

//----------------------------------------------------------------------
// SLOT: showArchiveTableContextMenu
//----------------------------------------------------------------------
void ActionHandler::showArchiveTableContextMenu(const QPoint & p)
{
    static const int NumOfProdTypeCol = 1;

    if (mw->isProductsCustomFilterActive) { return; }

    QModelIndex m = mw->ui->treevwArchive->indexAt(p);
    //if (m.parent() == QModelIndex()) { return; }

    QModelIndex m2 = m.model()->index(m.row(), NumOfProdTypeCol, m.parent());
    if (!m2.data().isValid()) { return; }
    QString productType = m2.data().toString();

    QList<QAction *> actions;
    if (m.isValid()) {
        foreach (QString key, mw->userDefTools.keys()) {
            const QUserDefTool & udt = mw->userDefTools.value(key);
            if (udt.prod_types.contains(productType) || true) {
                QAction * ac = acUserTools[key];
                actions.append(ac);
            }
        }
        acArchiveOpenExt->clear();
        acArchiveOpenExt->addAction(acDefault);
        acArchiveOpenExt->addSeparator();
        acArchiveOpenExt->addActions(actions);

        QMenu menu(mw);
        menu.addAction(acArchiveShow);
        menu.addSeparator();
        menu.addMenu(acArchiveOpenExt);
        
        if ((m.parent().isValid()) && ((productType.left(4) == "LE1_") ||
                                      (productType.left(4) == "SIM_") ||
                                      (productType.left(4) == "SOC_"))) {
            acReprocess->setEnabled(cfg.flags.allowReprocessing());
            menu.addSeparator();
            menu.addAction(acReprocess);
            acReprocess->setProperty("clickedItem", p);
        }

        QMenu * cmAnalyze = menu.addMenu("Analyze ...");
        cmAnalyze->addAction(acAnalyzeIPython);
        cmAnalyze->addAction(acAnalyzeJupyter);

        menu.addAction(acExport);
        /*
        QMenu * cmExport = menu.addMenu("Export ...");
        cmExport->addAction(acExport);
        cmExport->addAction(acExportRemote);
        cmExport->addAction(acExportVOSpace);
        cmExport->addAction(acExportVOSpaceOther);
        */
        mw->isViewsUpdateActive = false;
        menu.exec(mw->ui->treevwArchive->mapToGlobal(p));
        mw->isViewsUpdateActive = true;
    }
}

//----------------------------------------------------------------------
// SLOT: showTabsContextMenu
// Shows closing menu for all the main tabs in the window
//----------------------------------------------------------------------
void ActionHandler::showTabsContextMenu(const QPoint & p)
{
    QWidget * w = qobject_cast<QWidget *>(sender());
    mw->isMenuForTabWidget = (w == (QWidget*)(mw->ui->tabMainWgd));
    menuPt = p;

    QMenu menu(w);
    menu.addAction(acTabClose);
    menu.addAction(acTabCloseAll);
    menu.addAction(acTabCloseOther);

    mw->isViewsUpdateActive = false;
    menu.exec(w->mapToGlobal(p));
    mw->isViewsUpdateActive = true;
}

//----------------------------------------------------------------------
// SLOT: showJsonContextMenu
//----------------------------------------------------------------------
void ActionHandler::showJsonContextMenu(const QPoint & p)
{
    QAbstractItemView * w = qobject_cast<QAbstractItemView *>(sender());

    QModelIndex m = w->indexAt(p);
    //if (m.parent() == QModelIndex()) { return; }

    if (w->indexAt(p).isValid()) {
        QAction * acExp   = new QAction(tr("Expand"), w);
        QAction * acExpS  = new QAction(tr("Expand subtree"), w);
        QAction * acExpA  = new QAction(tr("Expand all"), w);

        QAction * acColl  = new QAction(tr("Collapse"), w);
        QAction * acCollS = new QAction(tr("Collapse subtree"), w);
        QAction * acCollA = new QAction(tr("Collapse all"), w);

        connect(acExp,   SIGNAL(triggered()), mw, SLOT(jsontreeExpand()));
        connect(acExpS,  SIGNAL(triggered()), mw, SLOT(jsontreeExpandSubtree()));
        connect(acExpA,  SIGNAL(triggered()), mw, SLOT(jsontreeExpandAll()));

        connect(acColl,  SIGNAL(triggered()), mw, SLOT(jsontreeCollapse()));
        connect(acCollS, SIGNAL(triggered()), mw, SLOT(jsontreeCollapseSubtree()));
        connect(acCollA, SIGNAL(triggered()), mw, SLOT(jsontreeCollapseAll()));

        QMenu menu(w);
        menu.addAction(acExp);
        menu.addAction(acExpS);
        menu.addAction(acExpA);
        menu.addSeparator();
        menu.addAction(acColl);
        menu.addAction(acCollS);
        menu.addAction(acCollA);
        pointOfAction = p;

        mw->isViewsUpdateActive = false;
        menu.exec(w->mapToGlobal(p));
        mw->isViewsUpdateActive = true;
    }
}

//----------------------------------------------------------------------
// METHOD: createTasksMonitTreeActions
//----------------------------------------------------------------------
void ActionHandler::createTasksMonitViewActions()
{
    mw->ui->tblvwTaskMonit->setContextMenuPolicy(Qt::CustomContextMenu);

    acWorkDir         = new QAction(tr("Open task working directory..."), mw->ui->tblvwTaskMonit);
    acShowTaskInfo    = new QAction(tr("Display task information"),       mw->ui->tblvwTaskMonit);
    //acStopTask       = new QAction(tr("Stop"),                           mw->ui->tblvwTaskMonit);
    //acRestartTask    = new QAction(tr("Restart"),                        mw->ui->tblvwTaskMonit);

    acTaskPause       = new QAction(tr("Pause Task"),                     mw->ui->tblvwTaskMonit);
    acTaskResume      = new QAction(tr("Resume Task"),                    mw->ui->tblvwTaskMonit);
    acTaskCancel      = new QAction(tr("Cancel Task"),                    mw->ui->tblvwTaskMonit);

    // acAgentSuspend    = new QAction(tr("Suspend Agent Processing"),       mw->ui->tblvwTaskMonit);
    // acAgentStop       = new QAction(tr("Stop Agent Processing"),          mw->ui->tblvwTaskMonit);
    // acAgentReactivate = new QAction(tr("Reactivate Agent Processing"),    mw->ui->tblvwTaskMonit);

    acHostSuspend     = new QAction(tr("Suspend Host Processing"),        mw->ui->tblvwTaskMonit);
    acHostStop        = new QAction(tr("Stop Host Processing"),           mw->ui->tblvwTaskMonit);
    acHostReactivate  = new QAction(tr("Reactivate Host Processing"),     mw->ui->tblvwTaskMonit);

    connect(acWorkDir,          SIGNAL(triggered()), mw, SLOT(showWorkDir()));
    connect(acShowTaskInfo,     SIGNAL(triggered()), mw, SLOT(displayTaskInfo()));
    //connect(acStopTask,     SIGNAL(triggered()), mw, SLOT(stopTask()));
    //connect(acRestartTask,  SIGNAL(triggered()), mw, SLOT(restartTask()));

    connect(acTaskPause,       SIGNAL(triggered()), mw, SLOT(doTaskPause()));
    connect(acTaskResume,      SIGNAL(triggered()), mw, SLOT(doTaskResume()));
    connect(acTaskCancel,      SIGNAL(triggered()), mw, SLOT(doTaskCancel()));

    // connect(acAgentSuspend,    SIGNAL(triggered()), mw, SLOT(doAgentSuspend()));
    // connect(acAgentStop,       SIGNAL(triggered()), mw, SLOT(doAgentStop()));
    // connect(acAgentReactivate, SIGNAL(triggered()), mw, SLOT(doAgentReactivate()));

    connect(acHostSuspend,     SIGNAL(triggered()), mw, SLOT(doHostSuspend()));
    connect(acHostStop,        SIGNAL(triggered()), mw, SLOT(doHostStop()));
    connect(acHostReactivate,  SIGNAL(triggered()), mw, SLOT(doHostReactivate()));

}

//----------------------------------------------------------------------
// SLOT: showTaskMonitContextMenu
//----------------------------------------------------------------------
void ActionHandler::showTaskMonitContextMenu(const QPoint & p)
{
    if (mw->ui->tblvwTaskMonit->indexAt(p).isValid()) {
        QMenu cm;
        
        cm.addAction(acWorkDir);
        cm.addAction(acShowTaskInfo);
        //cm.addAction(acRestartTask);
        //cm.addAction(acStopTask);
        
        cm.addSeparator();
        
        QMenu * cmTask = cm.addMenu("Task control ...");
        cmTask->addAction(acTaskPause);
        cmTask->addAction(acTaskResume);
        cmTask->addAction(acTaskCancel);
        
        // QMenu * cmAgent = cm.addMenu("Agent processing ...");
        // cmAgent->addAction(acAgentSuspend);
        // cmAgent->addAction(acAgentStop);
        // cmAgent->addAction(acAgentReactivate);

        QMenu * cmHost = cm.addMenu("Host processing ...");
        cmHost->addAction(acHostSuspend);
        cmHost->addAction(acHostStop);
        cmHost->addAction(acHostReactivate);
        
        mw->isViewsUpdateActive = false;
        cm.exec(mw->ui->tblvwTaskMonit->mapToGlobal(p));
        mw->isViewsUpdateActive = true;
    }
}

//----------------------------------------------------------------------
// METHOD: createAlertsTablesAcetions
//----------------------------------------------------------------------
void ActionHandler::createAlertsTablesActions()
{
    mw->ui->tblvwAlerts->setContextMenuPolicy(Qt::CustomContextMenu);
    mw->ui->tblvwSysAlerts->setContextMenuPolicy(Qt::CustomContextMenu);

    acShowAlert = new QAction(tr("Show alert information"), this);
    connect(acShowAlert, SIGNAL(triggered()), mw, SLOT(showProcAlertInfo()));
    
    acShowSysAlert = new QAction(tr("Show alert information"), this);
    connect(acShowSysAlert, SIGNAL(triggered()), mw, SLOT(showSysAlertInfo()));
    
    //acAckAlert = new QAction(tr("Acknowledge alert"), this);

    connect(mw->ui->tblvwAlerts, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showAlertsContextMenu(const QPoint &)));
    connect(mw->ui->tblvwSysAlerts, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showSysAlertsContextMenu(const QPoint &)));
}

//----------------------------------------------------------------------
// SLOT: showAlertsContextMenu
//----------------------------------------------------------------------
void ActionHandler::showAlertsContextMenu(const QPoint & p)
{
    if (mw->isAlertsCustomFilterActive) { return; }

    QList<QAction *> actions;
    QTableView * tblvw = qobject_cast<QTableView*>(sender());
    if (tblvw->indexAt(p).isValid()) { actions.append(acShowAlert); }
    if (actions.count() > 0) {
        mw->isViewsUpdateActive = false;
        QMenu::exec(actions, tblvw->mapToGlobal(p));
        mw->isViewsUpdateActive = true;
    }
}

//----------------------------------------------------------------------
// SLOT: showSysAlertsContextMenu
//----------------------------------------------------------------------
void ActionHandler::showSysAlertsContextMenu(const QPoint & p)
{
    if (mw->isAlertsCustomFilterActive) { return; }

    QList<QAction *> actions;
    QTableView * tblvw = qobject_cast<QTableView*>(sender());
    if (tblvw->indexAt(p).isValid()) { actions.append(acShowSysAlert); }
    if (actions.count() > 0) {
        mw->isViewsUpdateActive = false;
        QMenu::exec(actions, tblvw->mapToGlobal(p));
        mw->isViewsUpdateActive = true;
    }
}

//----------------------------------------------------------------------
// METHOD: createTxViewActions
//----------------------------------------------------------------------
void ActionHandler::createTxViewActions()
{
    mw->ui->tblvwTx->setContextMenuPolicy(Qt::CustomContextMenu);

    acShowMsgInfo = new QAction(tr("Display message content"), mw->ui->tblvwTx);

    connect(acShowMsgInfo, SIGNAL(triggered()), mw, SLOT(displayTxInfo()));

    connect(mw->ui->tblvwTx, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showTxContextMenu(const QPoint &)));
}

//----------------------------------------------------------------------
// SLOT: showTxContextMenu
//----------------------------------------------------------------------
void ActionHandler::showTxContextMenu(const QPoint & p)
{
    QList<QAction *> actions;
    if (mw->ui->tblvwTx->indexAt(p).isValid()) {
        actions.append(acShowMsgInfo);

    }
    if (actions.count() > 0) {
        mw->isViewsUpdateActive = false;
        QMenu::exec(actions, mw->ui->tblvwTx->mapToGlobal(p));
        mw->isViewsUpdateActive = true;
    }
}

}
