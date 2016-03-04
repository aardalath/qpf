/******************************************************************************
 * File:    types.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.types
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
 *   Provides object implementation for some declarations
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

#include "types.h"

namespace QPF {

#define T(a) #a
const char * MdataFieldNames[] = { TLIST_PROD_BASIC_MDATA_FIELDS };
#undef T

const int NumOfMdataFieldNames = sizeof(MdataFieldNames) / sizeof(char*);

}
