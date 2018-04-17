/******************************************************************************
 * File:    launcher.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.Launcher
 *
 * Version:  1.0
 *
 * Date:    2017/12/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Launches a predefined application
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
#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <string>
#include <vector>

//======================================================================
// Class: Launcher
//======================================================================
class Launcher {

public:
    
    //------------------------------------------------------------
    // Constructor
    // Generate a temporary file with unique file name in /tmp
    //------------------------------------------------------------
    Launcher() : isDefined(false) {}

    //----------------------------------------------------------------------
    // Method: create
    // Defines a new app launcher
    //----------------------------------------------------------------------
    virtual void create(std::string prg, std::vector<std::string> arg);
    
    //------------------------------------------------------------
    // Method: exec
    // Executes the defined launcher
    //------------------------------------------------------------
    virtual bool exec();
    
    
protected:
    bool isDefined;
    std::string app;
    std::vector<std::string> args;

};

//======================================================================
// Class: IPythonLauncher
//======================================================================
class IPythonLauncher : public Launcher {

public:
    
    //------------------------------------------------------------
    // Constructor
    // Generate a temporary file with unique file name in /tmp
    //------------------------------------------------------------
    IPythonLauncher(std::string ipyApp, std::string ipyPath);

};

//======================================================================
// Class: JupyterLauncher
//======================================================================
class JupyterLauncher : public Launcher {

public:
    
    //------------------------------------------------------------
    // Constructor
    // Generate a temporary file with unique file name in /tmp
    //------------------------------------------------------------
    JupyterLauncher(std::string jpyApp, std::string jptAddr);

};

#endif // LAUNCHER_H
