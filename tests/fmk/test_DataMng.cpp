#include "test_DataMng.h"

//#define CheckResultOf(s,r) do {                                         \
//    ev.clear();                                                         \
//    ev.set(std::string( #s ));                                          \
//    EXPECT_EQ(ev.getValue(), r);                                        \
//    } while (0)

namespace TestDataMng {

TEST_F(TestDataMng, Test_isProductAvailable) {
    EXPECT_DEATH({DataMng x("name", "address", 0);}, "failed");

}

TEST_F(TestDataMng, Test_getProductLatest) {
    
}

TEST_F(TestDataMng, Test_txInDataToLocalArch) {
    
}

TEST_F(TestDataMng, Test_storeTskRegData) {
    
}

TEST_F(TestDataMng, Test_saveTaskToDB) {
    
}

}           
