#ifndef DICT_H
#define DICT_H

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

class Dict : public map<string, Value> {
public:
    Dict();
    ~Dict();

    typedef std::pair<string, Value> item;

    void dump(ostream & out = cout);
    bool has(string key);

    inline friend ostream &operator<<(ostream &out, const Dict & c) {
        const_cast<Dict*>(&c)->dump(out);
        return out;
    }

};

} // end-of: namespace SDC

#endif
