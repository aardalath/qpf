#include "array.h"

#include "value.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: SDC
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace SDC {

Array::Array() { clear(); }

Array::~Array() { clear(); }

void Array::dump(ostream &out) {
    out << "[ " << flush;
    for (unsigned int i = 0; i < size(); ++i) {
        at(i).dump(out);
        if (i < (size() - 1)) { out << ", " << flush; }
    }
    out << " ]" << flush;
}

} // end-of: namespace SDC

