/******************************************************************************
 * File:    acthdl.h
 *          Declaration of class ActionHandler
 *
 * Domain:  QPF.qpfgui.acthdl
 *
 * Version:  1.2
 *
 * Date:    2016-11-03
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2017 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declaration of class ActionHandler
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

#ifndef ACTIONHDL_H
#define ACTIONHDL_H

#include <QAction>
#include <QMenu>
#include <QWidget>
#include <QToolBar>

#include "textview.h"

namespace QPF {

class MainWindow;
 
class ActionHandler : public QObject
{
    Q_OBJECT

public:
    explicit ActionHandler(QWidget *parent = 0);
    ~ActionHandler();

public:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    void activateClipboardFor(TextView * child);

    void createLocalArchiveViewActions();
    void createTasksMonitViewActions();
    void createAlertsTablesActions();
    void createTxViewActions();

    // Accessors
    QAction * getAcQuit();
    QAction * getAcQuitAll();
    QMap<QString, QAction *> & getAcUserTools();
    QAction * getAcReprocess();
    QAction * getAcShowAlert();

public slots:
    void updateMenus();
    void updateWindowMenu();
    

private slots:
    void showArchiveTableContextMenu(const QPoint&);
    void showTabsContextMenu(const QPoint&);
    void showJsonContextMenu(const QPoint&);
    void showTaskMonitContextMenu(const QPoint&);
    void showAlertsContextMenu(const QPoint&);
    void showTxContextMenu(const QPoint&);
        
    void closeTab(int n);
    void closeTabAction();
    void closeAllTabAction();
    void closeOtherTabAction();

private:
    MainWindow * mw;
    
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *windowMenu;
    QMenu *toolsMenu;
    QMenu *sessionInfoMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *acProcessPath;
    QAction *acSaveAs;

    //QAction *acStopTask;
    //QAction *acRestartTask;

    QAction *acRestart;
    QAction *acQuit;
    QAction *acQuitAll;
#ifndef QT_NO_CLIPBOARD
    QAction *acCut;
    QAction *acCopy;
    QAction *acPaste;
#endif

    QAction *acConfigTool;
    QAction *acBrowseDB;
    QAction *acExtTools;
    QAction *acVerbosity;
    QAction *acExecTestRun;

    QAction *acDefault;
    QAction *acReprocess;

    QAction *acAnalyzeIPython;
    QAction *acAnalyzeJupyter;
    QAction *acExportLocal;
    QAction *acExportRemote;
    QAction *acExportVOSpace;
    QAction *acExportVOSpaceOther;

    QAction *acNavig;
    QAction *acClose;
    QAction *acCloseAll;
    QAction *acTile;
    QAction *acCascade;
    QAction *acNext;
    QAction *acPrevious;
    QAction *acSeparator;
    QAction *acAbout;
    QAction *acAboutQt;

    bool isMenuForTabWidget;
    QPoint menuPt;
    QAction * acTabClose;
    QAction * acTabCloseAll;
    QAction * acTabCloseOther;

    QAction * acWorkDir;
    QAction * acShowTaskInfo;

    QAction * acRestartTask;
    QAction * acStopTask;

    QAction * acTaskPause;
    QAction * acTaskResume;
    QAction * acTaskCancel;

    QAction * acAgentSuspend;
    QAction * acAgentStop;
    QAction * acAgentReactivate;

    QAction * acHostSuspend;
    QAction * acHostStop;
    QAction * acHostReactivate;

    QAction * acShowMsgInfo;

    QAction * acShowAlert;
    QAction * acAckAlert;

    QAction * acArchiveShow;
    QMenu *   acArchiveOpenExt;
    QList<QAction *> acArchiveOpenExtTools;
    QMap<QString, QAction *> acUserTools;

    QPoint pointOfAction;
};

}

#endif // ACTIONHDL_H
