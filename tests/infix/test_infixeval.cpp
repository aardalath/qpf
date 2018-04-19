#include "test_infixeval.h"

#define CheckResultOfStr(s,r) do {                                      \
    std::cerr << "------------------------------------------------------------\n"; \
    ev.clear();                                                         \
    ev.set(std::string( s ));                                           \
    std::cerr << s << "  =>  " << ev.getValue() << "  ( " << r << " )\n";     \
    EXPECT_EQ(ev.getValue(), r);                                        \
    std::cerr << "------------------------------------------------------------\n"; \
    } while (0)

#define CheckResultOf(s,r)  CheckResultOfStr( #s , r )

namespace TestInFix {

TEST_F(TestInFix, SimpleInfixOperations1) {
    
    CheckResultOf(10 + 2 * 6, 22);
    CheckResultOf(10 - 2 * 6, -2);
    CheckResultOf(100 * 2 + 12, 212);
    CheckResultOf(100 * ( 2 + 12 ), 1400);
    CheckResultOf(( 8 + 4 + 12 ) / 6, 4);

}

TEST_F(TestInFix, SimpleInfixOperations2) {
    
    CheckResultOf((2 ^ 3) < 9, 1);
    CheckResultOf((2 ^ 3) > 9, 0);
    CheckResultOf((3 ^ 2) >= 9, 1);
    CheckResultOf((2 ^ 3) <= 9, 1);

}

TEST_F(TestInFix, SimpleInfixOperations3) {
    
    CheckResultOf(131 & 5, 1);
    CheckResultOf(131 & 0, 0);
    CheckResultOf(129 | 5, 1);
    CheckResultOf((23 < 12) | (5 > 3), 1);

}

TEST_F(TestInFix, SimpleInfixOperations4) {

    CheckResultOf("a = 24; a + 12", 36);

    CheckResultOf(b = 12, 12);
    CheckResultOf(a <> b, 0);
    
    CheckResultOf(2 * 12, 24);
    CheckResultOf(c = _, 0);
    CheckResultOf(a == c, 1);

}

}
