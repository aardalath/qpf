#include "value.h"
#include "array.h"
#include "dict.h"

#include "valueiojsoncpp.h"

#include <cassert>

using namespace std;

////////////////////////////////////////////////////////////////////////////
// Namespace: SDC
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace SDC {

Value::Value()                : data(nullptr), dataType(UNDEFINED) {}
Value::Value(bool x)          : data(nullptr) { (*this) = x; }
Value::Value(int x)           : data(nullptr) { (*this) = x; }
Value::Value(double x)        : data(nullptr) { (*this) = x; }
Value::Value(string x)        : data(nullptr) { (*this) = x; }
Value::Value(Array & x)       : data(nullptr) { (*this) = x; }
Value::Value(Dict & x)        : data(nullptr) { (*this) = x; }
Value::Value(Value & x)       : data(nullptr) { (*this) = x; }
Value::Value(const Value & x) : data(nullptr) { (*this) = x; }

Value::~Value() { clear(); }

void Value::clear()
{
    try {
        if (data != nullptr) { delete data; }
    } catch(...) {
        exit(1);
    }
}

void Value::set(bool rhs)
{
    clear();
    data = (bytePtr)(new bool(rhs));
    dataType = BOOL;
}
void Value::set(int rhs)
{
    clear();
    data = (bytePtr)(new int(rhs));
    dataType = INT;
}
void Value::set(double rhs)
{
    clear();
    data = (bytePtr)(new double(rhs));
    dataType = REAL;
}
void Value::set(string rhs)
{
    clear();
    data = (bytePtr)(new string(rhs));
    dataType = STRING;
}
void Value::set(Array & rhs)
{
    clear();
    data = (bytePtr)(new Array(rhs));
    dataType = ARRAY;
}
void Value::set(Dict & rhs)
{
    clear();
    data = (bytePtr)(new Dict(rhs));
    dataType = DICT;
}
void Value::set(Value & rhs)
{
    clear();
    switch (rhs.type()) {
    case BOOL:
        data = (bytePtr)(new bool(rhs.asBool()));
        break;
    case INT:
        data = (bytePtr)(new int(rhs.asInt()));
        break;
    case REAL:
        data = (bytePtr)(new double(rhs.asDbl()));
        break;
    case STRING:
        data = (bytePtr)(new string(rhs.asStr()));
        break;
    case ARRAY:
        data = (bytePtr)(new Array(rhs.asArray()));
        break;
    case DICT:
        data = (bytePtr)(new Dict(rhs.asDict()));
        break;
    default:
        break;
    }
    dataType = rhs.type();
}
void Value::set(const Value & rhs)
{
    Value & v = *const_cast<Value*>(&rhs);
    set(v);
}

Value::ValueType Value::type() const { return dataType; }

Value::bytePtr Value::getData() const { return data; }

int Value::getSizeOfData() const
{
    int sizeOfData = 1;
    switch (dataType) {
    case BOOL:
        sizeOfData = sizeof(bool);
        break;
    case INT:
        sizeOfData = sizeof(int);
        break;
    case REAL:
        sizeOfData = sizeof(double);
        break;
    case STRING:
        sizeOfData = sizeof(string);
        break;
    case ARRAY:
        sizeOfData = sizeof(Array);
        break;
    case DICT:
        sizeOfData = sizeof(Dict);
        break;
    default:
        break;
    }
    return sizeOfData;
}

Value & Value::operator=(bool rhs)
{
    set(rhs);
    return *this;
}
Value & Value::operator=(int rhs)
{
    set(rhs);
    return *this;
}
Value & Value::operator=(unsigned int rhs)
{
    set((int)(rhs));
    return *this;
}
Value & Value::operator=(float rhs)
{
    set((double)(rhs));
    return *this;
}
Value & Value::operator=(double rhs)
{
    set(rhs);
    return *this;
}
Value & Value::operator=(string rhs)
{
    set(rhs);
    s = (string*)(data);
    return *this;
}
Value & Value::operator=(Array & rhs)
{
    set(rhs);
    a = (Array*)(data);
    return *this;
}
Value & Value::operator=(Dict & rhs)
{
    set(rhs);
    d = (Dict*)(data);
    return *this;
}
Value & Value::operator=(Value & rhs)
{
    if (data != rhs.getData()) set(rhs);
    return *this;
}
Value & Value::operator=(const Value & rhs)
{
    if (data != const_cast<Value*>(&rhs)->getData()) set(rhs);
    return *this;
}

Value & Value::operator<<(Value & rhs)
{
    if (data != rhs.getData())
        const_cast<Array&>(asArray()).push_back(rhs);
    return *this;
}

Value & Value::operator<<(const Value & rhs)
{
    if (data != const_cast<Value*>(&rhs)->getData())
        const_cast<Array&>(asArray()).push_back(rhs);
    return *this;
}

Value & Value::operator<<(std::pair<string, Value> & rhs)
{
    Dict & d = const_cast<Dict&>(asDict());
    d[rhs.first] = rhs.second;
    return *this;
}

Value & Value::operator<<(const std::pair<string, Value> & rhs)
{
    Dict & d = const_cast<Dict&>(asDict());
    d[rhs.first] = rhs.second;
    return *this;
}

//=== Dereference

const bool     Value::asBool()  const { return *(bool*)(data); }
const int      Value::asInt()   const { return *(int*)(data); }
const double   Value::asDbl()   const { return *(double*)(data); }
const string & Value::asStr()   const { return *(string*)(data); }
const Array &  Value::asArray() const { return *(Array*)(data); }
const Dict &   Value::asDict()  const { return *(Dict*)(data); }

string & Value::asStr()   { return *(string*)(data); }
Array &  Value::asArray() { return *(Array*)(data); }
Dict &   Value::asDict()  { return *(Dict*)(data); }

bool Value::isBool()  { return dataType == BOOL; }
bool Value::isInt()   { return dataType == INT; }
bool Value::isDbl()   { return dataType == REAL; }
bool Value::isStr()   { return dataType == STRING; }
bool Value::isArray() { return dataType == ARRAY; }
bool Value::isDict()  { return dataType == DICT; }

bool Value::isNull() { return (data != nullptr) || (dataType == UNDEFINED); }

Value &Value::operator[](string &key)
{
    assert(dataType == DICT);
    Dict & dic = const_cast<Dict&>(asDict());
    return dic[key];
}

Value &Value::operator[](const char *ckey)
{
    assert(dataType == DICT);
    string key(ckey);
    return (*this)[key];
}

Value &Value::operator[](int i)
{
    assert(dataType == ARRAY);
    Array & arr = const_cast<Array&>(asArray());
    if (i > arr.size()) { arr.reserve(i + 1); }
    return arr[i];
}

unsigned int Value::size()
{
    if (dataType == ARRAY) {
        return asArray().size();
    } else if (dataType == DICT) {
        return asDict().size();
    } else {
        return 0;
    }
}

Value::iterator Value::begin()
{
    assert(dataType == DICT);
    Dict & d = const_cast<Dict&>(asDict());
    return d.begin();
}

Value::iterator Value::end()
{
    assert(dataType == DICT);
    Dict & d = const_cast<Dict&>(asDict());
    return d.end();
}

void Value::dump(ostream & out)
{
    switch (dataType) {
    case BOOL:
        out << "\"" << (asBool() ? "True" : "False") << "\"" << flush;
        break;
    case INT:
        out << asInt() << flush;
        break;
    case REAL:
        out << asDbl() << flush;
        break;
    case STRING:
        out << "\"" << asStr() << "\"" << flush;
        break;
    case ARRAY:
        const_cast<Array&>(asArray()).dump(out);
        break;
    case DICT:
        const_cast<Dict&>(asDict()).dump(out);
        break;
    default:
        break;
    }
}

void Value::readFrom(string fileName)
{
    ValueIO * ioHdl = new ValueIOJsonCpp;
    ioHdl->readFrom(fileName, *this);
    delete ioHdl;
}

void Value::saveTo(string fileName)
{
    ValueIO * ioHdl = new ValueIOJsonCpp;
    ioHdl->saveTo(fileName, *this);
    delete ioHdl;
}

void Value::deserialize(string serializedString)
{
    ValueIO * ioHdl = new ValueIOJsonCpp;
    ioHdl->deserialize(serializedString, *this);
    delete ioHdl;
}

void Value::serialize(string & serializedString, bool formatted)
{
    ValueIO * ioHdl = new ValueIOJsonCpp;
    ioHdl->serialize(*this, serializedString);
    delete ioHdl;

    if (formatted) { prettyFormat(serializedString); }
}

string Value::asSerialized(bool formatted)
{
    string serializedString;
    serialize(serializedString, formatted);
    return serializedString;
}

void Value::prettyFormat(string & serializedString)
{
    string fmt;
    bool inString = false;
    bool inString2 = false;
    int level = 0;
    for (char c : serializedString) {
        if (c == '\'') inString = !inString;
        if (c == '"') inString2 = !inString2;
        switch (c) {
        case '{':
        case '[':
            fmt += c;
            if (!(inString || inString2)) {
                level++;
                fmt += '\n' + string(4 * level - 1, ' ');
            }
            break;
        case '}':
        case ']':
            if (!(inString || inString2)) {
                level--;
                fmt += '\n' + string(4 * level, ' ');
            }
            fmt += c;
            break;
        case ',':
            fmt += c;
            if (!(inString || inString2)) {
                fmt += '\n' + string(4 * level - 1, ' ');
            }
            break;
        default:
            fmt += c;
            break;
        }
    }
    serializedString = fmt;
}

bool operator!=(const Value& lhs, const bool& rhs) { return !(lhs == rhs); }
bool operator==(const Value& lhs, const bool& rhs)
{
    return lhs.asBool() == rhs;
}

bool operator!=(const Value& lhs, const int& rhs) { return !(lhs == rhs); }
bool operator==(const Value& lhs, const int& rhs)
{
    return lhs.asInt() == rhs;
}

bool operator!=(const Value& lhs, const double& rhs) { return !(lhs == rhs); }
bool operator==(const Value& lhs, const double& rhs)
{
    return lhs.asDbl() == rhs;
}

bool operator!=(const Value& lhs, const string& rhs) { return !(lhs == rhs); }
bool operator==(const Value& lhs, const string& rhs)
{
    return lhs.asStr() == rhs;
}

bool operator!=(const Value& lhs, const Array& rhs) { return !(lhs == rhs); }
bool operator==(const Value& lhs, const Array& rhs)
{
    const Array & lhsa = lhs.asArray();
    if (lhsa.size() != rhs.size()) { return false; }
    return std::equal(lhsa.begin(), lhsa.end(), rhs.begin());
}

bool operator!=(const Value& lhs, const Dict& rhs) { return !(lhs == rhs); }
bool operator==(const Value& lhs, const Dict& rhs)
{
    const Dict & lhsd = lhs.asDict();
    if (lhsd.size() != rhs.size()) { return false; }
    return std::equal(lhsd.begin(), lhsd.end(), rhs.begin());
}

bool operator!=(const Value& lhs, const Value& rhs) { return !(lhs == rhs); }
bool operator==(const Value& lhs, const Value& rhs)
{
    if (lhs.type() != rhs.type()) { return false; }
    switch (lhs.type()) {
    case Value::BOOL:
        return (lhs == rhs.asInt());
        break;
    case Value::INT:
        return (lhs == rhs.asInt());
        break;
    case Value::REAL:
        return (lhs == rhs.asDbl());
        break;
    case Value::STRING:
        return (lhs == rhs.asStr());
        break;
    case Value::ARRAY:
        return (lhs == rhs.asArray());
        break;
    case Value::DICT:
        return (lhs == rhs.asDict());
        break;
    default:
        break;
    }
    return false;
}

} // end-of: namespace SDC

