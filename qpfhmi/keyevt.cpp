/******************************************************************************
 * File:    keyevt.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.keyevt
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
 *   Check CAPS LOCK state
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

#include "keyevt.h"

#ifdef Q_OS_WIN32
# include <windows.h>
#else
#  include <X11/XKBlib.h>
# undef KeyPress
# undef KeyRelease
# undef FocusIn
# undef FocusOut
// #undef those Xlib #defines that conflict with QEvent::Type enum
#endif

bool checkCapsLock()
{
    // platform dependent method of determining if CAPS LOCK is on
#ifdef Q_OS_WIN32 // MS Windows version
    return GetKeyState(VK_CAPITAL) == 1;
#else // X11 version (Linux/Unix/Mac OS X/etc...)
    Display * d = XOpenDisplay((char*)0);
    bool caps_state = false;
    if (d) {
        unsigned n;
        XkbGetIndicatorState(d, XkbUseCoreKbd, &n);
        caps_state = (n & 0x01) == 1;
    }
    return caps_state;
#endif
}

