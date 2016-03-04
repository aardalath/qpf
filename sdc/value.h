#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

#include "array.h"
#include "dict.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: SDC
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace SDC {

class Value {

public:
    enum ValueType {
        UNDEFINED,
        BOOL,
        INT,
        REAL,
        STRING,
        ARRAY,
        DICT,
    };

    Value(ValueType t = defaultDataType);

    Value(bool x);
    Value(int x);
    Value(double x);
    Value(string x);
    Value(Array & x);
    Value(Dict & x);
    Value(Value & x);
    Value(const Value & x);

    ~Value();

    typedef unsigned char     byte;
    typedef unsigned char *   BytePtr;

    typedef map<string, Value>::iterator  iterator;

    void clear();

    void set(bool rhs);
    void set(int rhs);
    void set(double rhs);
    void set(string rhs);
    void set(Array & rhs);
    void set(Dict & rhs);
    void set(Value & rhs);
    void set(const Value & rhs);

    ValueType type() const;
    void setType(Value::ValueType t);
    BytePtr getData() const;
    int getSizeOfData() const;

    Value & operator=(bool rhs);
    Value & operator=(int rhs);
    Value & operator=(unsigned int rhs);
    Value & operator=(float rhs);
    Value & operator=(double rhs);
    Value & operator=(string rhs);
    Value & operator=(Array & rhs);
    Value & operator=(Dict & rhs);
    Value & operator=(Value & rhs);
    Value & operator=(const Value & rhs);

    Value & operator<<(Value & rhs);
    Value & operator<<(const Value & rhs);
    Value & operator<<(std::pair<string, Value> & rhs);
    Value & operator<<(const std::pair<string, Value> & rhs);

    bool     asBool() const;
    int      asInt() const;
    double   asDbl() const;
    const string & asString() const;
    const Array  & asArray() const;
    const Dict   & asDict() const;

    string & asString();
    Array  & asArray();
    Dict   & asDict();

    bool isBool();
    bool isInt();
    bool isDbl();
    bool isStr();
    bool isArray();
    bool isDict();

    bool isNull();

    Value & operator[](string & key);
    Value & operator[](const char * ckey);
    Value & operator[](int i);

    unsigned int size();

    void dump(ostream & out = cout);

    friend ostream &operator<<(ostream &out, const Value & c) {
        const_cast<Value*>(&c)->dump(out);
        return out;
    }

    void readFrom(string fileName);
    void saveTo(string fileName);

    void deserialize(string serializedString);
    void serialize(string & serializedString, bool formatted = false);
    string asSerialized(bool formatted = false);

    void prettyFormat(string &serializedString);

    Value::iterator begin();
    Value::iterator end();

    static void setDefaultType(ValueType d);

protected:
    BytePtr    data;
    ValueType  dataType;

    static ValueType  defaultDataType;
};

bool operator!=(const Value& lhs, const bool& rhs);
bool operator==(const Value& lhs, const bool& rhs);

bool operator!=(const Value& lhs, const int& rhs);
bool operator==(const Value& lhs, const int& rhs);

bool operator!=(const Value& lhs, const double& rhs);
bool operator==(const Value& lhs, const double& rhs);

bool operator!=(const Value& lhs, const string& rhs);
bool operator==(const Value& lhs, const string& rhs);

bool operator!=(const Value& lhs, const Array& rhs);
bool operator==(const Value& lhs, const Array& rhs);

bool operator!=(const Value& lhs, const Dict& rhs);
bool operator==(const Value& lhs, const Dict& rhs);

bool operator!=(const Value& lhs, const Value& rhs);
bool operator==(const Value& lhs, const Value& rhs);

} // end-of: namespace SDC


#endif
