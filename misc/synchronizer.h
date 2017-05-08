/******************************************************************************
 * File:    synchronizer.h
 *          This file is part of Package GBB.
 *
 * Module:  synchronizer
 *
 * Version:  1.2
 *
 * Date:    2009/06/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015,2016,2017 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Define handy synchronizer to use in the application.
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
 * CVS Information:
 *   - $FileName: RDS_RestartTypes.h,v$
 *   - $Revision: 1.2.4.2 $
 *   - $Date: 2008/12/23 11:04:50 $
 *
 * History:
 *   See <Changelog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

/************************************************************
 * Group: External Dependencies
 ************************************************************/

/*----------------------------------------------------------
 * Topic: System Include Files
 *   - stdincl.h
 *   - macros.h
 */
#include "stdincl.h"
#include "macros.h"

/*----------------------------------------------------------
 * Topic: External Package Include Files
 *   none
 */

/*----------------------------------------------------------
 * Topic: Local Include Files
 *   none
 */

/************************************************************
 * Group: Public Declarations
 ************************************************************/

/*----------------------------------------------------------
 * Topic: Constants and macros
 *   EXTERN(t,v,i) - Declares *extern* conditionally
 */

#ifdef SYNCHRONIZER_IMPLEMENTATION
#  define EXTERN(t,v,i)         t v = i
#else
#  define EXTERN(t,v,i)  extern t v
#endif /* SYNCHRONIZER_IMPLEMENTATION */

/*----------------------------------------------------------
 * Topic: Public Type Definitions
 */

/*----------------------------------------------------------
 * Topic: Exported Data Entities
 *   syncCond  - (pthread_cond_t) := PTHREAD_COND_INITIALIZER
 *   syncMutex - (pthread_mutex_t) := PTHREAD_MUTEX_INITIALIZER
 */

EXTERN(pthread_cond_t,syncCond,PTHREAD_COND_INITIALIZER);
EXTERN(pthread_mutex_t,syncMutex,PTHREAD_MUTEX_INITIALIZER);

#undef EXTERN

/************************************************************
 * Group: Public Interface
 ************************************************************/

/****************************************************************************
 * Function: init
 *   Initializes timer
 *
 * Returns:
 *   <nothing>
 ***************************************************************************/
void
synchro_init();

/****************************************************************************
 * Function: launch
 *   Launchs synchronizer
 *
 * Returns:
 *   <nothing>
 ***************************************************************************/
void
synchro_launch();

/****************************************************************************
 * Function: sync
 *   Synchronize thread
 *
 * Returns:
 *   <nothing>
 ***************************************************************************/
void
synchro_sync(pthread_t thrId);

/****************************************************************************
 * Function: subscribe
 *   Waits until a given epoch
 *
 * Parameters:
 *   elmt -  (I) The element being subscribed
 *
 * Returns:
 *   <nothing>
 ***************************************************************************/
void
synchro_register(pthread_t thrId);

#endif /* SYNCHRONIZER_H */
