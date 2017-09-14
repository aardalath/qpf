/******************************************************************************
 * File:    httpserver.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.httpserver
 *
 * Version:  1.2
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2017 Euclid SOC Team @ ESAC
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
    const std::string HtmlStr(

"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"DTD/xhtml1-transitional.dtd\">"
"<html>"
"  <head>"
"    <title>Your Home Page</title>"
"    <style>"
"      body {"
"        margin:20px 10px 0px 10px;"
"        padding:0px;"
"        font-family: Arial, Helvetica, sans-serif;"
"        font-size: 1em;"
"        color: #666;"
"      }"
"      #center {"
"        width:45%;"
"        float:left;"
"        background:#fff;"
"        padding-bottom:10px;"
"        font-family: Arial, Helvetica, sans-serif;"
"        font-size: 0.65em;"
"        color: #666;"
"        line-height: 1.2em;"
"      }"
"      #right1 {"
"        width:20%;"
"        float:left;"
"        background:#eee;"
"        padding-bottom:10px;"
"        line-height: 1.2em;"
"        font-family: Arial, Helvetica, sans-serif;"
"        font-size: 0.65em;"
"        color: #666;"
"        border-right: 1px solid;"
"        border-color: #ccc;"
"      }"
"      #right2 {"
"        width:15%;"
"        float:left;"
"        background:#fff;"
"        padding-bottom:10px;"
"        border-left: 1px solid #eee;"
"        font-family: Arial, Helvetica, sans-serif;"
"        font-size: 0.65em;"
"        color: #666;"
"      }"
"      .banner {"
"        background:#6493C4;"
"        margin-left: 10px;"
"        margin-right: 10px;"
"        font-family: Arial, Helvetica, sans-serif;"
"        font-size: 1em;"
"        color: #333333;"
"        font-weight: bold;"
"        text-transform: uppercase;"
"      }"
"      #footer {"
"        padding-top: 10px;"
"        margin-left: 10px;"
"        margin-right: 10px;"
"        font-family: Arial, Helvetica, sans-serif;"
"        font-size: 0.8em;"
"        color: #999;"
"        font-weight: normal;"
"        text-transform: none;"
"        text-align: center;"
"      }"
"      #navigation p {"
"        display: none;"
"      }"
"      #navigation {"
"        color: #8A1E30;"
"        text-decoration: none;"
"        font-family: Arial,Helvetica,sans-serif;"
"        font-size: 1em;"
"        font-weight: bold;"
"        padding: 0;"
"        margin-bottom: 1em;"
"        background-color: #eee;"
"      }"
"      #navigation ul {"
"        list-style: none;"
"        margin: 0;"
"        padding: 0;"
"        border-top: 1px dotted #ccc;"
"      }"
"      #navigation ul li {"
"        margin: 0;"
"      }"
"      #navigation ul li a {"
"        display: block;"
"        padding: 2px 2px 2px 1em;"
"        background-color: #eee;"
"        text-decoration: none;"
"        width: 100%;"
"      }"
"      html>body #navigation ul li a {"
"        width:auto;"
"      }"
"      #navigation ul li a:hover {"
"        background-color: #8EABD1;"
"        color: #fff;"
"      }"
"      p,h1,pre {"
"        margin:0px 10px 10px 10px;"
"      }"
"      a {"
"        color: #5578AA;"
"        text-decoration: none;"
"        font-family: Arial,Helvetica,sans-serif;"
"        font-size: 1.2em;"
"        font-weight: bold"
"      }"
"      a:hover {"
"        text-decoration: underline;"
"      }"
"      a:visited:hover {"
"        color: #e7eff2;"
"        text-decoration: underline;"
"      }"
"      h1 {"
"        font-size:12px;"
"        padding-top:10px;"
"      }"
"      #center p {"
"        padding-left: 10px;"
"      }"
"      #right1 a {"
"        font-size: 1.1em;"
"      }"
"      #right2 a {"
"        font-size: 1.1em;"
"      }"
"      #center h1 {"
"        font-family: Arial,Helvetica,sans-serif;"
"        font-size: 30px;"
"        font-weight: normal;"
"        color: #6493C4;"
"        padding-left: 10px;"
"        margin-top: 10px;"
"        margin-bottom: 20px;"
"      }"
"      #center h2 { font-family: Arial,Helvetica,sans-serif;"
"        font-size: 18px;"
"        font-weight: normal;"
"        color: #6493C4;"
"        padding-left: 15px;"
"        margin-top: 25px;"
"        margin-bottom: 20px;"
"      }"
"      #right1 h1 { font-family: Arial,Helvetica,sans-serif;"
"        font-size: 18px;"
"        font-weight: normal;"
"        color: #333333;"
"        padding-top: 10px;"
"        padding-left: 5px;"
"        margin-bottom: 15px;"
"      }"
"      #right2 h1 { font-family: Arial,Helvetica,sans-serif;"
"        font-size: 18px;"
"        font-weight: normal;"
"        color: #333333;"
"        padding-top: 10px;"
"        padding-left: 5px;"
"        margin-bottom: 15px;"
"      }"
"      #right1 li { border-left: 1px dotted #ccc;"
"        border-right: 1px dotted #ccc;"
"        border-bottom: 1px dotted #ccc;"
"      }"
"    </style>"
"  </head>"
"  <body>"
"    <div id=\"right1\">"
"      <h1>Welcome!</h1>"
"      <p>\"There is no one who loves pain itself, who seeks after it and wants to have it, simply because it is pain...\""
"      </p>"
"      <p>"
"        <strong>Menu"
"        </strong><br/></p>"
"      <div id=\"navigation\">"
"        <ul>"
"          <li><a href=\"\">Home</a></li>"
"          <li><a href=\"\">Abot us</a></li>"
"          <li><a href=\"\">News</a></li>"
"          <li><a href=\"\">Contact</a></li>"
"          <li><a href=\"\">Links</a></li>"
"        </ul>"
"      </div>"
"    </div>"
"    <div id=\"center\">"
"      <div class=\"banner\">info</div>"
"      <h1>Your Home Page</h1>"
"      <p><strong>What is Lorem Ipsum?</strong></p>"
"      <p>Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.</p>"
"      <h2>Why do we use it?</h2>"
"      <p>It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here', making it look like readable English. Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, and a search for 'lorem ipsum' will uncover many web sites still in their infancy. Various versions have evolved over the years, sometimes by accident, sometimes on purpose (injected humour and the like).</p>"
"      <h2>Where does it come from?</h2>"
"      <p>Contrary to popular belief, Lorem Ipsum is not simply random text. It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old. Richard McClintock, a Latin professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the undoubtable source. Lorem Ipsum comes from sections 1.10.32 and 1.10.33 of \"de Finibus Bonorum et Malorum\" (The Extremes of Good and Evil) by Cicero, written in 45 BC. This book is a treatise on the theory of ethics, very popular during the Renaissance. The first line of Lorem Ipsum, \"Lorem ipsum dolor sit amet..\", comes from a line in section 1.10.32.</p>"
"      <p>The standard chunk of Lorem Ipsum used since the 1500s is reproduced below for those interested. Sections 1.10.32 and 1.10.33 from \"de Finibus Bonorum et Malorum\" by Cicero are also reproduced in their exact original form, accompanied by English versions from the 1914 translation by H. Rackham.</p>"
"      <p><br></br></p>"
"      <div class=\"banner\">donate</div>"
"      <p></p>"
"      <p>If you use this site regularly and would like to help keep the site on the Internet, please consider donating a small sum to help pay for the hosting and bandwidth bill. There is no minimum donation, any sum is appreciated - click here to donate using PayPal. Thank you for your support.</p>"
"      <div id=\"footer\">Copyright 2006 Your Home Page"
"        <!-- Designed by DreamTemplate. Please leave link unmodified. -->"
"        <br><center><a href=\"http://www.dreamtemplate.com\" title=\"Website Templates\" target=\"_blank\">Website templates</a></center>"
"          "
"        </div>"
"      </div>"
"      <div id=\"right2\">"
"        <h1>About Us</h1>"
"        <p>\"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\"</p>"
"        <h1>Photos</h1>"
"        <p><a href=\"\"><img src=\"pic1.jpg\" height=\"104\" width=\"125\" class=\"thumbnail\" alt=\"Included colors\" align=\"middle\"/></a></p>"
"        <p><a href=\"\"><img src=\"pic2.jpg\" height=\"104\" width=\"125\" class=\"thumbnail\" alt=\"Included colors\" align=\"middle\"/></a></p>"
"        <p><a href=\"\"><img src=\"pic3.jpg\" height=\"104\" width=\"125\" class=\"thumbnail\" alt=\"Included colors\" align=\"middle\"/></a></p>"
"        <p align=\"right\"><a href=\"\">More...</a></p>"
"      </div>"
"    </body>"
"  </html>"
                              );
    response << HtmlStr << std::endl;
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

    // 403 demo
    addRoute("GET",  "/info",      HttpServer, info);

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
    server->setOption("document_root", ".");
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
