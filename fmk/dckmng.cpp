/******************************************************************************
 * File:    dckmng.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.dckmng
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
 *   Implement ServiceMng class
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

#include "dckmng.h"

#include "process.h"
#include "str.h"
#include "dbg.h"

#include <iostream>
#include <fstream>
#include <cassert>

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
//namespace QPF {

//----------------------------------------------------------------------
// Method: getDockerInfo
// Retrieves information about running Docker instance
//----------------------------------------------------------------------
bool DockerMng::getDockerInfo(std::stringstream & info, std::string filt = std::string())
{
    procxx::process dckInfo("docker", "info");
    dckInfo.exec();

    info.str("");
    std::string line;
    while (std::getline(dckInfo.output(), line)) {
        if (filt.empty() || (line.find(filt) != std::string::npos)) {
            info << line << std::endl;
        }
        if (!dckInfo.running() ||
            !procxx::running(dckInfo.id()) ||
            !running(dckInfo)) {
            break;
        }
    }

    dckInfo.wait();
    return (dckInfo.code() == 0);
}

//----------------------------------------------------------------------
// Method: runCmd
// Run docker command with arguments
//----------------------------------------------------------------------
bool DockerMng::runCmd(std::string cmd, std::vector<std::string> args,
                       std::string & containerId)
{
    procxx::process dckCmd("docker", cmd);
    dckCmd.add_argument(containerId);
    if (args.size() > 0) {
        for (auto & a : args) { dckCmd.add_argument(a); }
    }
    
    dckCmd.exec();

    dckCmd.wait();
    return (dckCmd.code() == 0);
}
//}
