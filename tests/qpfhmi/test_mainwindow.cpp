#include "test_mainwindow.h"

namespace TestQPF {

TEST_F(TestMainWindow, about) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, activeTextView) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, checkForTaskRes) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, clearLayout) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, closeEvent) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, commandSystem) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, copy) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, createActions) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, createMenus) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, createStatusBar) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, createTextView) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, createToolBars) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, cut) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, displayTaskInfo) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, dumpTaskInfoToTree) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, dumpToTree) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, endOfInDataMsgs) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, handleFinishedHMI) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, init) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, initLogWatch) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, initTasksMonitTree) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, MainWindow) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, manualSetupUI) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, paste) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, pauseTask) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, prepareSendInDataFromFile) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, prepareSendMultInData) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, processInbox) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, processPendingEvents) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, readConfig) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, readSettings) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, resumeTask) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, runDockerCmd) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, saveAs) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, selectInboxPath) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, selectInDataParamsFile) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, sendInData) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, sentInData) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, setActiveSubWindow) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, setDebugInfo) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, setLogWatch) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, showConfigTool) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, showTaskMonitContextMenu) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, showTaskRes) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, showWorkDir) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, sortTaskViewByColumn) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, stopSendingMultInData) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, stopTask) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, switchLayoutDirection) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, transitToOperational) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, updateAgentsMonitPanel) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, updateMenus) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, updateTasksMonitTree) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, updateWindowMenu) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestMainWindow, writeSettings) {
    EXPECT_EQ(1, 1);
}


TEST_F(TestMainWindowExit, TestMainWindow_Exit) {
    EXPECT_EXIT({ exit(1); },
                ::testing::ExitedWithCode(1),
                "");
}

}
