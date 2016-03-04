#include "test_progbardlg.h"

namespace TestQPF {

TEST_F(TestProgressBarDelegate, paint) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestProgressBarDelegate, ProgressBarDelegate) {
    EXPECT_EQ(1, 1);
}


TEST_F(TestProgressBarDelegateExit, TestProgressBarDelegate_Exit) {
    EXPECT_EXIT({ exit(1); },
                ::testing::ExitedWithCode(1),
                "");
}

}
