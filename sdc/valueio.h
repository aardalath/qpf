#ifndef VALUEIO_H
#define VALUEIO_H

#include "value.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: SDC
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace SDC {

class ValueIO {
public:
    ValueIO() {}
    virtual ~ValueIO() {}

    virtual void readFrom(string fileName, Value & v) = 0;
    virtual void saveTo(string fileName, Value & v) = 0;

    virtual void deserialize(string serializedString, Value &v) = 0;
    virtual void serialize(Value &v, string &serializedString) = 0;
};

} // end-of: namespace SDC

#endif
