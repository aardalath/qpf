/******************************************************************************
 * File:    urlhdl.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.urlhdl
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

#include <unistd.h>
#include <cassert>
#include <ctime>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>

#include <libgen.h>

#include "str.h"
#include "config.h"
#include "voshdl.h"

#include "dbg.h"

#include "filetools.h"
using namespace FileTools;

#define showBacktrace()

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
//namespace QPF {

using Configuration::cfg;

//----------------------------------------------------------------------
// Method: Constructor
//----------------------------------------------------------------------
URLHandler::URLHandler(bool remote) : isRemote(remote)
{
}

//----------------------------------------------------------------------
// Method: fromExternal2Inbox
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromExternal2Inbox()
{
    // NO DOWNLOAD IS STILL IMPLEMENTED
    // TODO: Implement download of external products into inbox

    // Get product basename
    std::vector<std::string> tokens;
    productUrl = product.url();
    str::split(productUrl, '/', tokens);
    std::string baseName = tokens.back();

    // Set new location and url
    std::string newFile(cfg.storage.inbox + "/" + baseName);
    std::string newUrl ("file://" + newFile);

    // This method should only be called once the download has been done,
    // hence the only action left is setting the url
    TRC("Changing URL from " << productUrl << " to " << newUrl);

    // Change url in processing task
    product["url"]      = newUrl;
    product["urlSpace"] = InboxSpace;

    return product;
}

//----------------------------------------------------------------------
// Method: fromOutbox2External
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromOutbox2External()
{
    // NO UPLOAD IS STILL IMPLEMENTED
    // TODO: Implement upload of external products from outbox

    return product;
}

//----------------------------------------------------------------------
// Method: fromFolder2Inbox
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromFolder2Inbox()
{
    productUrl      = product.url();
    productUrlSpace = product.urlSpace();

    assert(str::mid(productUrl,0,8) == "file:///");
    assert((productUrlSpace == UserSpace) ||
           (productUrlSpace == ReprocessingSpace));

    // Get product basename
    std::vector<std::string> tokens;
    str::split(productUrl, '/', tokens);
    std::string baseName = tokens.back();

    // Set new location and url
    std::string file(str::mid(productUrl,7));
    std::string newFile(cfg.storage.inbox + "/" + baseName);
    std::string newUrl("file://" + newFile);

    // Set (hard) link (should it be move?)
    (void)relocate(file, newFile, LINK);

    // Change url in processing task
    product["url"] = newUrl;
    if (productUrlSpace != ReprocessingSpace) {
        product["urlSpace"] = InboxSpace;
    }

    return product;
}

//----------------------------------------------------------------------
// Method: fromInbox2Local
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromInbox2LocalArch(bool tx)
{
    productUrl      = product.url();
    productUrlSpace = product.urlSpace();

    assert(str::mid(productUrl,0,8) == "file:///");
    assert((productUrlSpace == InboxSpace) ||
           (productUrlSpace == ReprocessingSpace));

    // Set new location and url
    std::string file(str::mid(productUrl,7,1000));
    std::string newFile(file);
    std::string newUrl(productUrl);

    str::replaceAll(newFile,
                    cfg.storage.inbox,
                    cfg.storage.archive);
    str::replaceAll(newUrl,
                    cfg.storage.inbox,
                    cfg.storage.archive);

    if (product.hadNoVersion()) {
        std::string a("Z." + product.extension());
        std::string b("Z_" + product.productVersion() + "." + product.extension());

        str::replaceAll(productUrl,  a, b);
        str::replaceAll(newFile,     a, b);
        str::replaceAll(newUrl,      a, b);

        product["hadNoVersion"] = false;
    }

    if (tx) {
        if (productUrlSpace != ReprocessingSpace) {
            // Set (hard) link (should it be move?)
            (void)relocate(file, newFile, MOVE);
        } else {
            // From now on the addressed file will be the existing one in the
            // local archive, so we remove the existing (hard) link in the inbox
            (void)unlink(file.c_str());
        }
    }
    // Change url in processing task
    product["url"]      = newUrl;
    product["urlSpace"] = LocalArchSpace;

    return product;
}

//----------------------------------------------------------------------
// Method: fromLocal2Gateway
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromLocalArch2Gateway()
{
    productUrl      = product.url();
    productUrlSpace = product.urlSpace();

    assert(str::mid(productUrl,0,8) == "file:///");
    assert((productUrlSpace == LocalArchSpace)  ||
           (productUrlSpace == ReprocessingSpace));

    // Set new location and url
    std::string file(str::mid(productUrl,7,1000));
    std::string newFile(file);
    std::string newUrl(productUrl);

    str::replaceAll(newFile,
                    cfg.storage.archive,
                    cfg.storage.gateway + "/in");
    str::replaceAll(newUrl,
                    cfg.storage.archive,
                    cfg.storage.gateway + "/in");

    // Set (hard) link
    (void)relocate(file, newFile, LINK, -1);

    // Change url in processing task
    product["url"]      = newUrl;
    product["urlSpace"] = GatewaySpace;

    return product;
}

//----------------------------------------------------------------------
// Method: fromGateway2Processing
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromGateway2Processing()
{
    productUrl      = product.url();
    productUrlSpace = product.urlSpace();

    assert(str::mid(productUrl,0,8) == "file:///");
    assert(productUrlSpace == GatewaySpace);

    // Set new location and url
    std::string file(str::mid(productUrl,7,1000));
    std::string newFile(file);
    std::string newUrl(productUrl);

    std::string section("/in");

    str::replaceAll(newFile,
                    cfg.storage.gateway + section,
                    taskExchgDir + section);
    str::replaceAll(newUrl,
                    cfg.storage.gateway + section,
                    taskExchgDir + section);

    if (isRemote) {
        (void)relocate(file, newFile, COPY_TO_REMOTE);
        unlink(file.c_str());
    } else {
        (void)relocate(file, newFile, MOVE);
    }

    // Change url in processing task
    product["url"]      = newUrl;
    product["urlSpace"] = ProcessingSpace;

    return product;
}

//----------------------------------------------------------------------
// Method: fromProcessing2Gateway
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromProcessing2Gateway()
{
    productUrl      = product.url();
    productUrlSpace = product.urlSpace();

    assert(str::mid(productUrl,0,8) == "file:///");
    assert(productUrlSpace == ProcessingSpace);

    // Set new location and url
    std::string file(str::mid(productUrl,7,1000));
    std::string newFile(file);
    std::string newUrl(productUrl);

    // Get extension
    std::string extension = str::mid(file,file.find_last_of('.') + 1);
    std::string subdir = (extension == "log") ? "/log" : "/out";
    std::string section("/out");

    str::replaceAll(newFile,
                    taskExchgDir + subdir,
                    cfg.storage.gateway + section);
    str::replaceAll(newUrl,
                    taskExchgDir + subdir,
                    cfg.storage.gateway + section);

    if (isRemote) {
        (void)relocate(file, newFile, COPY_TO_MASTER);
        unlink(file.c_str());
    } else {
        (void)relocate(file, newFile, LINK);
    }

    // Change url in processing task
    product["url"]      = newUrl;
    product["urlSpace"] = GatewaySpace;

    return product;
}

//----------------------------------------------------------------------
// Method: fromGateway2LocalArch
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromGateway2LocalArch()
{
    productUrl      = product.url();
    productUrlSpace = product.urlSpace();

    assert(str::mid(productUrl,0,8) == "file:///");
    assert(productUrlSpace == GatewaySpace);

    // Set new location and url
    std::string file(str::mid(productUrl,7,1000));
    std::string newFile(file);
    std::string newUrl(productUrl);

    str::replaceAll(newFile,
                    cfg.storage.gateway + "/out",
                    cfg.storage.inbox);
    str::replaceAll(newUrl,
                    cfg.storage.gateway + "/out",
                    cfg.storage.archive);

    // Set (hard) link
    (void)relocate(file, newFile, MOVE);

    // Change url in processing task
    product["url"]      = newUrl;
    product["urlSpace"] = InboxSpace; //LocalArchSpace;

    return product;
}

//----------------------------------------------------------------------
// Method: fromGateway2FinalDestination
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromGateway2FinalDestination()
{
    productUrl      = product.url();
    productUrlSpace = product.urlSpace();

    assert(str::mid(productUrl,0,8) == "file:///");
    assert(productUrlSpace == GatewaySpace);

    // Set new location and url
    std::string file(str::mid(productUrl,7,1000));
    std::string newFile(file);
    std::string newUrl(productUrl);

    UserAreaId tgtType = UserAreaId(product.procTargetType());
    std::string tgtFolder(cfg.storage.archive);
    
    tgtFolder = product.procTarget();    

    if (tgtType == UA_VOSPACE) {
        if (sendToVOSpace(cfg.connectivity.vospace.user(),
                          cfg.connectivity.vospace.pwd(),
                          cfg.connectivity.vospace.url(),
                          ((tgtFolder.empty()) ?
                           cfg.connectivity.vospace.folder() :
                           tgtFolder),
                          file)) {
            (void)unlink(file.c_str());
            product["urlSpace"] = ReprocessingVOSpace;
        }
        return product;
    }

    // Ensure local folder exists
    if ((mkdir(tgtFolder.c_str(), Config::PATHMode) != 0) &&
        (errno != EEXIST)) {
        TRC("mkdir " + tgtFolder + ": " + std::strerror(errno));
        return product;
    }
    
    TRC("Must move " + newFile + " from " + cfg.storage.gateway +
        "/out to " + tgtFolder);
    
    str::replaceAll(newFile,
                    cfg.storage.gateway + "/out",
                    tgtFolder);
    str::replaceAll(newUrl,
                    cfg.storage.gateway + "/out",
                    tgtFolder);

    (void)relocate(file, newFile, MOVE);

    // Change url in processing task
    product["url"]      = newUrl;
    product["urlSpace"] = (tgtType == UA_NOMINAL ? InboxSpace :
                           (tgtType == UA_LOCAL ? ReprocessingLocalFolder :
                            ReprocessingVOSpace));
    
    return product;
}

//----------------------------------------------------------------------
// Method: fromLocalArch2ExportLocation
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromLocalArch2ExportLocation()
{
    productUrl      = product.url();
    productUrlSpace = product.urlSpace();

    assert(str::mid(productUrl,0,8) == "file:///");
    assert(productUrlSpace == LocalArchSpace);

    // Set new location and url
    std::string file(str::mid(productUrl,7,1000));
    std::string newFile(file);
    std::string newUrl(productUrl);

    UserAreaId tgtType = UserAreaId(product.procTargetType());
    std::string tgtFolder; 
    
    tgtFolder = product.procTarget();

    if (tgtType == UA_VOSPACE) {
        if (sendToVOSpace(cfg.connectivity.vospace.user(),
                          cfg.connectivity.vospace.pwd(),
                          cfg.connectivity.vospace.url(),
                          ((tgtFolder.empty()) ?
                           cfg.connectivity.vospace.folder() :
                           tgtFolder),
                          file)) {
            product["urlSpace"] = ReprocessingVOSpace;
        }
        return product;
    }

    // Ensure local folder exists
    if ((mkdir(tgtFolder.c_str(), Config::PATHMode) != 0) &&
        (errno != EEXIST)) {
        return product;
    }
    
    str::replaceAll(newFile,
                    cfg.storage.archive,
                    tgtFolder);
    str::replaceAll(newUrl,
                    cfg.storage.archive,
                    tgtFolder);

    (void)relocate(file, newFile, COPY);

    // Change url in processing task
    product["url"]      = newUrl;
    product["urlSpace"] = (tgtType == UA_NOMINAL ? InboxSpace :
                           (tgtType == UA_LOCAL ? ReprocessingLocalFolder :
                            ReprocessingVOSpace));
    
    return product;
}

//----------------------------------------------------------------------
// Method: sendToVOSpace
//----------------------------------------------------------------------
bool URLHandler::sendToVOSpace(std::string user, std::string pwd,
                               std::string vosURL, std::string folder,
                               std::string oFile)
{
    VOSpaceHandler vos(vosURL);
    vos.setAuth(user, pwd);
    if (!vos.uploadFile(folder, oFile)) {
        TRC("ERROR! Cannot upload " << oFile);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------
// Method: relocate
//----------------------------------------------------------------------
int URLHandler::relocate(std::string & sFrom, std::string & sTo,
                         LocalArchiveMethod method, int msTimeOut)
{
    // Wait for the file to appear
    if (msTimeOut != 0) {
        // If timeout is > 0, wait unti timeout is completed
        if (msTimeOut < 0) {
            // If timeout < 0, wait forever until stat is successful
            // (in fact, set a very large value for timeout, say 1 minute)
            msTimeOut = 60000;
        }
        struct stat buffer;
        struct timespec tsp1, tsp2;
        long elapsed = 0;
        (void)clock_gettime(CLOCK_REALTIME_COARSE, &tsp1);
        while ((stat(sFrom.c_str(), &buffer) != 0) || (elapsed > msTimeOut)) {
            (void)clock_gettime(CLOCK_REALTIME_COARSE, &tsp2);
            elapsed = ((tsp2.tv_sec - tsp1.tv_sec) * 1000 +
                       (tsp2.tv_nsec - tsp1.tv_nsec) / 1000000);
        }
        if (elapsed > msTimeOut) {
            TRC("ERROR: Timeout of " + std::to_string(msTimeOut) +
                          "ms before successful stat:\t" +
                          sFrom + std::string(" => ") + sTo);
            return -1;
        }
    }

    int retVal = 0;
    switch(method) {
    case LINK:
        retVal = link(sFrom.c_str(), sTo.c_str());
        TRC("LINK: Hard link of " << sFrom << " to " << sTo);
        break;
    case SYMLINK:
        retVal = symlink(sFrom.c_str(), sTo.c_str());
        TRC("SYMLINK: Soft link of " << sFrom << " to " << sTo);
        break;
    case MOVE:
        retVal = rename(sFrom.c_str(), sTo.c_str());
        TRC("MOVE: Moving file from " << sFrom << " to " << sTo);
        if ((retVal != 0) && (errno = EXDEV)) {
            // Error due to move between different logical devices
            // Try copy & remove
            if ((retVal = copyfile(sFrom, sTo)) == 0) {
                (void)unlink(sFrom.c_str());
            }
        }
        break;
    case COPY:
        retVal = copyfile(sFrom, sTo);
        TRC("COPY: Copying file from " << sFrom << " to " << sTo);
        break;
    case COPY_TO_REMOTE:
    case COPY_TO_MASTER:
        retVal = rcopyfile(sFrom, sTo, master_address, method == COPY_TO_REMOTE);
        TRC(((method == COPY_TO_REMOTE) ? "COPY_TO_REMOTE: " : "COPY_TO_MASTER: ")
            << "Transferring file from " << sFrom << " to " << sTo);
        break;
    default:
        break;
    }

    if (retVal != 0) {
        perror(("ERROR (" + std::to_string(retVal) + "/" + std::to_string(errno) +
                ") relocating product:\n\t" +
                sFrom + std::string(" => ") + sTo).c_str());
        //showBacktrace();
    }
    return retVal;
}

//----------------------------------------------------------------------
// Method: setRemoteCopyParams
//----------------------------------------------------------------------
void URLHandler::setRemoteCopyParams(std::string maddr, std::string raddr)
{
    master_address = maddr;
    remote_address = raddr;
    isRemote = true;
    TRC("Master addr: " << maddr << "  Remote addr: " << raddr);
}

//----------------------------------------------------------------------
// Method: setProcElemRunDir
//----------------------------------------------------------------------
void URLHandler::setProcElemRunDir(std::string wkDir, std::string tskDir)
{
    workDir = wkDir;
    intTaskDir = tskDir;

    taskExchgDir = workDir + "/" + intTaskDir;
    TRC("Workdir: " << workDir << "   IntTaskDir: " << intTaskDir
        << "  => TaskExchgDir: " << taskExchgDir);
}

//}
