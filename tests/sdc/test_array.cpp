#include "test_array.h"

#include <sstream>

namespace TestSDC {

TEST_F(TestArray, Array) {
    Array a;

    Value v1(1);
    Value v2(std::string("Hola"));
    Value v3(3.141592);

    a.push_back(v1);
    a.push_back(v2);
    a.push_back(v3);

    EXPECT_EQ(v1, a.at(0));
    EXPECT_EQ(v2, a.at(1));
    EXPECT_EQ(v3, a.at(2));
}

TEST_F(TestArray, dump) {
    Array a;

    Value v1(1);
    Value v2(std::string("Hola"));
    Value v3(3.141592);

    a.push_back(v1);
    a.push_back(v2);
    a.push_back(v3);

    std::stringstream ss;
    a.dump(ss);
    std::cout << ss.str() << std::endl;
    EXPECT_EQ(ss.str(),
              std::string("[ 1, \"Hola\", 3.14159 ]"));
}

}
