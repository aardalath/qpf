#include "test_TskOrc.h"

//#define CheckResultOf(s,r) do {                                         \
//    ev.clear();                                                         \
//    ev.set(std::string( #s ));                                          \
//    EXPECT_EQ(ev.getValue(), r);                                        \
//    } while (0)

namespace TestTskOrc {

TEST_F(TestTskOrc, Test_defineOrchestrationParams) {
    EXPECT_DEATH({TskOrc x("name", "address", 0);}, "failed");
    
}

TEST_F(TestTskOrc, Test_createTasks) {
    
}

TEST_F(TestTskOrc, Test_createTask) {
    
}

TEST_F(TestTskOrc, Test_checkRulesForProductType) {
    
}

}           
