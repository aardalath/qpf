#include "test_dbhdlpostgre.h"

namespace TestQPF {

TEST_F(TestDBHdlPostgreSQL, checkTask) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, closeConnection) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, DBHdlPostgreSQL) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, fillWithResult) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, getNumRowsInTable) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, getQuery) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, getTable) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, openConnection) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, retrieveMsgs) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, retrieveProducts) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, retrieveTask) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, runCmd) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, storeMsg) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, storeProducts) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, storeTask) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, updateTask) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDBHdlPostgreSQL, updTable) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestDBHdlPostgreSQLExit, TestDBHdlPostgreSQL_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
