#include "test_Master.h"

//#define CheckResultOf(s,r) do {                                         \
//    ev.clear();                                                         \
//    ev.set(std::string( #s ));                                          \
//    EXPECT_EQ(ev.getValue(), r);                                        \
//    } while (0)

namespace TestMaster {

TEST_F(TestMaster, Test_init) {
    EXPECT_DEATH({Master x("name", "address", 0);}, "failed");
    
}

TEST_F(TestMaster, Test_fromRunningToOperational) {
    
}

TEST_F(TestMaster, Test_fromOperationalToRunning) {
    
}

TEST_F(TestMaster, Test_setEvtMng) {
    
}

TEST_F(TestMaster, Test_setTskMng) {
    
}

TEST_F(TestMaster, Test_runEachIteration) {
    
}

}           
