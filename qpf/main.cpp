/******************************************************************************
 * File:    main.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.QPF.main
 *
 * Version:  1.2
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015,2016,2017 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Create all components with the Deployer, and launch the Core system
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

#include "version.h"

#ifndef BUILD_ID
#define BUILD_ID ""
#endif

#include "deployer.h"

//----------------------------------------------------------------------
// Function: sayHello
// Shows a minimal title and build id for the application
//----------------------------------------------------------------------
void sayHello()
{
    std::string buildId(BUILD_ID);
    if (buildId.empty()) {
        char buf[20];
        sprintf(buf, "%ld", (long)(time(0)));
        buildId = std::string(buf);
    }
    std::string hline("----------------------------------------"
                      "--------------------------------------");
    std::cout << hline << std::endl
            << " " << APP_NAME << " - " << APP_LONG_NAME << std::endl
            << " " << APP_DATE << " - "
            << APP_RELEASE << " Build " << buildId << std::endl
            << hline << std::endl << std::endl;
}

//----------------------------------------------------------------------
// Function: main
// Creates the application main class, and invokes its run method
//----------------------------------------------------------------------
int main(int argc, char *argv[])
{
    sayHello();
    QPF::Deployer deployer(argc, argv);
    return deployer.run();
}
