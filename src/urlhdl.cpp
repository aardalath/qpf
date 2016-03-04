/******************************************************************************
 * File:    urlhdl.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.urlhdl
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
 *   Implement URLHandler class
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

#include "urlhdl.h"

#include "tools.h"
using LibComm::replaceAll;

#include <unistd.h>
#include <ctime>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//----------------------------------------------------------------------
// Method: Constructor
//----------------------------------------------------------------------
URLHandler::URLHandler(ConfigurationInfo & aCfgInfo)
{
    cfgInfo = aCfgInfo;
}

//----------------------------------------------------------------------
// Method: fromExternal2Inbox
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromExternal2Inbox()
{
    // Get product basename
    std::vector<std::string> tokens = split(product.url, '/');
    std::string baseName = tokens.at(tokens.size() - 1);

    // Set new location and url
    std::string newFile(cfgInfo.storage.in.inbox + "/" + baseName);
    std::string newUrl ("file://" + newFile);

    // This method should only be called once the download has been done,
    // hence the only action left is setting the url
    std::cerr << "Changing URL from " << product.url << " to " << newUrl << std::endl;

    // Change url in processing task
    product.url = newUrl;

    return product;
}

//----------------------------------------------------------------------
// Method: fromFolder2Inbox
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromFolder2Inbox()
{
    assert(product.url.substr(0,8) == "file:///");

    // Get product basename
    std::vector<std::string> tokens = split(product.url, '/');
    std::string baseName = tokens.at(tokens.size() - 1);

    // Set new location and url
    std::string file(product.url.substr(7,1000));
    std::string newFile(cfgInfo.storage.in.inbox + "/" + baseName);
    std::string newUrl ("file://" + newFile);

    // Set (hard) link (should it be move?)
    (void)relocate(file, newFile);

    // Change url in processing task
    product.url = newUrl;

    return product;
}

//----------------------------------------------------------------------
// Method: fromInbox2Local
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromInbox2Local()
{
    assert(product.url.substr(0,8) == "file:///");

    // Set new location and url
    std::string file(product.url.substr(7,1000));
    std::string newFile(file);
    std::string newUrl(product.url);
    replaceAll(newFile, cfgInfo.storage.in.inbox, cfgInfo.storage.local.path + "/in");
    replaceAll(newUrl,  cfgInfo.storage.in.inbox, cfgInfo.storage.local.path + "/in");

    // Set (hard) link (should it be move?)
    (void)relocate(file, newFile);

    // Change url in processing task
    product.url = newUrl;

    return product;
}

//----------------------------------------------------------------------
// Method: fromLocal2Shared
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromLocal2Shared()
{
    assert(product.url.substr(0,8) == "file:///");

    // Set new location and url
    std::string file(product.url.substr(7,1000));
    std::string newFile(file);
    std::string newUrl(product.url);
    replaceAll(newFile, cfgInfo.storage.local.path + "/in", cfgInfo.storage.shared.local_path    + "/in");
    replaceAll(newUrl,  cfgInfo.storage.local.path + "/in", cfgInfo.storage.shared.external_path + "/in");

    // Set (hard) link
    (void)relocate(file, newFile);

    // Change url in processing task
    product.url = newUrl;

    return product;
}

//----------------------------------------------------------------------
// Method: fromShared2Local
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromShared2Local()
{
    assert(product.url.substr(0,8) == "file:///");

    // Set new location and url
    std::string file(product.url.substr(7,1000));
    std::string newFile(file);
    std::string newUrl(product.url);
    replaceAll(newFile, cfgInfo.storage.shared.external_path + "/out", cfgInfo.storage.local.path + "/out");
    replaceAll(newUrl,  cfgInfo.storage.shared.local_path    + "/out", cfgInfo.storage.local.path + "/out");

    // Set (hard) link
    (void)relocate(file, newFile);

    // Change url in processing task
    product.url = newUrl;

    return product;
}

//----------------------------------------------------------------------
// Method: fromLocal2Archive
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromLocal2Archive()
{
    /* TODO */
    return product;
}

//----------------------------------------------------------------------
// Method: fromArchive2Local
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromArchive2Local()
{
    /* TODO */
    return product;
}

//----------------------------------------------------------------------
// Method: relocate
//----------------------------------------------------------------------
int URLHandler::relocate(std::string & sFrom, std::string & sTo,
                         int msTimeOut, LocalArchiveMethod method)
{
    if (msTimeOut > 0) {
        struct stat buffer;
        struct timespec tsp1, tsp2;
        long elapsed = 0;
        (void)clock_gettime(CLOCK_REALTIME_COARSE, &tsp1);
        while ((stat(sFrom.c_str(), &buffer) != 0) || (elapsed > msTimeOut)) {
            (void)clock_gettime(CLOCK_REALTIME_COARSE, &tsp2);
            elapsed = ((tsp2.tv_sec - tsp1.tv_sec) * 1000 +
                       (tsp2.tv_nsec - tsp1.tv_nsec) / 1000000);
        }
    }

    int retVal = 0;
    switch(method) {
    case LINK:
        retVal = link(sFrom.c_str(), sTo.c_str());
        break;
    case MOVE:
        retVal = rename(sFrom.c_str(), sTo.c_str());
        break;
    case COPY:
        retVal = copyfile(sFrom, sTo);
        break;
    default:
        break;
    }

    if (retVal != 0) {
        perror((std::string("ERROR relocating product: ") +
                sFrom + std::string(" => ") + sTo).c_str());
    } else {
        std::cerr << "Relocatiion from " << sFrom << " to " << sTo << std::endl;
    }
    return retVal;
}


//----------------------------------------------------------------------
// Method: copyfile
//----------------------------------------------------------------------
int URLHandler::copyfile(std::string & sFrom, std::string & sTo)
{
    int source = open(sFrom.c_str(), O_RDONLY, 0);
    int dest = open(sTo.c_str(), O_WRONLY | O_CREAT, 0644);

    // struct required, rationale: function stat() exists also
    struct stat stat_source;
    fstat(source, &stat_source);

    sendfile(dest, source, 0, stat_source.st_size);

    close(source);
    close(dest);

    return 0;
}

}
