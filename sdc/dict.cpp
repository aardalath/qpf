#include "dict.h"

#include "value.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: SDC
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace SDC {

Dict::Dict() { clear(); }

Dict::~Dict() { clear(); }

void Dict::dump(ostream &out) {
    out << "{ ";
    map<string, Value>::iterator it = begin();
    while (it != end()) {
        out << '"' << (*it).first << '"' << " : " << flush;
        Value & v = (*it).second;
        v.dump(out);
        ++it;
        if (it != end()) { out << ", " << flush; }
    }
    out << " }" << flush;
}

bool Dict::has(string key) {
    return find(key) != end();
}

} // end-of: namespace SDC

