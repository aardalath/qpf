#include "test_archivemodel.h"

namespace TestQPF {

TEST_F(TestArchiveModel, ArchiveModel) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestArchiveModel, setupArchiveWatcher) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestArchiveModel, setupModel) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestArchiveModel, setupView) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestArchiveModel, updateView) {
    EXPECT_EQ(1, 1);
}


TEST_F(TestArchiveModelExit, TestArchiveModel_Exit) {
    EXPECT_EXIT({ exit(1); },
                ::testing::ExitedWithCode(1),
                "");
}

}
