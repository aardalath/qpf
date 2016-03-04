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

#include "dbg.h"
#include "str.h"

#include <unistd.h>
#include <ctime>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#include <libgen.h>

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
URLHandler::URLHandler()
{
}

//----------------------------------------------------------------------
// Method: fromExternal2Inbox
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromExternal2Inbox()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    // Get product basename
    std::vector<std::string> tokens = str::split(product.url, '/');
    std::string baseName = tokens.at(tokens.size() - 1);

    // Set new location and url
    std::string newFile(cfgInfo.storage.in.inbox + "/" + baseName);
    std::string newUrl ("file://" + newFile);

    // This method should only be called once the download has been done,
    // hence the only action left is setting the url
    DBG("Changing URL from " << product.url << " to " << newUrl);

    // Change url in processing task
    product.url = newUrl;
    product.urlSpace = InboxSpace;

    return product;
}

//----------------------------------------------------------------------
// Method: fromFolder2Inbox
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromFolder2Inbox()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    assert(str::mid(product.url,0,8) == "file:///");
    assert(product.urlSpace == UserSpace);

    // Get product basename
    std::vector<std::string> tokens = str::split(product.url, '/');
    std::string baseName = tokens.at(tokens.size() - 1);

    // Set new location and url
    std::string file(str::mid(product.url,7,1000));
    std::string newFile(cfgInfo.storage.in.inbox + "/" + baseName);
    std::string newUrl ("file://" + newFile);

    // Set (hard) link (should it be move?)
    (void)relocate(file, newFile);

    // Change url in processing task
    product.url = newUrl;
    product.urlSpace = InboxSpace;

    return product;
}

//----------------------------------------------------------------------
// Method: fromInbox2Local
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromInbox2Local()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    assert(str::mid(product.url,0,8) == "file:///");
    assert(product.urlSpace == InboxSpace);

    // Set new location and url
    std::string file(str::mid(product.url,7,1000));
    std::string newFile(file);
    std::string newUrl(product.url);
    str::replaceAll(newFile, cfgInfo.storage.in.inbox, cfgInfo.storage.local.path + "/in");
    str::replaceAll(newUrl,  cfgInfo.storage.in.inbox, cfgInfo.storage.local.path + "/in");

    // Set (hard) link (should it be move?)
    (void)relocate(file, newFile);

    // Change url in processing task
    product.url = newUrl;
    product.urlSpace = LocalArchSpace;

    return product;
}

//----------------------------------------------------------------------
// Method: fromLocal2Shared
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromLocal2Shared()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    assert(str::mid(product.url,0,8) == "file:///");
    assert(product.urlSpace == LocalArchSpace);

    // Set new location and url
    std::string file(str::mid(product.url,7,1000));
    std::string newFile(file);
    std::string newUrl(product.url);
    std::string archiveSection("/in");
    if (newUrl.find("/out/") != std::string::npos) {
        archiveSection = "/out";
    }
    str::replaceAll(newFile, cfgInfo.storage.local.path + archiveSection, cfgInfo.storage.shared.local_path    + "/in");
    str::replaceAll(newUrl,  cfgInfo.storage.local.path + archiveSection, cfgInfo.storage.shared.external_path + "/in");

    // Set (hard) link
    (void)relocate(file, newFile);

    // Change url in processing task
    product.url = newUrl;
    product.urlSpace = SharedSpace;

    return product;
}

//----------------------------------------------------------------------
// Method: fromShared2Processing
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromShared2Processing()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    assert(str::mid(product.url,0,8) == "file:///");
    assert(product.urlSpace == SharedSpace);

    // Set new location and url
    std::string file(str::mid(product.url,7,1000));
    std::string newFile(file);
    std::string newUrl(product.url);
    str::replaceAll(newFile, cfgInfo.storage.shared.external_path + "/in", taskExchgDir + "in");
    str::replaceAll(newUrl,  cfgInfo.storage.shared.external_path + "/in", taskExchgDir + "in");

    // Get extension
    std::string extension = str::mid(file,file.find_last_of('.') + 1);

    //(void)relocate(file, newFile, 0, (extension == "fits") ? SYMLINK : MOVE);
    // For the time being (while using VMs) substitute the symlink
    // above with a hard link to a single file
    if (extension == "fits") {
        std::string inst = str::mid(file,4,3);
        std::string hrdlnkSource;
        if      (inst == "VIS") { hrdlnkSource = "/qpf/data/....fits"; }
        else if (inst == "NIR") { hrdlnkSource = "/qpf/data/....fits"; }
        else if (inst == "SIR") { hrdlnkSource = "/qpf/data/....fits"; }
        else                    { hrdlnkSource = "/dev/null"; }
        if (link(hrdlnkSource.c_str(), newFile.c_str()) != 0) {
            perror(std::string("link input product: from " + file +
                               " to " + newFile).c_str());
            showBacktrace();
        }
    } else {
        (void)relocate(file, newFile, 0, COPY);
    }

    // Change url in processing task
    product.url = newUrl;
    product.urlSpace = ProcessingSpace;

    return product;
}

