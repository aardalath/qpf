/******************************************************************************
 * File:    msgtypes.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.msgtypes
 *
 * Version: 1.0
 *
 * Date:    2015/07/01
 *
 * Copyright (C) 2015 J C Gonzalez
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declare Msgtypes class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   CommNode
 *
 * Files read / modified:
 *   none
 *
 * History:
 *   See <Changelog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#ifndef MSGTYPES_H
#define MSGTYPES_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   - string
//   - map
//   - iostream
//------------------------------------------------------------

#include <string>
#include <map>
#include <iostream>
#include <memory>

//------------------------------------------------------------
// Topic: External packages
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   - propdef.h
//   - json/json.h
//------------------------------------------------------------

#include "propdef.h"
#include "json/json.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//==========================================================================
// Group: Message related type definitions
//==========================================================================

//------------------------------------------------------------
// Topic: Basic types
//------------------------------------------------------------

typedef std::string   ProductType;
typedef std::string   DateTime;
typedef std::string   URL;
typedef std::string   Instrument;
typedef std::string   ObsMode;
typedef std::string   Creator;
typedef std::string   ProductId;
typedef std::string   ProductVersion;
typedef std::string   ProductStatus;
typedef unsigned int  ProductSize;

//------------------------------------------------------------
// Topic: Structures
//------------------------------------------------------------

struct JsonStruct {
    JsonStruct() : data(Json::nullValue) {}
    virtual ~JsonStruct() {}

    virtual void setData(Json::Value & v) {
        data = v;
//        Json::StyledWriter w;
//        std::cerr << w.write(v) << std::endl;
        toFields();
    }
    virtual void setData(std::string key, Json::Value & v) {
        data[key] = v;
        toFields();
    }
    virtual void setDataString(std::string vStr) {
        Json::Reader reader;
        reader.parse(vStr, data);
        toFields();
    }
    virtual Json::Value & getData() {
        toData();
        return data;
    }
    virtual std::string getDataString() {
        toData();
        Json::FastWriter writer;
        return writer.write(data);
    }
    virtual void toFields() = 0;
    virtual void toData() = 0;

    Json::Value data;
};

struct ProductMetadata : public JsonStruct {
    DateTime       startTime;
    DateTime       endTime;
    DateTime       regTime;
    Instrument     instrument;
    ObsMode        obsMode;
    Creator        creator;
    ProductId      productId;
    ProductType    productType;
    ProductVersion productVersion;
    ProductStatus  productStatus;
    ProductSize    productSize;
    URL            url;

    virtual void toFields() {
        startTime      = data["startTime"     ].asString();
        endTime        = data["endTime"       ].asString();
        instrument     = data["instrument"    ].asString();
        obsMode        = data["obsMode"       ].asString();
        creator        = data["creator"       ].asString();
        productId      = data["productId"     ].asString();
        productType    = data["productType"   ].asString();
        productVersion = data["productVersion"].asString();
        productStatus  = data["productStatus" ].asString();
        productSize    = data["productSize"   ].asUInt();
        url            = data["url"           ].asString();
    }

    virtual void toData() {
        data["startTime"     ] = startTime;
        data["endTime"       ] = endTime;
        data["instrument"    ] = instrument;
        data["obsMode"       ] = obsMode;
        data["productId"     ] = productId;
        data["productType"   ] = productType;
        data["productVersion"] = productVersion;
        data["productStatus" ] = productStatus;
        data["productSize"   ] = productSize;
        data["url"           ] = url;
    }
};

struct ProductCollection : public JsonStruct {
    std::map<ProductType, ProductMetadata> productList;

    virtual void toFields() {
        productList.clear();
        for (unsigned int i = 0; i < data.size(); ++i) {
            Json::Value & v = data[i];
            ProductType t = v["productType"].asString();
            ProductMetadata m;
            m.setData(v);
            productList[t] = m;
        }
    }

    virtual void toData() {
        data.clear();
        std::map<ProductType,
                ProductMetadata>::iterator it = productList.begin();
        while (it != productList.end()) {
            ProductMetadata & m = it->second;
            data.append(m.getData());
            ++it;
        }
    }
};

struct ProductList : public JsonStruct {
    std::vector<ProductMetadata> productList;

    virtual void toFields() {
        productList.clear();
        for (unsigned int i = 0; i < data.size(); ++i) {
            Json::Value & v = data[i];
            ProductMetadata m;
            m.setData(v);
            productList.push_back(m);
        }
    }

    virtual void toData() {
        data.clear();
        for (unsigned int i = 0; i < productList.size(); ++i) {
            data.append(productList.at(i).getData());
        }
    }
};

struct StringList : public JsonStruct {
    std::vector<std::string> items;

    virtual void toFields() {
        items.clear();
        for (unsigned int i = 0; i < data.size(); ++i) {
            items.push_back(data[i].asString());
        }
    }

    virtual void toData() {
        data.clear();
        for (unsigned int i = 0; i < items.size(); ++i) {
            data.append(items.at(i));
        }
    }
};

struct ProductShortList : public StringList {};

struct ParameterList : public JsonStruct {
    std::map<std::string, std::string> paramList;

    virtual void toFields() {
        paramList.clear();
        const Json::Value::Members & members = data.getMemberNames();
        for (unsigned int i = 0; i < members.size(); ++i) {
            paramList[members.at(i)] = data[members.at(i)].asString();
        }
    }

    virtual void toData() {
        data.clear();
        std::multimap<std::string, std::string>::iterator it = paramList.begin();
        while (it != paramList.end()) {
            data[it->first] = it->second;
            ++it;
        }
    }
};

typedef std::string                            TaskName;
typedef std::string                            TaskPath;
typedef int                                    TaskExitCode;
typedef Json::Value                            TaskData;

#define TLIST_TASK_STATUS \
    T(FAILED,  -1), \
    T(FINISHED, 0), \
    T(RUNNING,  1), \
    T(WAITING,  2), \
    T(PAUSED,   3), \
    T(STOPPED,  4)

#define T(a,b) TASK_ ## a = b
enum TaskStatus { TLIST_TASK_STATUS };
#undef T

extern std::map<TaskStatus, std::string> TaskStatusName;
extern std::map<std::string, TaskStatus> TaskStatusValue;

struct TaskInfo : public JsonStruct {
    TaskName           taskName;
    TaskPath           taskPath;
    DateTime           taskStart;
    DateTime           taskEnd;
    TaskExitCode       taskExitCode;
    TaskStatus         taskStatus;
    ProductCollection  inputs;
    ProductCollection  outputs;
    ParameterList      params;
    TaskData           taskData;

    virtual void toFields() {
        taskName     = data["taskName"    ].asString();
        taskPath     = data["taskPath"    ].asString();
        taskStart    = data["taskStart"   ].asString();
        taskEnd      = data["taskEnd"     ].asString();
        taskExitCode = data["taskExitCode"].asInt();
        taskStatus   = static_cast<TaskStatus>(data["taskStatus"  ].asInt());
        inputs .setData(data["inputs" ]);
        outputs.setData(data["outputs"]);
        params .setData(data["params" ]);
        taskData      = data["taskData"];
    }

    virtual void toData() {
        data["taskName"     ] = taskName;
        data["taskPath"     ] = taskPath;
        data["taskStart"    ] = taskStart;
        data["taskEnd"      ] = taskEnd;
        data["taskExitCode" ] = taskExitCode;
        data["taskStatus"   ] = static_cast<int>(taskStatus);
        data["inputs" ]  = inputs .getData();
        data["outputs"]  = outputs.getData();
        data["params" ]  = params .getData();
        data["taskData"] = taskData;
    }
};

struct TaskAgentInfo : public JsonStruct {
    int         total;
    int         maxnum;
    int         running;
    int         waiting;
    int         paused;
    int         stopped;
    int         failed;
    int         finished;
    int         load1min;
    int         load5min;
    int         load15min;
    int         uptimesecs;
    std::string name;
    std::string client;
    std::string server;

    virtual void toFields() {
        total      = data["total"].asInt();
        maxnum     = data["maxnum"].asInt();
        running    = data["running"].asInt();
        waiting    = data["waiting"].asInt();
        failed     = data["failed"].asInt();
        finished   = data["finished"].asInt();
        load1min   = data["load1min"].asInt();
        load5min   = data["load5min"].asInt();
        load15min  = data["load15min"].asInt();
        uptimesecs = data["uptimesecs"].asInt();
        name       = data["name"].asString();
        client     = data["client"].asString();
        server     = data["server"].asString();
    }

    virtual void toData() {
        data["total"]      = total;
        data["maxnum"]     = maxnum;
        data["running"]    = running;
        data["waiting"]    = waiting;
        data["failed"]     = failed;
        data["finished"]   = finished;
        data["load1min"]   = load1min;
        data["load5min"]   = load5min;
        data["load15min"]  = load15min;
        data["uptimesecs"] = uptimesecs;
        data["name"]       = name;
        data["client"]     = client;
        data["server"]     = server;
    }
};

//------------------------------------------------------------
// Message types
//------------------------------------------------------------

typedef int                   MessageId;
typedef int                   MessageVersion;
typedef std::string           NodeName;

struct NodeList : public StringList {};

typedef int                   CRC;

struct MessageHeader : public JsonStruct {
    MessageId      msgId;
    MessageVersion msgVersion;
    NodeName       source;
    NodeName       destination;
    NodeList       path;
    DateTime       dateCreation;
    DateTime       dateTransmission;
    DateTime       dateReception;
    CRC            crc;

    virtual void toFields() {
        msgId            = data["msgId"           ].asInt();
        msgVersion       = data["msgVersion"      ].asInt();
        source           = data["source"          ].asString();
        destination      = data["destination"     ].asString();
        dateCreation     = data["dateCreation"    ].asString();
        dateTransmission = data["dateTransmission"].asString();
        dateReception    = data["dateReception"   ].asString();
        crc              = data["crc"             ].asInt();
        path.setData(data["path"]);
    }

    virtual void toData() {
        data["msgId"           ] = msgId;
        data["msgVersion"      ] = msgVersion;
        data["source"          ] = source;
        data["destination"     ] = destination;
        data["dateCreation"    ] = dateCreation;
        data["dateTransmission"] = dateTransmission;
        data["dateReception"   ] = dateReception;
        data["crc"             ] = crc;
        data["path"] = path.getData();
    }
};

struct Message : public JsonStruct {
    MessageHeader header;
    virtual void toFields() {}
    virtual void toData() {}
};

struct Message_INDATA : public Message {
    ProductCollection    productsMetadata;

    virtual void toFields() {
        header          .setData(data["header"]);
        productsMetadata.setData(data["productcsMetadata"]);
    }
    virtual void toData() {
        data["header"]            = header          .getData();
        data["productcsMetadata"] = productsMetadata.getData();
    }
};

struct Message_DATA_Exchange : public Message {
    ParameterList    variables;

    virtual void toFields() {
        header   .setData(data["header"]);
        variables.setData(data["variables"]);
    }
    virtual void toData() {
        data["header"]    = header   .getData();
        data["variables"] = variables.getData();
    }
};

typedef Message_DATA_Exchange Message_DATA_RQST;
typedef Message_DATA_Exchange Message_DATA_INFO;

typedef Message_DATA_Exchange Message_MONIT_RQST;
typedef Message_DATA_Exchange Message_MONIT_INFO;

struct Message_TASK_Processing : public Message {
    TaskInfo    task;
    std::string rule;

    virtual void toFields() {
        header.setData(data["header"]);
        task  .setData(data["task"]);
        rule = data["rule"].asString();
    }
    virtual void toData() {
        data["header"] = header.getData();
        data["task"]   = task  .getData();
        data["rule"]   = rule;
    }
};

//typedef Message_TASK_Processing Message_TASK_PROC;
//typedef Message_TASK_Processing Message_TASK_RES;
class Message_TASK_PROC : public Message_TASK_Processing {};
class Message_TASK_RES  : public Message_TASK_Processing {};

#define TLIST_MESSAGE_TYPES \
    T(START), \
    T(INDATA), \
    T(DATA_RQST), \
    T(DATA_INFO), \
    T(MONIT_RQST), \
    T(MONIT_INFO), \
    T(TASK_PROC), \
    T(TASK_RES), \
    T(STOP), \
    T(UNKNOWN)

#define T(a) MSG_ ## a ## _IDX
enum MessageTypeIdx { TLIST_MESSAGE_TYPES };
#undef T

#define T(a) MSG_ ## a
extern const std::string TLIST_MESSAGE_TYPES;
#undef T

extern const std::string MessageTypeId[];

extern const int BadMsgProcessing;

struct MessageData {
    MessageData(Message * p = nullptr) : msg(p) {}
    ~MessageData() { reset(); }
    void assign(Message * newPtr = nullptr) { reset(); msg = newPtr; }
    void reset() { if (msg != nullptr) delete msg; msg = nullptr; }
    Message * msg;
};

#define UNUSED(x) (void)(x)

}

#endif  /* MSGTYPES_H */
