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

Value::Value(Value::ValueType t) : data(nullptr) { setType(t); }

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
        if (data != nullptr) {
            delete data;
            data = nullptr;
        }
    } catch(...) {
        exit(1);
    }
}

void Value::set(bool rhs)
{
    clear();
    data = (BytePtr)(new bool(rhs));
    dataType = BOOL;
}
void Value::set(int rhs)
{
    clear();
    data = (BytePtr)(new int(rhs));
    dataType = INT;
}
void Value::set(double rhs)
{
    clear();
    data = (BytePtr)(new double(rhs));
    dataType = REAL;
}
void Value::set(string rhs)
{
    clear();
    data = (BytePtr)(new string(rhs));
    dataType = STRING;
}
void Value::set(Array & rhs)
{
    clear();
    data = (BytePtr)(new Array(rhs));
    dataType = ARRAY;
}
void Value::set(Dict & rhs)
{
    clear();
    data = (BytePtr)(new Dict(rhs));
    dataType = DICT;
}
void Value::set(Value & rhs)
{
    clear();
    switch (rhs.type()) {
    case BOOL:
        data = (BytePtr)(new bool(rhs.asBool()));
        break;
    case INT:
        data = (BytePtr)(new int(rhs.asInt()));
        break;
    case REAL:
        data = (BytePtr)(new double(rhs.asDbl()));
        break;
    case STRING:
        data = (BytePtr)(new string(rhs.asString()));
        break;
    case ARRAY:
        data = (BytePtr)(new Array(rhs.asArray()));
        break;
    case DICT:
        data = (BytePtr)(new Dict(rhs.asDict()));
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
void Value::setType(Value::ValueType t)
{
    dataType = t;
    if (data != 0) { return; }
    switch (dataType) {
    case BOOL:
        data = (Value::BytePtr)(new bool);
        break;
    case INT:
        data = (Value::BytePtr)(new int);
        break;
    case REAL:
        data = (Value::BytePtr)(new double);
        break;
    case STRING:
        data = (Value::BytePtr)(new string);
        break;
    case ARRAY:
        data = (Value::BytePtr)(new Array);
        break;
    case DICT:
        data = (Value::BytePtr)(new Dict);
        break;
    default:
        data = 0;
        break;
    }
}

Value::BytePtr Value::getData() const { return data; }

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
    return *this;
}
Value & Value::operator=(Array & rhs)
{
    set(rhs);
    return *this;
}
Value & Value::operator=(Dict & rhs)
{
    set(rhs);
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

bool     Value::asBool()  const { return *(bool*)(data); }
int      Value::asInt()   const { return *(int*)(data); }
double   Value::asDbl()   const { return *(double*)(data); }
const string & Value::asString()   const { return *(string*)(data); }
const Array &  Value::asArray() const { return *(Array*)(data); }
const Dict &   Value::asDict()  const { return *(Dict*)(data); }

string & Value::asString()   { return *(string*)(data); }
Array &  Value::asArray() { return *(Array*)(data); }
Dict &   Value::asDict()  { return *(Dict*)(data); }

bool Value::isBool()  { return dataType == BOOL; }
bool Value::isInt()   { return dataType == INT; }
bool Value::isDbl()   { return dataType == REAL; }
bool Value::isStr()   { return dataType == STRING; }
bool Value::isArray() { return dataType == ARRAY; }
bool Value::isDict()  { return dataType == DICT; }

bool Value::isNull() { return (data != nullptr) || (dataType == UNDEFINED); }

Value & Value::operator[](string &key)
{
    if (dataType != DICT) { setType(DICT); }
    Dict & dic = const_cast<Dict&>(asDict());
    if (!dic.has(key)) { dic.insert(pair<string,Value>(key, Value(string()))); }
    return dic[key];
}

Value & Value::operator[](const char *ckey)
{
    string key(ckey);
    return this->operator[](key);
}

Value & Value::operator[](int i)
{
    if (dataType != ARRAY) { setType(ARRAY); }
    Array & array = const_cast<Array&>(asArray());
    int sz = array.size();
    if (i > sz - 1) {
        for (int k = sz; k < (i + 1); ++k) {
            array.push_back(Value(""));
        }
    }
    return array[i];
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
        out << "\"" << asString() << "\"" << flush;
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
    return lhs.asString() == rhs;
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
        return (lhs == rhs.asString());
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

void Value::setDefaultType(ValueType d)
{
    defaultDataType = d;
}

Value::ValueType Value::defaultDataType = Value::UNDEFINED;

} // end-of: namespace SDC

