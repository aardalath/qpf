/******************************************************************************
 * File:    datatypes.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.datatypes
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
 *   Implement Msgdatatypes class
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

#include "datatypes.h"

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

//==== JsonStruct

JsonStruct::JsonStruct() : data(Json::nullValue)
{
}

JsonStruct::~JsonStruct()
{
}

JsonStruct & JsonStruct::operator=(const JsonStruct& other)
{ // copy assignment
    if (this != &other) { // self-assignment check expected
        const_cast<JsonStruct&>(other).toData();
        this->data = other.data;
        this->toFields();
    }
    return *this;
}

void JsonStruct::setData(Json::Value & v) {
    data = v;
//        Json::StyledWriter w;
//        std::cerr << w.write(v) << std::endl;
    toFields();
}

void JsonStruct::setData(std::string key, Json::Value & v) {
    data[key] = v;
    toFields();
}

void JsonStruct::setDataString(std::string vStr) {
    Json::Reader reader;
    reader.parse(vStr, data);
    toFields();
}

Json::Value & JsonStruct::getData() {
    toData();
    return data;
}

std::string JsonStruct::getDataString() {
    toData();
    Json::FastWriter writer;
    return writer.write(data);
}

//==== Friend output operator

std::ostream & operator<<(std::ostream &out, const JsonStruct & c) {
    out << const_cast<JsonStruct & >(c).getDataString() << std::endl << std::flush;
    return out;
}

//==== ProductCollection

void ProductMetadata::toFields() {
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
    signature      = data["signature"     ].asString();
    url            = data["url"           ].asString();
    urlSpace       = data["urlSpace"      ].asString();
}

void ProductMetadata::toData() {
    data["startTime"     ] = startTime;
    data["endTime"       ] = endTime;
    data["instrument"    ] = instrument;
    data["obsMode"       ] = obsMode;
    data["creator"       ] = creator;
    data["productId"     ] = productId;
    data["productType"   ] = productType;
    data["productVersion"] = productVersion;
    data["productStatus" ] = productStatus;
    data["productSize"   ] = productSize;
    data["signature"     ] = signature;
    data["url"           ] = url;
    data["urlSpace"      ] = urlSpace;
}

//==== ProductCollection

void ProductCollection::toFields() {
    productList.clear();
    for (unsigned int i = 0; i < data.size(); ++i) {
        Json::Value & v = data[i];
        ProductType t = v["productType"].asString();
        ProductMetadata m;
        m.setData(v);
        productList[t] = m;
    }
}

void ProductCollection::toData() {
    data.clear();
    std::map<ProductType,
             ProductMetadata>::iterator it = productList.begin();
    while (it != productList.end()) {
        ProductMetadata & m = it->second;
        data.append(m.getData());
        ++it;
    }
}

//==== ProductList

void ProductList::toFields() {
    productList.clear();
    for (unsigned int i = 0; i < data.size(); ++i) {
        Json::Value & v = data[i];
        ProductMetadata m;
        m.setData(v);
        productList.push_back(m);
    }
}

void ProductList::toData() {
    data.clear();
    for (unsigned int i = 0; i < productList.size(); ++i) {
        data.append(productList.at(i).getData());
    }
}

//==== StringList

void StringList::toFields() {
    items.clear();
    for (unsigned int i = 0; i < data.size(); ++i) {
        items.push_back(data[i].asString());
    }
}

void StringList::toData() {
    data.clear();
    for (unsigned int i = 0; i < items.size(); ++i) {
        data.append(items.at(i));
    }
}

//==== ProductShortList

/* none */

//==== ParameterList

void ParameterList::toFields() {
    paramList.clear();
    const Json::Value::Members & members = data.getMemberNames();
    for (unsigned int i = 0; i < members.size(); ++i) {
        paramList[members.at(i)] = data[members.at(i)].asString();
    }
}

void ParameterList::toData() {
    data.clear();
    std::multimap<std::string, std::string>::iterator it = paramList.begin();
    while (it != paramList.end()) {
        data[it->first] = it->second;
        ++it;
    }
}

//==== TaskInfo

TaskInfo::TaskInfo() : taskData(Json::nullValue)
{
}

void TaskInfo::toFields() {
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

void TaskInfo::toData() {
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

//==== TaskAgentInfo

TaskAgentInfo::TaskAgentInfo() :
    total(0),
    maxnum(0),
    running(0),
    waiting(0),
    paused(0),
    stopped(0),
    failed(0),
    finished(0),
    load1min(0),
    load5min(0),
    load15min(0),
    uptimesecs(0)
{
}

void TaskAgentInfo::toFields() {
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

void TaskAgentInfo::toData() {
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

//==== NodeList

/* none */

//==== MessageHeader

void MessageHeader::toFields() {
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

void MessageHeader::toData() {
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

//==== Message

void Message::toFields()
{
}

void Message::toData()
{
}

//==== Message_INDATA

void Message_INDATA::toFields() {
    header          .setData(data["header"]);
    productsMetadata.setData(data["productcsMetadata"]);
}

void Message_INDATA::toData() {
    data["header"]            = header          .getData();
    data["productcsMetadata"] = productsMetadata.getData();
}

//==== Message_DATA_Exchange

void Message_DATA_Exchange::toFields() {
    header   .setData(data["header"]);
    variables.setData(data["variables"]);
}

void Message_DATA_Exchange::toData() {
    data["header"]    = header   .getData();
    data["variables"] = variables.getData();
}

//==== Message_TASK_Processing

void Message_TASK_Processing::toFields() {
    header.setData(data["header"]);
    task  .setData(data["task"]);
    rule = data["rule"].asString();
}

void Message_TASK_Processing::toData() {
    data["header"] = header.getData();
    data["task"]   = task  .getData();
    data["rule"]   = rule;
}

//==== MessageData

MessageData::MessageData(Message * p) : msg(p)
{
}

MessageData::~MessageData()
{
    reset();
}

void MessageData::assign(Message * newPtr)
{
    reset();
    msg = newPtr;
}

void MessageData::reset()
{
    if (msg != nullptr) delete msg;
    msg = nullptr;
}

}