//----------------------------------------------------------------------
// Method: fromProcessing2Shared
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromProcessing2Shared()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    assert(str::mid(product.url,0,8) == "file:///");
    assert(product.urlSpace == ProcessingSpace);

    // Set new location and url
    std::string file(str::mid(product.url,7,1000));
    std::string newFile(file);
    std::string newUrl(product.url);

    DBG("Trying to change URL from " << product.url << " to " << newUrl);

    // Get extension
    std::string extension = str::mid(file,file.find_last_of('.') + 1);
    std::string subdir = (extension == "log") ? "log" : "out";
    str::replaceAll(newFile, taskExchgDir + subdir, cfgInfo.storage.shared.external_path + "/out");
    str::replaceAll(newUrl,  taskExchgDir + subdir, cfgInfo.storage.shared.local_path    + "/out");

    //(void)relocate(file, newFile, (extension == "fits") ? SYMLINK : MOVE);
    // For the time being (while using VMs) substitute the symlink
    // above with a hard link to a single file
    if (extension == "fits") {
        std::string inFileName(newFile);
        str::replaceAll(inFileName, "/out/", "/in/");
        str::replaceAll(inFileName, "_LE1_", "_RAW_");
        DBG("FITS file, trying to point to orig.:" << inFileName << " => " << newFile);
        unlink(newFile.c_str());
        symlink(inFileName.c_str(), newFile.c_str());
    } else {
        (void)relocate(file, newFile, 0, COPY);
    }

    // Change url in processing task
    product.url = newUrl;
    product.urlSpace = SharedSpace;

    return product;
}

//----------------------------------------------------------------------
// Method: fromShared2Local
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromShared2Local()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    assert(str::mid(product.url,0,8) == "file:///");
    assert(product.urlSpace == SharedSpace);

    // Set new location and url
    std::string file(str::mid(product.url,7,1000));
    std::string newFile(file);
    std::string newUrl(product.url);

    // If file is FITS, must be symlink to a hard link (RAW), so convert it
    // instead to a hard link to the original file
    // Get extension
    std::string extension = str::mid(file,file.find_last_of('.') + 1);
    if (extension == "fits") {
        char cLink[2048];
        int lenBuf = 0;
        if ((lenBuf = readlink(newFile.c_str(), cLink, 2048)) < 0) {
            perror((std::string("readlink: ") + strerror(errno)).c_str());
            showBacktrace();
        } else {
            cLink[lenBuf] = 0;
            DBG("File " << newFile << " was soft link to " << cLink);
            std::string scLink(newFile);
            str::replaceAll(scLink, "/out/", "/in/");
            str::replaceAll(scLink, "VIS_LE1", "VIS_RAW");
            unlink(newFile.c_str());
            relocate(scLink, newFile, 0, MOVE);
            //link(scLink.c_str(), newFile.c_str());
            DBG(" now " << newFile << " is what " << scLink << " was");
        }
    }

    str::replaceAll(newFile, cfgInfo.storage.shared.local_path + "/out", cfgInfo.storage.local.path + "/out");
    str::replaceAll(newUrl,  cfgInfo.storage.shared.local_path + "/out", cfgInfo.storage.local.path + "/out");

    // Set (hard) link
    (void)relocate(file, newFile, 0, MOVE);

    // Change url in processing task
    product.url = newUrl;
    product.urlSpace = LocalArchSpace;

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
        DBG("Hard link of " << sFrom << " to " << sTo);
        break;
    case SYMLINK:
        retVal = symlink(sFrom.c_str(), sTo.c_str());
        DBG("Soft link of " << sFrom << " to " << sTo);
        break;
    case MOVE:
        retVal = rename(sFrom.c_str(), sTo.c_str());
        DBG("Moving file from " << sFrom << " to " << sTo);
        break;
    case COPY:
        retVal = copyfile(sFrom, sTo);
        DBG("Copying file from " << sFrom << " to " << sTo);
        break;
    default:
        break;
    }

    if (retVal != 0) {
        perror((std::string("ERROR relocating product: ") +
                sFrom + std::string(" => ") + sTo).c_str());
        showBacktrace();
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

//----------------------------------------------------------------------
// Method: setProcElemRunDir
//----------------------------------------------------------------------
void URLHandler::setProcElemRunDir(std::string wkDir, std::string tskDir)
{
    workDir = wkDir;
    intTaskDir = tskDir;

    taskExchgDir = workDir + "/" + intTaskDir + "/";
}

}
