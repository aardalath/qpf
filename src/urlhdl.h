/******************************************************************************
 * File:    urlhdl.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.URLHandler
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
 *   Declare URLHandler class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   Component
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

#ifndef URLHANDLED_H
#define URLHANDLED_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: External packages
//  - propref.h
//------------------------------------------------------------
#include "propdef.h"

//------------------------------------------------------------
// Topic: Project headers
//   - cfg.h
//   - msgtypes.h
//------------------------------------------------------------
#include "msgtypes.h"
#include "cfg.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//==========================================================================
// Class: URLHandler
//==========================================================================
class URLHandler {

public:
    URLHandler(ConfigurationInfo & aCfgInfo);

    PropertyRef(URLHandler, ProductMetadata, product, Product);
    PropertyRef(URLHandler, ConfigurationInfo, cfgInfo, CfgInfo);

public:

    //----------------------------------------------------------------------
    // Method: fromExternal2Inbox
    //----------------------------------------------------------------------
    ProductMetadata & fromExternal2Inbox();

    //----------------------------------------------------------------------
    // Method: fromFolder2Inbox
    //----------------------------------------------------------------------
    ProductMetadata & fromFolder2Inbox();

    //----------------------------------------------------------------------
    // Method: fromInbox2Local
    //----------------------------------------------------------------------
    ProductMetadata & fromInbox2Local();

    //----------------------------------------------------------------------
    // Method: fromLocal2Shared
    //----------------------------------------------------------------------
    ProductMetadata & fromLocal2Shared();

     //----------------------------------------------------------------------
    // Method: fromShared2InternalInput
    //----------------------------------------------------------------------
    ProductMetadata & fromShared2InternalInput();

    //----------------------------------------------------------------------
    // Method: fromInternalOutput2Shared
    //----------------------------------------------------------------------
    ProductMetadata & fromInternalOutput2Shared();

     //----------------------------------------------------------------------
    // Method: fromShared2Local
    //----------------------------------------------------------------------
    ProductMetadata & fromShared2Local();

    //----------------------------------------------------------------------
    // Method: fromLocal2Archive
    //----------------------------------------------------------------------
    ProductMetadata & fromLocal2Archive();

    //----------------------------------------------------------------------
    // Method: fromArchive2Local
    //----------------------------------------------------------------------
    ProductMetadata & fromArchive2Local();

    //----------------------------------------------------------------------
    // Method: relocate
    //----------------------------------------------------------------------
    int relocate(std::string & sFrom, std::string & sTo,
                 int msTimeOut = 0, LocalArchiveMethod method = LINK);

    //----------------------------------------------------------------------
    // Method: copyfile
    //----------------------------------------------------------------------
    int copyfile(std::string & sFrom, std::string & sTo);

    inline ProductMetadata & i2l() { return fromInbox2Local(); }
    inline ProductMetadata & l2s() { return fromLocal2Shared(); }
    inline ProductMetadata & s2i() { return fromShared2InternalInput(); }
    inline ProductMetadata & o2e() { return fromInternalOutput2Shared(); }
    inline ProductMetadata & s2l() { return fromShared2Local(); }
    inline ProductMetadata & i2a() { return fromLocal2Archive(); }
    inline ProductMetadata & a2l() { return fromArchive2Local(); }

};

}

#endif  /* URLHANDLED_H */
