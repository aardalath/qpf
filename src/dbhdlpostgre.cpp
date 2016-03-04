/******************************************************************************
 * File:    dbhdlpostgre.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.dbhdlpostgre
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
 *   Implement MessageHandler class
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

#include <iterator>
#include "dbhdlpostgre.h"
#include "except.h"
#include "tools.h"
#include "json/json.h"
#include "config.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//----------------------------------------------------------------------
// Function: quoted
// Single-quotes its argument
//----------------------------------------------------------------------
inline std::string quoted(std::string s) {
    return "'" + s + "'";
}

//----------------------------------------------------------------------
// Function: tagToTimestamp
// Get date and time components from time tag
//----------------------------------------------------------------------
inline std::string tagToTimestamp(std::string tag)
{
    if (tag.empty()) { tag = "20010101T000000"; }
    return std::string(tag.substr(0,4) + "-" +
                       tag.substr(4,2) + "-" +
                       tag.substr(6,2) + " " +
                       tag.substr(9,2) + ":" +
                       tag.substr(11,2) + ":" +
                       tag.substr(13,2));
}

//----------------------------------------------------------------------
// Constructor: DBHdlPostgreSQL
//----------------------------------------------------------------------
DBHdlPostgreSQL::DBHdlPostgreSQL() : conn(0)
{
}

//----------------------------------------------------------------------
// Method: openConnection
// Open a connection to the database
//----------------------------------------------------------------------
bool DBHdlPostgreSQL::openConnection(const char * data)
{
    // Generate new connection to database
    const char * connStr = data;
    if (data == 0) {
        if (!connectionParamsSet) {
            setDbHost(Configuration::DBHost);
            setDbPort(Configuration::DBPort);
            setDbName(Configuration::DBName);
            setDbUser(Configuration::DBUser);
            setDbPasswd(Configuration::DBPwd);

            setDbConnectionParams("host=" + getDbHost() +
                                  " port=" + getDbPort() +
                                  " dbname=" + getDbName() +
                                  " user=" + getDbUser() +
                                  " password=" + getDbPasswd());
        }
        connStr = getDbConnectionParams().c_str();
    }
    if (conn != 0) {
        char thid[10];
        sprintf(thid, "%ld", pthread_self());
        std::string msg = (std::string("Connection will be re-open at thread ") +
                           std::string(thid));
        PQfinish(conn);
        throw RuntimeException(msg);
    }
    conn = PQconnectdb(connStr);
    if (PQstatus(conn) != CONNECTION_OK) {
        std::string msg = (std::string("Connection to database failed: ") +
                           std::string(PQerrorMessage(conn)));
        PQfinish(conn);
        throw RuntimeException(msg);
    }
    return true;
}

//----------------------------------------------------------------------
// Method: closeConnection
// Closes the connection to the database
//----------------------------------------------------------------------
bool DBHdlPostgreSQL::closeConnection(const char * data)
{
    UNUSED(data);

    PQfinish(conn);
    return true;
}

//----------------------------------------------------------------------
// Method: storeProducts
// Saves a set of products' metadata to the database
//----------------------------------------------------------------------
int  DBHdlPostgreSQL::storeProducts(ProductCollection & prodList)
{
    bool result;

    int nInsProd = 0;
    std::stringstream ss;

    for (auto & kv : prodList.productList) {
        ProductMetadata & p = kv.second;

        ss.str("");
        ss << "INSERT INTO products_info "
           << "(product_id, product_type, product_status_id, product_version, product_size, creator_id, "
           << "instrument_id, obsmode_id, start_time, end_time, registration_time, url) "
           << "VALUES ("
           << quoted(p.productId) << ", "
           << quoted(p.productType) << ", "
           << 0 << ", "
           << quoted(p.productVersion) << ", "
           << p.productSize << ", "
           << 1 << ", "
           << 1 << ", "
           << 1 << ", "
           << quoted(tagToTimestamp(p.startTime)) << ", "
           << quoted(tagToTimestamp(p.endTime)) << ", "
           << quoted(tagToTimestamp(LibComm::timeTag())) << ", "
           << quoted(p.url) << ")";

        try { result = runCmd(ss.str()); } catch(...) { throw; }

        PQclear(res);
        nInsProd++;
    }

    UNUSED(result);

    return nInsProd;
}

//----------------------------------------------------------------------
// Method: retrieveProducts
// Retrieves a set of products from the database, according to
// pre-defined criteria
//----------------------------------------------------------------------
int  DBHdlPostgreSQL::retrieveProducts(ProductList & prodList,
                                       std::string criteria)
{
    bool result;
    std::string cmd(
                "SELECT p.product_id, p.product_type, s.status_desc, p.product_version, "
                "p.product_size, c.creator_desc, i.instrument, m.obsmode_desc, "
                "p.start_time, p.end_time, p.registration_time, p.url "
                "FROM (((products_info AS p "
                "  INNER JOIN creators AS c "
                "  ON p.creator_id = c.creator_id) "
                "    INNER JOIN instruments AS i "
                "    ON p.instrument_id = i.instrument_id) "
                "      INNER JOIN observation_modes AS m "
                "      ON p.obsmode_id = m.obsmode_id) "
                "        INNER JOIN product_status AS s "
                "        ON p.product_status_id = s.product_status_id "
                "ORDER BY p.id ");
    cmd += criteria + ";";

    try { result = runCmd(cmd); } catch(...) { throw; }

    // Transfer the data to the product list argument
    prodList.productList.clear();
    ProductMetadata m;
    int nRows = PQntuples(res);
    for (int i = 0; i < nRows; ++i) {
        m.productId      = std::string(PQgetvalue(res, i, 0));
        m.productType    = std::string(PQgetvalue(res, i, 1));
        m.productStatus  = std::string(PQgetvalue(res, i, 2));
        m.productVersion = std::string(PQgetvalue(res, i, 3));
        m.productSize    =    *((int*)(PQgetvalue(res, i, 4)));
        m.creator        = std::string(PQgetvalue(res, i, 5));
        m.instrument     = std::string(PQgetvalue(res, i, 6));
        m.obsMode        = std::string(PQgetvalue(res, i, 7));
        m.startTime      = std::string(PQgetvalue(res, i, 8));
        m.endTime        = std::string(PQgetvalue(res, i, 9));
        m.regTime        = std::string(PQgetvalue(res, i, 10));
        m.url            = std::string(PQgetvalue(res, i, 11));
        prodList.productList.push_back(m);
    }
    PQclear(res);

    UNUSED(result);

    return nRows;
}

//----------------------------------------------------------------------
// Method: storeTask
// Stores a task information to the database
//----------------------------------------------------------------------
bool DBHdlPostgreSQL::storeTask(TaskInfo & task)
{
    bool result = true;

    std::string registrationTime("2004-10-19 10:23:54");
    std::stringstream ss;
    std::string taskPath = task.taskPath;
    Json::StyledWriter writer;
    std::string taskData("{}");
    if (!task.taskData.isNull()) {
        taskData = writer.write(task.taskData);
    }

    ss.str("");
    ss << "INSERT INTO tasks_info "
       << "(task_id, task_status_id, task_exitcode, task_path, "
       << "task_size, registration_time, task_data) "
       << "VALUES ("
       << quoted(task.taskName) << ", "
       << task.taskStatus << ", "
       << task.taskExitCode << ", "
       << quoted(taskPath) << ", "
       << 0 << ", "
       << quoted(registrationTime) << ", "
       << quoted(taskData) << ")";

    try { result = runCmd(ss.str()); } catch(...) { throw; }

    PQclear(res);

    return result;
}

//----------------------------------------------------------------------
// Method: checkTask
// Returns true if an entry for a task exists in the DB
//----------------------------------------------------------------------
bool DBHdlPostgreSQL::checkTask(TaskInfo & task)
{
    bool result;
    std::string cmd("SELECT t.task_id FROM tasks_info AS t "
                    "WHERE t.task_id=" + quoted(task.taskName) + ";");

    try { result = runCmd(cmd); } catch(...) { throw; }

    result &= (PQntuples(res) > 0);
    PQclear(res);

    return result;
}

//----------------------------------------------------------------------
// Method: updateTable<int>
// Instantiation of template updateTable<> for ints
//----------------------------------------------------------------------
template<>
bool DBHdlPostgreSQL::updateTable<int>(std::string table, std::string cond,
                                       std::string field, int value)
{
    return updTable(table, cond, field, LibComm::toStr<int>(value));
}

//----------------------------------------------------------------------
// Method: updateTable<int>
// Instantiation of template updateTable<> for doubles
//----------------------------------------------------------------------
template<>
bool DBHdlPostgreSQL::updateTable<double>(std::string table, std::string cond,
                                          std::string field, double value)
{
    return updTable(table, cond, field, LibComm::toStr<double>(value));
}

//----------------------------------------------------------------------
// Method: updateTable<int>
// Instantiation of template updateTable<> for char strings
//----------------------------------------------------------------------
template<>
bool DBHdlPostgreSQL::updateTable<std::string>(std::string table, std::string cond,
                                               std::string field, std::string value)
{
    return updTable(table, cond, field, quoted(value));
}

//----------------------------------------------------------------------
// Method: updateTable<int>
// Instantiation of template updateTable<> for JSON values
//----------------------------------------------------------------------
template<>
bool DBHdlPostgreSQL::updateTable<Json::Value>(std::string table, std::string cond,
                                               std::string field, Json::Value value)
{
    Json::StyledWriter writer;
    return updTable(table, cond, field, quoted(writer.write(value)));
}

//----------------------------------------------------------------------
// Method: updateTask
// Updates the information for a given task in the database
//----------------------------------------------------------------------
bool DBHdlPostgreSQL::updateTask(TaskInfo & task)
{
    bool result = true;

    if (checkTask(task)) {
        std::string filter("task_id=" + quoted(task.taskName));
        result &= updateTable<int>("tasks_info", filter,
                                   "task_status_id", (int)(task.taskStatus));
        result &= updateTable<std::string>("tasks_info", filter,
                                           "start_time", task.taskStart);
        if (task.taskStatus == TASK_FINISHED) {
            result &= updateTable<std::string>("tasks_info", filter,
                                               "end_time", task.taskEnd);
        }
        result &= updateTable<Json::Value>("tasks_info", filter,
                                           "task_data", task.taskData);
        PQclear(res);
    } else {
        result = storeTask(task);
    }

    return result;
}

//----------------------------------------------------------------------
// Method: retrieveTask
// Retrieves a task information from the database
//----------------------------------------------------------------------
bool DBHdlPostgreSQL::retrieveTask(TaskInfo & task)
{
    UNUSED(task);

    return false;
}

//----------------------------------------------------------------------
// Method: storeMsg
// Stores a message into the database
//----------------------------------------------------------------------
bool DBHdlPostgreSQL::storeMsg(std::string from,
                               Router2RouterPeer::PeerMessage & msg,
                               bool isBroadcast)
{
    bool result = true;
    if (from.empty()) { from = "all"; }

    std::string cmd("INSERT INTO transmissions "
                    "(msg_date, msg_from, msg_to, msg_type, msg_bcast, msg_content) "
                    "VALUES (" +
                    quoted(tagToTimestamp(LibComm::timeTag())) + ", " +
                    quoted(from) + ", " +
                    quoted(msg.at(Router2RouterPeer::FRAME_PEER_ID)) + ", " +
                    quoted(msg.at(Router2RouterPeer::FRAME_MSG_TYPE)) + ", " +
                    quoted(isBroadcast ? "Y" : "N") + ", " +
                    quoted(msg.at(Router2RouterPeer::FRAME_MSG_CONTENT)) + ")");

    try { result = runCmd(cmd); } catch(...) { throw; }

    return result;
}

//----------------------------------------------------------------------
// Method: retrieveMsgs
// Retrieves a set of messages from the database, according to
// pre-defined criteria
//----------------------------------------------------------------------
bool DBHdlPostgreSQL::retrieveMsgs(std::vector<std::pair<std::string,
                                   Router2RouterPeer::PeerMessage> > & msgList,
                                   std::string criteria)
{
    bool result = true;

    std::string cmd(
                "SELECT m.msg_date, m.msg_to, m.msg_type, m.msg_content "
                "FROM transmissions as m "
                "ORDER BY m.msg_date ");
    cmd += criteria + ";";

    try { result = runCmd(cmd); } catch(...) { throw; }

    // Transfer the data to the msgList argument
    msgList.clear();
    int nRows = PQntuples(res);
    for (int i = 0; i < nRows; ++i) {
        std::string msg_date(PQgetvalue(res, i, 1));
        Router2RouterPeer::PeerMessage m;
        m.push_back(std::string(PQgetvalue(res, i, 2)));
        m.push_back(std::string(PQgetvalue(res, i, 3)));
        m.push_back(std::string(PQgetvalue(res, i, 4)));
        msgList.push_back(std::make_pair(msg_date, m));
    }
    PQclear(res);

    return result;
}

//----------------------------------------------------------------------
// Method: getTable
// Get the content (all the records) from a given table
//----------------------------------------------------------------------
bool DBHdlPostgreSQL::getTable(std::string tName,
                               std::vector< std::vector<std::string> > & table)
{
    return  (runCmd(std::string("SELECT * FROM " + tName)) && fillWithResult(table));
}

//----------------------------------------------------------------------
// Method: getQuery
// Get the result (all the records) of a given query
//----------------------------------------------------------------------
bool DBHdlPostgreSQL::getQuery(std::string qry,
                               std::vector< std::vector<std::string> > & table)
{
    return  (runCmd(qry) && fillWithResult(table));
}

//----------------------------------------------------------------------
// Method: getNumRowsInTable
// Get the content (all the records) from a given table
//----------------------------------------------------------------------
int DBHdlPostgreSQL::getNumRowsInTable(std::string tName)
{
    int nRows = -1;
    if (runCmd(std::string("SELECT COUNT(*) FROM " + tName))) {
        nRows = atoi(PQgetvalue(res, 0, 0));
    }
    PQclear(res);
    return nRows;
}

//----------------------------------------------------------------------
// Method: runCmd
// Runs a SQL command
//----------------------------------------------------------------------
bool DBHdlPostgreSQL::runCmd(std::string cmd)
{
    // Run the command
    res = PQexec(conn, cmd.c_str());
    if ((PQresultStatus(res) != PGRES_COMMAND_OK) &&
            (PQresultStatus(res) != PGRES_TUPLES_OK)) {
        std::string msg = (std::string("Failed cmd '" + cmd + "': ") +
                           std::string(PQerrorMessage(conn)));
        PQclear(res);
        PQfinish(conn);
        throw RuntimeException(msg);
    }
    return true;
}

//----------------------------------------------------------------------
// Method: fillWithResult
// Retrieves the content of the returned n-tuples (after and SQL command)
// into the table parameter
//----------------------------------------------------------------------
bool DBHdlPostgreSQL::fillWithResult(std::vector< std::vector<std::string> > & table)
{
    // Transfer the data to the table argument
    table.clear();
    int nFields = PQnfields(res);
    int nRows = PQntuples(res);
    for (int i = 0; i < nRows; ++i) {
        std::vector<std::string> row;
        for (int j = 0; j < nFields; ++j) {
            row.push_back(std::string(PQgetvalue(res, i, j)));
        }
        table.push_back(row);
    }
    PQclear(res);

    return true;
}

//----------------------------------------------------------------------
// Method: updTable
// Update a single field in a given table
//----------------------------------------------------------------------
bool DBHdlPostgreSQL::updTable(std::string table, std::string cond,
                               std::string field, std::string value)
{
    std::string cmd("UPDATE " + table + " SET " + field + " = " + value);
    if (!cond.empty()) {
        cmd += " WHERE " + cond;
    }
    return runCmd(cmd);
}

}
