/******************************************************************************
 * File:    datatypes.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.datatypes
 *
 * Version:  1.2
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015,2016,2017 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declare Msgdatatypes class
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

#ifndef DATATYPES_H
#define DATATYPES_H

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
#include <set>
#include <iostream>
//#include <memory>

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
// Topic: Basic datatypes
//------------------------------------------------------------

typedef std::string   ProductType;
typedef std::string   DateTime;
typedef std::string   DateRange;
typedef std::string   URL;
typedef std::string   URLSpace;
typedef unsigned int  ObsId;
typedef std::string   Instrument;
typedef std::string   ObsMode;
typedef unsigned int  Exposure;
typedef std::string   Creator;
typedef std::string   Mission;
typedef std::string   Origin;
typedef std::string   ProcFunction;
typedef std::string   FileType;
typedef std::string   ProductParams;
typedef std::string   ProductId;
typedef std::string   ProductVersion;
typedef std::string   ProductStatus;
typedef unsigned int  ProductSize;
typedef std::string   Signature;
typedef std::string   FileNamePart;

//------------------------------------------------------------
// Topic: Structures
//------------------------------------------------------------

struct JsonStruct {
    JsonStruct();
    virtual ~JsonStruct();

    JsonStruct & operator=(const JsonStruct& other);

    virtual void setData(Json::Value & v);
    virtual void setData(std::string key, Json::Value & v);
    virtual void setDataString(std::string vStr);
    virtual Json::Value & getData();
    virtual std::string getDataString();
    virtual void toFields() = 0;
    virtual void toData() = 0;

    friend std::ostream &operator<<(std::ostream &out, const JsonStruct & c);

    Json::Value data;
};

struct ProductMetadata : public JsonStruct {
    Mission        mission;        // %M
    DateTime       startTime;      // %f
    DateTime       endTime;        // %t
    DateTime       regTime;
    DateRange      timeInterval;   // %D
    ObsId          obsId;
    Instrument     instrument;     // %I
    ObsMode        obsMode;
    Exposure       expos;
    Creator        creator;
    Origin         origin;
    ProcFunction   procFunc;       // %F
    FileType       fileType;
    ProductParams  params;         // %P
    ProductId      productId;
    ProductType    productType;    // %T
    ProductVersion productVersion; // %V
    ProductStatus  productStatus;
    ProductSize    productSize;
    Signature      signature;      // %S
    FileNamePart   dirName;
    FileNamePart   baseName;
    FileNamePart   suffix;
    FileNamePart   extension;
    URL            url;
    URLSpace       urlSpace;
    bool           hadNoVersion;

    virtual void toFields();
    virtual void toData();

#ifdef DEBUG_BUILD
    static void dump(std::ostream & os, ProductMetadata * o) {
        os << "mission        : " << o->mission << std::endl;
        os << "creator        : " << o->creator << std::endl;
        os << "origin         : " << o->origin << std::endl;
        os << "procFunc       : " << o->procFunc << std::endl;
        os << "params         : " << o->params << std::endl;
        os << "instrument     : " << o->instrument << std::endl;
        os << "obsId          : " << o->obsId << std::endl;
        os << "obsMode        : " << o->obsMode << std::endl;
        os << "expos          : " << o->expos << std::endl;
        os << "productType    : " << o->productType << std::endl;
        os << "signature      : " << o->signature << std::endl;
        os << "productId      : " << o->productId << std::endl;
        os << "productVersion : " << o->productVersion << std::endl;
        os << "productStatus  : " << o->productStatus << std::endl;
        os << "startTime      : " << o->startTime << std::endl;
        os << "endTime        : " << o->endTime << std::endl;
        os << "regTime        : " << o->regTime << std::endl;
        os << "productSize    : " << o->productSize << std::endl;
        os << "fileType       : " << o->fileType << std::endl;
        os << "url            : " << o->url << std::endl;
        os << "urlSpace       : " << o->urlSpace << std::endl;
    }
#endif
};

struct ProductCollection : public JsonStruct {
    std::map<ProductType, ProductMetadata> productList;

    virtual void toFields();
    virtual void toData();
};

struct ProductList : public JsonStruct {
    std::vector<ProductMetadata> productList;

    virtual void toFields();
    virtual void toData();
};

struct StringList : public JsonStruct {
    std::vector<std::string> items;

    virtual void toFields();
    virtual void toData();
};

struct ProductShortList : public StringList {};

typedef std::map<std::string, std::string> PList;

struct ParameterList : public JsonStruct {
    PList paramList;

    virtual void toFields();
    virtual void toData();
};

typedef std::string                            TaskName;
typedef std::string                            TaskPath;
typedef int                                    TaskExitCode;
typedef Json::Value                            TaskData;

#define TLIST_TASK_STATUS \
    T(SCHEDULED, -2), \
    T(FAILED,    -1), \
    T(FINISHED,   0), \
    T(RUNNING,    1), \
    T(PAUSED,     2), \
    T(STOPPED,    3), \
    T(ARCHIVED,   4), \
    T(UNKNOWN_STATE, 5)

#define T(a,b) TASK_ ## a = b
enum TaskStatus { TLIST_TASK_STATUS };
#undef T

extern std::map<TaskStatus, std::string> TaskStatusName;
extern std::map<std::string, TaskStatus> TaskStatusValue;

struct TaskInfo : public JsonStruct {
    TaskInfo();
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

    virtual void toFields();
    virtual void toData();
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

    TaskAgentInfo();

    virtual void toFields();
    virtual void toData();
};

//------------------------------------------------------------
// Message datatypes
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

    virtual void toFields();
    virtual void toData();
};

struct Message : public JsonStruct {
    MessageHeader header;
    virtual void toFields();
    virtual void toData();
};

struct Message_INDATA : public Message {
    ProductCollection    productsMetadata;

    virtual void toFields();
    virtual void toData();
};

struct Message_DATA_Exchange : public Message {
    ParameterList    variables;

    virtual void toFields();
    virtual void toData();
};

typedef Message_DATA_Exchange Message_DATA_RQST;
typedef Message_DATA_Exchange Message_DATA_INFO;

typedef Message_DATA_Exchange Message_MONIT_RQST;
typedef Message_DATA_Exchange Message_MONIT_INFO;

typedef Message_DATA_Exchange Message_CMD;

struct Message_TASK_Processing : public Message {
    TaskInfo    task;
    std::string rule;

    virtual void toFields();
    virtual void toData();
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
    T(CMD), \
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
    MessageData(Message * p = nullptr);
    ~MessageData();
    void assign(Message * newPtr = nullptr);
    void reset();

    Message * msg;
};

#define UNUSED(x) (void)(x)

}

#endif  /* DATATYPES_H */
