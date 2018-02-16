/******************************************************************************
 * File:    httpserver.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.httpserver
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
 *   Implement HttpServer class
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

#include "httpserver.h"

#include "log.h"
#include "tools.h"
#include "str.h"

#include "config.h"

using Configuration::cfg;

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
//namespace QPF {

class WebComposer {
private:
    static const std::string PageBegin;
    static const std::string PageEnd;
    static const std::string HeadBegin;
    static const std::string HeadEnd;
    static const std::string BodyBegin;
    static const std::string BodyEnd;
    static const std::string Style;
    static const std::string FootMsg;

public:
    enum Column { Center, Right, Left };

private:
    std::map<Column,std::string> content;
    std::string title;

public:    
    void reset() {
        content.clear();
        title = "New Page";
    }

    void setTitle(std::string tit) {
        title = tit;
    }
    
    void addHeading(std::string heading, int level, Column col = Center) {
        content[col] += ("<h" + std::to_string(level) + ">" + heading +
                         "</h" + std::to_string(level) + ">\n");
    }
    
    void addPar(std::string par, Column col = Center) {
        content[col] += "<p>" + par + "</p>\n";
    }
    
    void addPre(std::string txt, Column col = Center) {
        content[col] += "<pre>" + txt + "</pre>\n";
    }
    
    void addSpace(Column col = Center) {
        content[col] += "<p><br></br></p>\n";
    }
    
    void addSec(std::string sec) {
        content[Center] += "<div class=\"banner\">" + sec + "</div>\n";
    }
    
    void startMenu(std::string tit, Column col = Left) {
        content[col] += ("<p><strong>" + tit + "</strong><br/></p>\n" +
                         "<div id=\"navigation\"><ul>\n");
    }

    void addMenuItem(std::string txt, std::string url, Column col = Left) {
        content[col] += ("<li><a href=\"" + url + "\">" + txt + "</a></li>\n");
    }

    void endMenu(Column col = Left) {
        content[col] += ("</ul></div>\n");
    }

    void addImg(std::string fileName, int height = 104, int width = 125,
                std::string url = std::string(),
                Column col = Center,
                std::string title = std::string(),
                std::string cls = "thumbnail") {
        content[col] += ("<p><a href=\"" + url + "\"><img src=\"" + fileName + "\" " +
                         "height=\"" + std::to_string(height) +
                         "\" width=\"" + std::to_string(width) +
                         "\" class=\"" + cls + "\" alt=\"" + title +
                         "\" align=\"middle\"/></a></p>\n");
    }

    void begTable(Column col = Center) { 
        content[col] += "<blockquote><table cellspacing=\"0\" cellpadding=\"0\">\n";
    }
    void endTable(Column col = Center) { 
        content[col] += "</table></blockquote>\n"; 
    }
    
    void begTRow(Column col = Center) { content[col] += "<tr>"; }
    void endTRow(Column col = Center) { content[col] += "</tr>\n"; }
    
    void addTCell(std::string s, Column col = Center) { content[col] += "<td>" + s + "</td>"; }
    void addHCell(std::string s, Column col = Center) { content[col] += "<td id=\"heading\">" + s + "</td>"; }

    void addHTML(std::string html, Column col = Center) {
        content[col] += html;
    }
    
    std::string getPage() {
        std::string page = (PageBegin +
                            HeadBegin + "<title>" + title + "</title>" + Style + HeadEnd +
                            BodyBegin);
        page += "<div id=\"right1\">\n" + content[Left] + "</div>\n"; 
        page += "<div id=\"center\">\n" + content[Center] + FootMsg + "</div>\n"; 
        page += "<div id=\"right2\">\n" + content[Right] + "</div>\n"; 
        page += BodyEnd + PageEnd;
        return page;
    }
};

const std::string WebComposer::PageBegin = std::string
    ("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"DTD/xhtml1-transitional.dtd\">\n"
     "<html>\n");
const std::string WebComposer::PageEnd = std::string
    ("</html>\n");
const std::string WebComposer::HeadBegin = std::string
    ("<head>");
const std::string WebComposer::HeadEnd = std::string
    ("</head>\n");
const std::string WebComposer::BodyBegin = std::string
    ("<body>");
const std::string WebComposer::BodyEnd = std::string
    ("</body>\n");
const std::string WebComposer::Style = std::string
    ("<style>\n"
     "      body {\n"
     "        margin:20px 10px 0px 10px;\n"
     "        padding:0px;\n"
     "        font-family: Arial, Helvetica, sans-serif;\n"
     "        font-size: 1em;\n"
     "        color: #666;\n"
     "      }\n"
     "      #center {\n"
     "        width:55%;\n"
     "        float:left;\n"
     "        background:#fff;\n"
     "        padding-bottom:10px;\n"
     "        font-family: Arial, Helvetica, sans-serif;\n"
     "        font-size: 0.65em;\n"
     "        color: #666;\n"
     "        line-height: 1.2em;\n"
     "      }\n"
     "      #right1 {\n"
     "        width:20%;\n"
     "        float:left;\n"
     "        background:#eee;\n"
     "        padding-bottom:10px;\n"
     "        line-height: 1.2em;\n"
     "        font-family: Arial, Helvetica, sans-serif;\n"
     "        font-size: 0.65em;\n"
     "        color: #666;\n"
     "        border-right: 1px solid;\n"
     "        border-color: #ccc;\n"
     "      }\n"
     "      #right2 {\n"
     "        width:20%;\n"
     "        float:left;\n"
     "        background:#fff;\n"
     "        padding-bottom:10px;\n"
     "        border-left: 1px solid #eee;\n"
     "        font-family: Arial, Helvetica, sans-serif;\n"
     "        font-size: 0.65em;\n"
     "        color: #666;\n"
     "      }\n"
     "      .banner {\n"
     "        background:#6493C4;\n"
     "        margin-left: 10px;\n"
     "        margin-right: 10px;\n"
     "        font-family: Arial, Helvetica, sans-serif;\n"
     "        font-size: 1em;\n"
     "        color: #333333;\n"
     "        font-weight: bold;\n"
     "        text-transform: uppercase;\n"
     "      }\n"
     "      #footer {\n"
     "        padding-top: 10px;\n"
     "        margin-left: 10px;\n"
     "        margin-right: 10px;\n"
     "        font-family: Arial, Helvetica, sans-serif;\n"
     "        font-size: 0.8em;\n"
     "        color: #999;\n"
     "        font-weight: normal;\n"
     "        text-transform: none;\n"
     "        text-align: center;\n"
     "      }\n"
     "      #navigation p {\n"
     "        display: none;\n"
     "      }\n"
     "      #navigation {\n"
     "        color: #8A1E30;\n"
     "        text-decoration: none;\n"
     "        font-family: Arial,Helvetica,sans-serif;\n"
     "        font-size: 1em;\n"
     "        font-weight: bold;\n"
     "        padding: 0;\n"
     "        margin-bottom: 1em;\n"
     "        background-color: #eee;\n"
     "      }\n"
     "      #navigation ul {\n"
     "        list-style: none;\n"
     "        margin: 0;\n"
     "        padding: 0;\n"
     "        border-top: 1px dotted #ccc;\n"
     "      }\n"
     "      #navigation ul li {\n"
     "        margin: 0;\n"
     "      }\n"
     "      #navigation ul li a {\n"
     "        display: block;\n"
     "        padding: 2px 2px 2px 1em;\n"
     "        background-color: #eee;\n"
     "        text-decoration: none;\n"
     "        width: 100%;\n"
     "      }\n"
     "      html>body #navigation ul li a {\n"
     "        width:auto;\n"
     "      }\n"
     "      #navigation ul li a:hover {\n"
     "        background-color: #8EABD1;\n"
     "        color: #fff;\n"
     "      }\n"
     "      p,h1,pre {\n"
     "        margin:0px 10px 10px 10px;\n"
     "      }\n"
     "      a {\n"
     "        color: #5578AA;\n"
     "        text-decoration: none;\n"
     "        font-family: Arial,Helvetica,sans-serif;\n"
     "        font-size: 1.2em;\n"
     "        font-weight: bold\n"
     "      }\n"
     "      a:hover {\n"
     "        text-decoration: underline;\n"
     "      }\n"
     "      a:visited:hover {\n"
     "        color: #e7eff2;\n"
     "        text-decoration: underline;\n"
     "      }\n"
     "      h1 {\n"
     "        font-size:12px;\n"
     "        padding-top:10px;\n"
     "      }\n"
     "      table {\n"
     "        border-collapse: collapse;\n"
     "      }\n"
     "      table, th, td {\n"
     "        border: 1px solid gray;\n"
     "      }\n"
     "      th, td {\n"
     "        padding: 5px;\n"
     "      }\n"
     "      #heading {\n"
     "        background:#86B5D6;\n"
     "        color: white;\n"
     "        font-weight: bold;\n"
     "      }\n"
     "      #center p {\n"
     "        padding-left: 10px;\n"
     "      }\n"
     "      #right1 a {\n"
     "        font-size: 1.1em;\n"
     "      }\n"
     "      #right2 a {\n"
     "        font-size: 1.1em;\n"
     "      }\n"
     "      #center h1 {\n"
     "        font-family: Arial,Helvetica,sans-serif;\n"
     "        font-size: 30px;\n"
     "        font-weight: normal;\n"
     "        color: #6493C4;\n"
     "        padding-left: 10px;\n"
     "        margin-top: 10px;\n"
     "        margin-bottom: 20px;\n"
     "      }\n"
     "      #center h2 { font-family: Arial,Helvetica,sans-serif;\n"
     "        font-size: 18px;\n"
     "        font-weight: normal;\n"
     "        color: #6493C4;\n"
     "        padding-left: 15px;\n"
     "        margin-top: 25px;\n"
     "        margin-bottom: 20px;\n"
     "      }\n"
     "      #right1 h1 { font-family: Arial,Helvetica,sans-serif;\n"
     "        font-size: 18px;\n"
     "        font-weight: normal;\n"
     "        color: #333333;\n"
     "        padding-top: 10px;\n"
     "        padding-left: 5px;\n"
     "        margin-bottom: 15px;\n"
     "      }\n"
     "      #right2 h1 { font-family: Arial,Helvetica,sans-serif;\n"
     "        font-size: 18px;\n"
     "        font-weight: normal;\n"
     "        color: #333333;\n"
     "        padding-top: 10px;\n"
     "        padding-left: 5px;\n"
     "        margin-bottom: 15px;\n"
     "      }\n"
     "      #right1 li { border-left: 1px dotted #ccc;\n"
     "        border-right: 1px dotted #ccc;\n"
     "        border-bottom: 1px dotted #ccc;\n"
     "      }\n"
     "    </style>\n");
const std::string WebComposer::FootMsg = std::string
    ("<hr><div id=\"footer\">Copyright 2015-2018 The Euclid SOC Team @ ESAC<br>\n"
     "<center><a href=\"https://www.cosmos.esa.int/web/euclid/euclid-at-esac\" "
     "title=\"The Euclid QLA Processing Framework\" "
     "target=\"_blank\">The Euclid QLA Processing Framework</a><br>\n"
     "<center><a href=\"https://www.cosmos.esa.int/web/euclid/home\" "
     "title=\"Euclid - A Space Mission to map the Dark Universe\" "
     "target=\"_blank\">Euclid - A Space Mission to map the Dark Universe</a></center>\n"
     "</div>\n");

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
HttpServer::HttpServer(const char * name, const char * addr, bool serveFiles)
    : Component(name, "", 0), enableSwarmRequests(serveFiles), server(0), serverIsStarted(false)
{
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
HttpServer::HttpServer(std::string name, std::string addr, bool serveFiles)
    : Component(name, "", 0), enableSwarmRequests(serveFiles), server(0), serverIsStarted(false)
{
}

//----------------------------------------------------------------------
// Method: hello
//----------------------------------------------------------------------
void HttpServer::hello(Request &request, StreamResponse &response)
{
    response << "Hello "
             << htmlEntities(request.get("name", "... what's your name ?")) << std::endl;
}

//----------------------------------------------------------------------
// Method: info
//----------------------------------------------------------------------
void HttpServer::info(Request &request, StreamResponse &response)
{
    WebComposer wc;
    wc.reset();

    genPageLeftColumn(wc);
    genPageRightColumn(wc);
    
    // Center Column
    wc.addSec("QLA General Information");

    wc.addHeading("General", 1);
    wc.addPar("Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.");

    wc.addHeading("Why do we use it?", 2);
    wc.addPar("It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here', making it look like readable English. Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, and a search for 'lorem ipsum' will uncover many web sites still in their infancy. Various versions have evolved over the years, sometimes by accident, sometimes on purpose (injected humour and the like).");

    wc.addHeading("Where does it come from?", 2);
    wc.addPar("Contrary to popular belief, Lorem Ipsum is not simply random text. It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old. Richard McClintock, a Latin professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the undoubtable source. Lorem Ipsum comes from sections 1.10.32 and 1.10.33 of 'de Finibus Bonorum et Malorum' (The Extremes of Good and Evil) by Cicero, written in 45 BC. This book is a treatise on the theory of ethics, very popular during the Renaissance. The first line of Lorem Ipsum, 'Lorem ipsum dolor sit amet..', comes from a line in section 1.10.32.");
    wc.addPar("The standard chunk of Lorem Ipsum used since the 1500s is reproduced below for those interested. Sections 1.10.32 and 1.10.33 from \"de Finibus Bonorum et Malorum\" by Cicero are also reproduced in their exact original form, accompanied by English versions from the 1914 translation by H. Rackham.");
    
    response << wc.getPage() << std::endl;
}

//----------------------------------------------------------------------
// Method: config
//----------------------------------------------------------------------
void HttpServer::config(Request &request, StreamResponse &response)
{
    WebComposer wc;
    wc.reset();

    genPageLeftColumn(wc);
    genPageRightColumn(wc);
    
    // Center Column
    wc.addSec("QLA General Information");

    //============================================================
    wc.addHeading("Configuration", 1);

    //------------------------------------------------------------
    wc.addHeading("General", 2);

    wc.begTable();
    {
        wc.begTRow();
        wc.addHCell("Application Name");
        wc.addTCell(cfg.general.appName());
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("Application Version");
        wc.addTCell(cfg.general.appVersion());
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("Last Access Date");
        wc.addTCell(cfg.general.lastAccess());
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("Work Area Folder");
        wc.addTCell(cfg.general.workArea());
        wc.endTRow();
    }
    wc.endTable();
    
    //------------------------------------------------------------
    wc.addHeading("Network", 2);
    
    wc.begTable();
    {
        wc.begTRow();
        wc.addHCell("Master Node");
        wc.addTCell(cfg.network.masterNode());
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("Starting Port");
        wc.addTCell(std::to_string(cfg.network.startingPort()));
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("Processing nodes");
        {
            wc.addHTML("<td><table>");
            json mp = cfg.network["processingNodes"];
            Json::Value::iterator it = mp.begin();
            while (it != mp.end()) {
                wc.addHTML("<tr><td><b>" + it.key().asString() +
                           "</b></td><td>" + std::to_string((*it).asInt()) + " agents</td>");
                ++it;
            }                          
            wc.addHTML("</table></td>");
        }
        wc.endTRow();

        wc.begTRow();
        wc.addHCell("Swarms configuration");
        for (auto & skv : cfg.network.swarms()) {
            const std::string & swrmName = skv.first;
            CfgGrpSwarm & swrm = skv.second;
            std::string ip = swrm.serviceNodes().at(0);
            {
                wc.addHTML("<td><table>\n");
                wc.addHTML("<tr><td><b>" + swrmName + "</b></td><td><table>\n");
                
                wc.addHTML("<tr><td>Nodes:</td><td>");
                for (auto & s : swrm.serviceNodes()) { wc.addHTML(s + "<br>\n"); }
                wc.addHTML("</td></tr>\n");
                    
                wc.addHTML("<tr><td>Srv.Name:</td><td>" + swrm.name() + "</td></tr>\n");
                wc.addHTML("<tr><td>Scale:</td><td>" + std::to_string(swrm.scale()) + "</td></tr>\n");
                wc.addHTML("<tr><td>Image:</td><td>" + swrm.image() + "</td></tr>\n");
                wc.addHTML("<tr><td>Executable:</td><td>" + swrm.exec() + "</td></tr>\n");
                wc.addHTML("<tr><td>Arguments:</td><td>");
                for (auto & s : swrm.args()) { wc.addHTML(s + " "); }
                wc.addHTML("</td></tr></table></td></tr></table></td>\n");
            }
        }
        wc.endTRow();
    }
    wc.endTable();

    //------------------------------------------------------------
    wc.addHeading("Database", 2);
    
    wc.begTable();
    {
        wc.begTRow();
        wc.addHCell("Host");
        wc.addTCell(cfg.db.host());
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("Port");
        wc.addTCell(cfg.db.port());
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("Data Base name");
        wc.addTCell(cfg.db.name());
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("User");
        wc.addTCell(cfg.db.user());
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("Password");
        wc.addTCell(cfg.db.pwd());
        wc.endTRow();
    }
    wc.endTable();    

    //------------------------------------------------------------
    wc.addHeading("Products", 2);
    
    wc.begTable();
    {
        wc.addHTML("<tr><td id=\"heading\">Product Types:</td><td>");
        for (auto & s : cfg.products.productTypes()) { wc.addHTML(s + "<br>\n"); }
        wc.addHTML("</td></tr>\n");
        
        wc.begTRow();
        wc.addHCell("Parsing RegEx");
        wc.addTCell(cfg.products.parsingRegEx());
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("Parsing Assign");
        wc.addTCell(cfg.products.parsingAssign());
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("Product Id Template");
        wc.addTCell(cfg.products.productIdTpl());
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("Extensions");
        {
            wc.addHTML("<td><table>");
            json mp = cfg.products["extensions"];
            Json::Value::iterator it = mp.begin();
            while (it != mp.end()) {
                wc.addHTML("<tr><td><b>" + it.key().asString() +
                           "</b></td><td>" + (*it).asString() + "</td>");
                ++it;
            }                          
            wc.addHTML("</table></td>");
        }
        wc.endTRow();
    }
    wc.endTable();

    //------------------------------------------------------------
    wc.addHeading("Orchestration", 2);

    wc.addHeading("Rules", 3);

    wc.begTable();
    {
        wc.begTRow();
        wc.addHCell("Rule #");
        wc.addHCell("Tag");
        wc.addHCell("Processor");
        wc.addHCell("Inputs");
        wc.addHCell("Outputs");
        wc.addHCell("Condition");
        wc.endTRow();
        
        int numRules = cfg.orchestration.rules.size();
        for (int i = 0; i < numRules; ++i) {
            wc.begTRow();
            wc.addHCell(std::to_string(i));
            wc.addTCell(cfg.orchestration.rules.tag(i));
            wc.addTCell(cfg.orchestration.rules.processing(i));
            wc.addTCell(cfg.orchestration.rules.inputs(i));
            wc.addTCell(cfg.orchestration.rules.outputs(i));
            wc.addTCell(cfg.orchestration.rules.condition(i));
            wc.endTRow();
        }
    }
    wc.endTable();

    wc.addHeading("Processors", 3);

    wc.begTable();
    {
        json mp = cfg.orchestration["processors"];
        Json::Value::iterator it = mp.begin();
        while (it != mp.end()) {
            wc.begTRow();
            wc.addHCell(it.key().asString());
            wc.addTCell((*it).asString());
            wc.endTRow();
            ++it;
        }                          
    }
    wc.endTable();
    
    //------------------------------------------------------------
    wc.addHeading("User Defined Tools", 2);
    
    wc.begTable();
    {
        CfgGrpUserDefToolsList & uts = cfg.userDefTools;
        for (int i = 0; i < uts.size(); ++i) {
            wc.begTRow();
            wc.addHCell(uts.name(i));
            wc.addHTML("<td><table>\n");
            {
                wc.begTRow();
                wc.addHCell("Description:");
                wc.addTCell(uts.description(i));
                wc.endTRow();
                wc.begTRow();
                wc.addHCell("Executable:");
                wc.addTCell(uts.executable(i));
                wc.endTRow();
                wc.begTRow();
                wc.addHCell("Arguments:");
                wc.addTCell(uts.arguments(i));
                wc.endTRow();
                wc.begTRow();
                wc.addHCell("Associated Prod.Types:");
                wc.addTCell(uts.productTypes(i));
                wc.endTRow();
            }
            wc.addHTML("</table></td>\n");
            wc.endTRow();
        }
    }
    wc.endTable();
    
    //------------------------------------------------------------
    wc.addHeading("Flags", 2);
    
    wc.begTable();
    {
        wc.addHTML("<tr><td id=\"heading\">MsgsToDisk:</td><td>");
        for (auto & s : cfg.flags.msgsToDisk()) { wc.addHTML(s + "<br>\n"); }
        wc.addHTML("</td></tr>\n");

        wc.begTRow();
        wc.addHCell("NotifyMsgArrival");
        wc.addTCell((cfg.flags.notifyMsgArrival() ? "YES" : "NO"));
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("GroupTaskAgentLogs");
        wc.addTCell((cfg.flags.groupTaskAgentLogs() ? "YES" : "NO"));
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("AllowReprocessing");
        wc.addTCell((cfg.flags.allowReprocessing() ? "YES" : "NO"));
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("IntermediateProducts");
        wc.addTCell((cfg.flags.intermediateProducts() ? "YES" : "NO"));
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("SendOutputsToMainArchive");
        wc.addTCell((cfg.flags.sendOutputsToMainArchive() ? "YES" : "NO"));
        wc.endTRow();
        
        wc.begTRow();
        wc.addHCell("Progress Mark String in Logs");
        wc.addTCell("\"" + cfg.flags.progressString() + "\"");
        wc.endTRow();
    }
    wc.endTable();
    
    response << wc.getPage() << std::endl;
}

//----------------------------------------------------------------------
// Method: stat
//----------------------------------------------------------------------
void HttpServer::stat(Request &request, StreamResponse &response)
{
    WebComposer wc;
    wc.reset();

    genPageLeftColumn(wc);
    genPageRightColumn(wc);
    
    // Center Column
    wc.addSec("QLA General Information");

    wc.addHeading("General", 1);
    wc.addPar("Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.");

    wc.addHeading("Why do we use it?", 2);
    wc.addPar("It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here', making it look like readable English. Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, and a search for 'lorem ipsum' will uncover many web sites still in their infancy. Various versions have evolved over the years, sometimes by accident, sometimes on purpose (injected humour and the like).");

    wc.addHeading("Where does it come from?", 2);
    wc.addPar("Contrary to popular belief, Lorem Ipsum is not simply random text. It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old. Richard McClintock, a Latin professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the undoubtable source. Lorem Ipsum comes from sections 1.10.32 and 1.10.33 of 'de Finibus Bonorum et Malorum' (The Extremes of Good and Evil) by Cicero, written in 45 BC. This book is a treatise on the theory of ethics, very popular during the Renaissance. The first line of Lorem Ipsum, 'Lorem ipsum dolor sit amet..', comes from a line in section 1.10.32.");
    wc.addPar("The standard chunk of Lorem Ipsum used since the 1500s is reproduced below for those interested. Sections 1.10.32 and 1.10.33 from \"de Finibus Bonorum et Malorum\" by Cicero are also reproduced in their exact original form, accompanied by English versions from the 1914 translation by H. Rackham.");
    
    response << wc.getPage() << std::endl;
}

//----------------------------------------------------------------------
// Method: genPageLeftColumn()
//----------------------------------------------------------------------
void HttpServer::genPageLeftColumn(WebComposer & wc)
{
    // Left Column
    wc.addHeading("QLA Processing Framework", 1, WebComposer::Left);
    wc.addPar("These are the pages generated by the Euclid QLA Processing "
              "Framework, to provide the users with information on the "
              "configuration and processing status of the running QPF.",

              WebComposer::Left);
    
    wc.startMenu("Simple Menu", WebComposer::Left);
    wc.addMenuItem("Home", "../info", WebComposer::Left);
    wc.addMenuItem("Configuration", "../config", WebComposer::Left);
    wc.addMenuItem("Statistics", "../stat", WebComposer::Left);
    wc.addMenuItem("Form", "../form", WebComposer::Left);
    wc.addMenuItem("Hello", "../hello", WebComposer::Left);
    wc.endMenu(WebComposer::Left);
}

//----------------------------------------------------------------------
// Method: genPageRightColumn()
//----------------------------------------------------------------------
void HttpServer::genPageRightColumn(WebComposer & wc)
{
    // Right Column
    wc.addHeading("Hi!", 1, WebComposer::Right);
    wc.addPar("This is a simple test", WebComposer::Right);

    wc.addHeading("Hi, again!", 1, WebComposer::Right);
    wc.addPar("This is another simple test", WebComposer::Right);
}

//----------------------------------------------------------------------
// Method: form
//----------------------------------------------------------------------
void HttpServer::form(Request &request, StreamResponse &response)
{
    response << "<form method=\"post\">" << std::endl;
    response << "<input type=\"text\" name=\"test\" /><br >" << std::endl;
    response << "<input type=\"submit\" value=\"Envoyer\" />" << std::endl;
    response << "</form>" << std::endl;
}

//----------------------------------------------------------------------
// Method: formPost
//----------------------------------------------------------------------
void HttpServer::formPost(Request &request, StreamResponse &response)
{
    response << "Test=" << htmlEntities(request.get("test", "(unknown)"));
}

//----------------------------------------------------------------------
// Method: session
//----------------------------------------------------------------------
void HttpServer::session(Request &request, StreamResponse &response)
{
    Session & session = getSession(request, response);

    if (session.hasValue("try")) {
        response << "Session value: " << session.get("try");
    } else {
        ostringstream val;
        val << time(NULL);
        session.setValue("try", val.str());
        response << "Session value set to: " << session.get("try");
    }
}

//----------------------------------------------------------------------
// Method: forbid
//----------------------------------------------------------------------
void HttpServer::forbid(Request &request, StreamResponse &response)
{
    response.setCode(HTTP_FORBIDDEN);
    response << "403 forbidden demo";
}

//----------------------------------------------------------------------
// Method: exception
//----------------------------------------------------------------------
void HttpServer::exception(Request &request, StreamResponse &response)
{
    throw std::string("<b>HttpServer Exception!!</b>");
}


//----------------------------------------------------------------------
// Method: uploadForm
//----------------------------------------------------------------------
void HttpServer::uploadForm(Request &request, StreamResponse &response)
{
    response << "<h1>File upload demo (don't forget to create a tmp/ directory)</h1>";
    response << "<form enctype=\"multipart/form-data\" method=\"post\">";
    response << "Choose a file to upload: <input name=\"file\" type=\"file\" /><br />";
    response << "<input type=\"submit\" value=\"Upload File\" />";
    response << "</form>";
}


//----------------------------------------------------------------------
// Method: uploadFormExecute
//----------------------------------------------------------------------
void HttpServer::uploadFormExecute(Request &request, StreamResponse &response)
{
    request.handleUploads();

    // Iterate through all the uploaded files
    std::vector<UploadFile>::iterator it = request.uploadFiles.begin();
    for (; it != request.uploadFiles.end(); it++) {
        UploadFile file = *it;
        file.saveTo("tmp/");
        response << "Uploaded file: " << file.getName() << std::endl;
    }
}

//----------------------------------------------------------------------
// Method: setup
//----------------------------------------------------------------------
void HttpServer::setup()
{
    // Hello demo
    addRoute("GET",  "/hello",     HttpServer, hello);
    addRoute("GET",  "/",          HttpServer, hello);

    // Form demo
    addRoute("GET",  "/form",      HttpServer, form);
    addRoute("POST", "/form",      HttpServer, formPost);

    // Session demo
    addRoute("GET",  "/session",   HttpServer, session);

    // Exception example
    addRoute("GET",  "/exception", HttpServer, exception);

    // 403 demo
    addRoute("GET",  "/403",       HttpServer, forbid);

    // Main pages
    addRoute("GET",  "/info",      HttpServer, info);
    addRoute("GET",  "/config",    HttpServer, config);
    addRoute("GET",  "/stat",      HttpServer, stat);

    // Data server
    addRoute("GET",  "/get_task",      HttpServer, info);
    addRoute("GET",  "/config",    HttpServer, config);
    addRoute("GET",  "/end_task",      HttpServer, stat);

    // File upload demo
    addRoute("GET",  "/upload",    HttpServer, uploadForm);
    addRoute("POST", "/upload",    HttpServer, uploadFormExecute);
}

//----------------------------------------------------------------------
// Method: start
//----------------------------------------------------------------------
void HttpServer::start()
{
    int t;
    t = time(NULL);
    srand(t);

    server = new Server(8080);
    server->registerController(this);
    server->setOption("document_root", Config::PATHWww.c_str());
    server->setOption("enable_directory_listing", "false");
    server->start();

    InfoMsg("Server started, routes:");
    std::map<std::string, RequestHandlerBase *>::iterator it;
    for (it=routes.begin(); it!=routes.end(); it++) {
        InfoMsg("- " + (*it).first);
    }
    
    this->dumpRoutes();

    serverIsStarted = true;
}

//----------------------------------------------------------------------
// Method: fromRunningToOperational
//----------------------------------------------------------------------
void HttpServer::fromRunningToOperational()
{
    while (!serverIsStarted) { timeStep(); }
    
    transitTo(OPERATIONAL);
    InfoMsg("New state: " + getStateName(getState()));
}

//----------------------------------------------------------------------
// Method: fromRunningToOff
//----------------------------------------------------------------------
void HttpServer::fromRunningToOff()
{
    delete server;
}

//----------------------------------------------------------------------
// Method: step
//----------------------------------------------------------------------
void HttpServer::timeStep()
{
    const long int stepSize = 200; // ms

    // Sleep until next second
    /*
      std::time_t tt = system_clock::to_time_t(system_clock::now());
      struct std::tm * ptm = std::localtime(&tt);
      ptm->tm_sec++;
      std::this_thread::sleep_until(system_clock::from_time_t(mktime(ptm)));
    */
    // Sleep for 'stepSize' milliseconds
    //auto start = std::chrono::high_resolution_clock::now();
    long int msnow =
        static_cast<long int>(std::chrono::duration_cast<
                              std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()
                                                         .time_since_epoch()).count());
    long int ms2wait = stepSize - ((msnow + stepSize) % stepSize);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms2wait));
}

//----------------------------------------------------------------------
// Method: run
//----------------------------------------------------------------------
void HttpServer::run()
{
    // State: Initialised
    // Transition to: Running
    fromInitialisedToRunning();

    // State: Initialised
    // Transition to: Running
    fromRunningToOperational();

    do {
        runEachIteration();
        timeStep();
    } while (getState() == OPERATIONAL);

    // State: Initialised
    // Transition to: Running
    fromOperationalToRunning();

    // State: Initialised
    // Transition to: Running
    fromRunningToOff();
}

//}
