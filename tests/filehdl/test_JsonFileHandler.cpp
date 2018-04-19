#include "test_JsonFileHandler.h"

//#define CheckResultOf(s,r) do {                                         \
//    ev.clear();                                                         \
//    ev.set(std::string( #s ));                                          \
//    EXPECT_EQ(ev.getValue(), r);                                        \
//    } while (0)

namespace TestJsonFileHandler {

TEST_F(TestJsonFileHandler, Test_JSONFHdl) {    
    Json::Value v;
    Json::Reader reader;
    reader.parse("{\"key1\": 1, \"key2\": \"dos\", \"key3\": [ 3 ]}", v);

    JsonFileHandler jfh("/tmp/data.json");

    jfh.setData(v);

    EXPECT_EQ(true, jfh.write());
    EXPECT_EQ(true, jfh.read());

    Json::Value w = jfh.getData();

    EXPECT_EQ(v, w);
}
    
TEST_F(TestJsonFileHandler, Test_JSONFHdlErr) {    
    JsonFileHandler jfh1("/tmp/non-existing-file.json");
    EXPECT_EQ(jfh1.read(), false);
                                 
    JsonFileHandler jfh2(std::string("/unwritable-file.json"));
    EXPECT_EQ(jfh2.write(), false);
}

}           
