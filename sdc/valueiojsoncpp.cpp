#include "valueiojsoncpp.h"

#include "value.h"
#include "array.h"
#include "dict.h"

#include <fstream>
#include <sstream>

using namespace std;

////////////////////////////////////////////////////////////////////////////
// Namespace: SDC
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace SDC {

void ValueIOJsonCpp::readFrom(string fileName, Value & v)
{
    string serializedString;

    ifstream ifs;
    ifs.open(fileName, std::ifstream::in);
    if (ifs.good()) {
        stringstream ss;
        ss << ifs.rdbuf();
        serializedString = ss.str();
        ifs.close();
    }

    deserialize(serializedString, v);
}

void ValueIOJsonCpp::saveTo(string fileName, Value & v)
{
    string serializedString;
    serialize(v, serializedString);

    ofstream ofs;
    ofs.open(fileName, std::ifstream::out);
    if (ofs.good()) {
        ofs.write(serializedString.c_str(), serializedString.length());
        ofs.close();
    }
}

void ValueIOJsonCpp::deserialize(string serializedString, Value & v)
{
    Json::Value j;
    Json::Reader reader;
    reader.parse(serializedString, j);
    importData(j, v);
}

void ValueIOJsonCpp::serialize(Value & v, string & serializedString)
{
    Json::Value j;
    exportData(v, j);
    Json::FastWriter writer;
    serializedString = writer.write(j);
}

void ValueIOJsonCpp::importData(Json::Value & in, Value & out)
{
    if (in.isString()) {
        out = in.asString();
    } else if (in.isBool()) {
        out = (bool)(in.asBool());
    } else if (in.isInt()) {
        out = (int)(in.asInt());
    } else if (in.isUInt()) {
        out = (int)(in.asUInt());
    } else if (in.isDouble()) {
        out = (double)(in.asDouble());
    } else if (in.isArray()) {
        Array a;
        for (auto vin : in) {
            Value vout;
            importData(vin, vout);
            a.push_back(vout);
        }
        out = a;
    } else if (in.isObject()) {
        Dict d;
        Json::ValueIterator it = in.begin();
        while (it != in.end()) {
            string key = it.key().asString();
            Value vout;
            importData((*it), vout);
            d[key] = vout;
            ++it;
        }
        out = d;
    } else {
        out = "";
    }
}

void ValueIOJsonCpp::exportData(Value & in, Json::Value & out)
{
    if (in.isStr()) {
        out = in.asString();
    } else if (in.isBool()) {
        out = (bool)(in.asBool());
    } else if (in.isInt()) {
        out = (int)(in.asInt());
    } else if (in.isDbl()) {
        out = (double)(in.asDbl());
    } else if (in.isArray()) {
        Json::Value v(Json::arrayValue);
        for (auto vin : in.asArray()) {
            Json::Value vout;
            exportData(vin, vout);
            v.append(vout);
        }
        out = v;
    } else if (in.isDict()) {
        Json::Value v(Json::objectValue);
        Dict & d = const_cast<Dict&>(in.asDict());
        map<string, Value>::iterator it  = d.begin();
        map<string, Value>::iterator ite = d.end();
        while (it != ite) {
            const string & key = (*it).first;
            Value & vin = (*it).second;
            Json::Value vout;
            exportData(vin, vout);
            v[key] = vout;
            ++it;
        }
        out = v;
    } else {
        out = "";
    }
}

} // end-of: namespace SDC

