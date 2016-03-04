/******************************************************************************
 * File:    propdef.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.propdef
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
 *   Macro declaration for definition of class data properties
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

#ifndef PROPDEF_H
#define PROPDEF_H

//------------------------------------------------------------
// Topic: Declarations
//------------------------------------------------------------

#define SET_NAMED_PROPERTIES

#ifdef SET_NAMED_PROPERTIES
#define Property(c,t,p,n)					\
  private: t p;							\
  public:							\
  inline t get ## n (void) const { return p; }			\
  inline t get ## n (void) { return p; }			\
  inline c & set ## n (t new ## n ) { p = new ## n ; return *this; }	
# define with(s)   s
#else
# define Property(t,p,n)					\
  private: t p;							\
  public:							\
  inline t get ## n (void) const { return p; }			\
  inline t get ## n (void) { return p; }			\
  inline void set ## n (t new ## n ) { p = new ## n ; }		
# ifdef with
#  undef with
# endif
#endif

#endif // PROPDEF_H
