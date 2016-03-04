/******************************************************************************
 * File:    cfginfo.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.ConfigurationInfo
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
 *   Implement ConfigurationInfo class
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

#include "cfginfo.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

ConfigurationInfo::ConfigurationInfo()
{
}

void ConfigurationInfo::clear()
{
    orcParams.processors.clear();
    orcParams.productTypes.clear();
    orcParams.rules.clear();
    peerNames.clear();
    peersCfg.clear();
    peersCfgByName.clear();
    connections.clear();
    peerNodes.clear();
    peerAgents.clear();
    machines.clear();
    machineNodes.clear();
}

ConfigurationInfo & ConfigurationInfo::data() {
    static ConfigurationInfo cfgInfoInstance {};
    return cfgInfoInstance;
}

}
