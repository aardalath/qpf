#ifndef VALUEIOJSONCPP_H
#define VALUEIOJSONCPP_H

#include "valueio.h"

#include "json/json.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: SDC
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace SDC {

class ValueIOJsonCpp : public ValueIO {
public:
    ValueIOJsonCpp() {}
    virtual ~ValueIOJsonCpp() {}

    virtual void readFrom(string fileName, Value & v);
    virtual void saveTo(string fileName, Value & v);

    virtual void deserialize(string serializedString, Value &v);
    virtual void serialize(Value &v, string &serializedString);

protected:
    void importData(Json::Value &in, Value &out);
    void exportData(Value &in, Json::Value &out);
};

} // end-of: namespace SDC

#endif
