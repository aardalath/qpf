/******************************************************************************
 * File:    synchronizer.c
 *          This file is part of Package GBB.
 *
 * Module:  synchronizer
 *
 * Version:  1.1
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
 * About: License
 *
 *   This piece of code is free software with the restrictions of this
 *   license.
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a
 *   copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to permit
 *   persons to whom the Software is furnished to do so, subject to the
 *   following conditions:
 * 
 *   The above copyright notice and this permission notice SHALL BE INCLUDED
 *   in all copies or substantial portions of the Software.
 * 
 *   The software is provided "AS IS", without warranty of any kind, express
 *   or implied, including but not limited to the warranties of
 *   merchantability, fitness for a particular purpose and noninfringement.
 *   In no event shall J C Gonzalez be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising
 *   from, out of or in connection with the software or the use or other
 *   dealings in the software.
 * 
 ******************************************************************************/

/******************************************************************************
 * Group: External Dependencies
 ******************************************************************************/

/*-----------------------------------------------------------------------------
 * Topic: System Include Files
 *   - time.h
 *   - sys/time.h
 *   - unistd.h
 *   - pthread.h
 *   - assert.h
 */
#include <time.h>
#include <sys/time.h>
#include <unistd.h> 
#include <pthread.h>
#include <assert.h>  

/*-----------------------------------------------------------------------------
 * Topic: External Package Include Files
 *   - macros.h
 */
#include "macros.h"

/*-----------------------------------------------------------------------------
 * Topic: Local Include Files
 *   - synchronizer.h
 */
#define SYNCHRONIZER_IMPLEMENTATION
#include "synchronizer.h"

/******************************************************************************
 * Group: Private Declarations
 ******************************************************************************/

/*-----------------------------------------------------------------------------
 * Topic: Constants and macros
 *   MAX_NUM_SYNC_THREADS - Max. number of threads that can be synchronized
 */
#define MAX_NUM_SYNC_THREADS 10

/*----------------------------------------------------------
 * Type: RegThrMap_t
 *   Contains information about threads
 *
 * Fields: 
 *   thrId     - Thread Identifier
 *   siIdxSync - Synchronization Index
 */
typedef struct {
  pthread_t  thrId;
  int        siIdxSync;
} RegThrMap_t;

/*-----------------------------------------------------------------------------
 * Topic: Private Data Entities
 */

static RegThrMap_t ptListThr[MAX_NUM_SYNC_THREADS];
static RegThrMap_t ptListSync[MAX_NUM_SYNC_THREADS];
static int         siNumRegThr = 0;
static int         siNumSyncThr = 0;

static bool        bInitialized = false;  

static pthread_t       thrSynchronizer;
static pthread_mutex_t mutexSynchronizer;

/* Number of usec to sleep */
int usec;

/* Target time */
struct timeval untilTm;

/* Last recorded time */
struct timeval lastTm;

/* Last sleep time */
struct timeval sleepTm;

/* Current time */
struct timeval nowTm;

/*-----------------------------------------------------------------------------
 * Topic: Declaration of Private Functions
 */

/******************************************************************************
 * Group: Implementation of Public Functions
 ******************************************************************************/

/****************************************************************************
 * Function: init
 *   Initializes timer
 *
 * Returns:
 *   <nothing>
 ***************************************************************************/
void synchro_init()
{
  INIT_MUTEX(mutexSynchronizer);
  MUTEXED(mutexSynchronizer, {
      if (bInitialized == false) {
        pthread_mutex_init(&syncMutex, NULL);
        pthread_cond_init(&syncCond, NULL);
        siNumRegThr   = 0;
        bInitialized = true;
      }
    });
}

/****************************************************************************
 * Function: thread
 *   Synchronizer main thread function
 *
 * Returns:
 *   <nothing>
 ***************************************************************************/
void * synchro_thread(void * arg)
{
  int i = 0;

  for (;;) {
    MUTEXED(syncMutex, {
        while (siNumSyncThr < siNumRegThr);

        while (i < siNumSyncThr) { 
          ptListSync[i].thrId = ptListSync[i].siIdxSync = 0;
          ++i;
        } 
        siNumSyncThr = 0;

        gettimeofday(&nowTm, NULL);
        untilTm.tv_sec  = nowTm.tv_sec + 1;
        untilTm.tv_usec = 0;

        gettimeofday(&lastTm, 0);
        usec = (untilTm.tv_usec - lastTm.tv_usec);
        if (untilTm.tv_sec != lastTm.tv_sec) 
          usec += ((untilTm.tv_sec - lastTm.tv_sec) * 1000000);
        if (usec > 0) usleep(usec);

        pthread_cond_broadcast( &syncCond ); 
      });
  } 
}

/****************************************************************************
 * Function: launch
 *   Launchs synchronizer
 *
 * Returns:
 *   <nothing>
 ***************************************************************************/
void synchro_launch()
{
  SPAWN_THREAD_TO_RUN( synchro_thread, thrSynchronizer );
}

/****************************************************************************
 * Function: sync
 *   Synchronize thread
 *
 * Returns:
 *   <nothing>
 ***************************************************************************/
void synchro_sync(pthread_t thrId)
{
  int i = 0, k = 0;
  while ((i < siNumRegThr) && 
         (ptListThr[i].thrId != thrId)) { ++i; }

  if (i < siNumRegThr) {

    MUTEXED(syncMutex, {
        k = ptListThr[i].siIdxSync;
        assert(ptListSync[k].thrId == 0);

        ptListSync[k].thrId     = thrId;
        ptListSync[k].siIdxSync = i;
        
        siNumSyncThr++;

        pthread_cond_wait( &syncCond, &syncMutex );
      });  
  }
}

/****************************************************************************
 * Function: subscribe
 *   Waits until a given epoch
 *
 * Parameters:
 *   elmt - (I) The element being subscribed
 *
 * Returns:
 *   <nothing>
 ***************************************************************************/
void synchro_register(pthread_t thrId)
{
  int i = 0;
  while ((i < siNumRegThr) && 
         (ptListThr[i].thrId != thrId)) { ++i; }
  if (i >= siNumRegThr) {
    ptListThr[siNumRegThr].thrId     = thrId;
    ptListThr[siNumRegThr].siIdxSync = siNumRegThr;
    siNumRegThr++;
  }
}

/******************************************************************************
 * Group: Implementation of Private Functions
 ******************************************************************************/

/* End of file */
