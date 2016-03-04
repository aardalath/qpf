#include "test_dict.h"

#include <sstream>

namespace TestSDC {

TEST_F(TestDict, Dict) {
    Dict d;

    Value v1(1);
    Value v2(std::string("Hola"));
    Value v3(3.141592);

    d["one"]   = v1;
    d["two"]   = v2;
    d["three"] = v3;
    d["four"]  = v1;

    EXPECT_EQ(v1, d["one"]);
    EXPECT_EQ(v2, d["two"]);
    EXPECT_EQ(v3, d["three"]);
    EXPECT_NE(v1, d["three"]);
    EXPECT_EQ(d["one"], d["four"]);

    d["one"] = d["three"];
    EXPECT_NE(v1, d["one"]);
    EXPECT_NE(d["one"], d["four"]);
    EXPECT_EQ(d["three"], d["one"]);
}

TEST_F(TestDict, dump) {
    Dict d;

    Value v1(1);
    Value v2(std::string("Hola"));
    Value v3(3.141592);

    d["one"]   = v1;
    d["two"]   = v2;
    d["three"] = v3;
    d["four"]  = v1;

    std::stringstream ss;
    d.dump(ss);
    std::cout << ss.str() << std::endl;
    EXPECT_EQ(ss.str(),
              std::string("{ \"four\" : 1, \"one\" : 1, \"three\" : 3.14159, \"two\" : \"Hola\" }"));
}

TEST_F(TestDict, has) {
    Dict d;

    Value v1(1);

    d["one"]   = v1;

    EXPECT_TRUE(d.has("one"));
    EXPECT_FALSE(d.has("unknown"));

    d.erase(d.find("one"));
    EXPECT_FALSE(d.has("one"));
}

}
