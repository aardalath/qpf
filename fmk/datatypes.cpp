/******************************************************************************
 * File:    component.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.component
 *
 * Version:  2.0
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implement Component class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   none
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

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
//namespace QPF {

json nullJson = JValue(std::string({})).val();

#define T(a,b) { TASK_ ## a , #a }
std::map<TaskStatus, std::string> TaskStatusName = { TLIST_TASK_STATUS };
#undef T

#define T(a,b) { #a , TASK_ ## a }
std::map<std::string, TaskStatus> TaskStatusValue = { TLIST_TASK_STATUS };
#undef T

JValue::JValue()
{
}

JValue::JValue(json val)
{
    value = val; 
}

JValue::JValue(std::string s)
{
    fromStr(s); 
}

JValue JValue::operator()(std::string key)
{
    return JValue(value[key]); 
}

json & JValue::operator[](std::string key)
{
    return value[key]; 
}

std::string JValue::str(bool styled)
{
    if (value.isObject() || value.isArray()) {
        if (! styled) {
            Json::FastWriter w;
            return w.write(value);
        } else {
            Json::StyledWriter w;
            return w.write(value);
        }
    } else {
        return value.asString();
    }
}

std::string JValue::sortedStr(std::vector<std::string> sortKeys)
{
    json sortedValue;
    char newKey[100];
    int i = 0;
    for (auto & key : sortKeys) {
        sprintf(newKey, "SORTEDKEY:%03d-%s", i++, key.c_str());
        sortedValue[newKey] = value[key];
    }
    Json::StyledWriter w;
    std::string result = w.write(sortedValue);
    std::size_t pos = 0;       
    while ((pos = result.find("SORTEDKEY:", pos)) != std::string::npos) {
        result.erase(pos, 14);
    }
    return result;
}

void JValue::fromStr(std::string content)
{
    Json::Reader reader;
    reader.parse(content, value);
}

json & JValue::val()
{
    return value; 
}

int JValue::size()
{
    return value.size(); 
}

bool JValue::has(const char * key)
{
    return value.isMember(key); 
}

//}
