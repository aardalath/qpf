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
// Method: fromInbox2Local
//----------------------------------------------------------------------
ProductMetadata & URLHandler::fromInbox2Local()
{
    assert(product.url.substr(0,8) == "file:///");

    // Set new location and url
    std::string newFile(product.url.substr(7,1000));
    std::string newUrl(product.url);
    replaceAll(newFile, cfgInfo.storage.in.exchangeBox, cfgInfo.storage.local.path + "/in");
    replaceAll(newUrl,  cfgInfo.storage.in.exchangeBox, cfgInfo.storage.local.path + "/in");

    // Set (hard) link
    if (link(product.url.c_str(), newFile.c_str()) != 0) {
        perror("hardlink input product");
    }

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
    std::string newFile(product.url.substr(7,1000));
    std::string newUrl(product.url);
    replaceAll(newFile, cfgInfo.storage.local.path + "/in", cfgInfo.storage.shared.local_path    + "/in");
    replaceAll(newUrl,  cfgInfo.storage.local.path + "/in", cfgInfo.storage.shared.external_path + "/in");

    // Set (hard) link
    if (link(product.url.c_str(), newFile.c_str()) != 0) {
        perror("hardlink input product");
    }

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
    std::string newFile(product.url.substr(7,1000));
    std::string newUrl(product.url);
    replaceAll(newFile, cfgInfo.storage.shared.external_path + "/out", cfgInfo.storage.local.path + "/out");
    replaceAll(newUrl,  cfgInfo.storage.shared.local_path    + "/out", cfgInfo.storage.local.path + "/out");

    // Set (hard) link
    if (link(product.url.c_str(), newFile.c_str()) != 0) {
        perror("hardlink input product");
    }

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


}
