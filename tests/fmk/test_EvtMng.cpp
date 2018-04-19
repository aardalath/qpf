#include "test_EvtMng.h"

//#define CheckResultOf(s,r) do {                                         \
//    ev.clear();                                                         \
//    ev.set(std::string( #s ));                                          \
//    EXPECT_EQ(ev.getValue(), r);                                        \
//    } while (0)

namespace TestEvtMng {

TEST_F(TestEvtMng, Test_fromRunningToOperational) {
    EXPECT_DEATH({EvtMng x("name", "address", 0);}, "failed");
    
}

TEST_F(TestEvtMng, Test_fromOperationalToRunning) {
    
}

TEST_F(TestEvtMng, Test_runEachIteration) {
    
}

TEST_F(TestEvtMng, Test_getInData) {
    
}

TEST_F(TestEvtMng, Test_getReprocData) {
    
}

TEST_F(TestEvtMng, Test_isHMIActive) {
    
}

TEST_F(TestEvtMng, Test_isQuitRequested) {
    
}

TEST_F(TestEvtMng, Test_quit) {
    
}

TEST_F(TestEvtMng, Test_sendProcFmkInfoUpdate) {
    
}

}           
