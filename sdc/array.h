#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>

using namespace std;

////////////////////////////////////////////////////////////////////////////
// Namespace: SDC
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace SDC {

class Value;

class Array : public vector<Value> {
public:
    Array();
    ~Array();

    void dump(ostream & out = cout);

    inline friend ostream & operator<<(ostream & out, const Array & c) {
        const_cast<Array*>(&c)->dump(out);
        return out;
    }
};

} // end-of: namespace SDC

#endif
