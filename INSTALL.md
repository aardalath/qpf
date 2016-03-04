QPF - QLA Processing Framework - Installation
=============================================

Introduction
----------------------

This document explains the installation procedure for the QLA Processing Framework (QPF) version 1.0.

QPF is a C++ application that makes use of many of the C++-11 new features of the C++ language.  It was originally developed in an Ubuntu 15.10 box with a GCC compiler version 5.2.1 (Ubuntu 5.2.1-22ubuntu2 - 20151010).  So the compiler you use must be compatible with this one.

The QPF Graphical User Interface is based on Qt 5.4.1 (GCC 4.6.1, 64 bit), built on Apr 22nd 2015 04:12:47 from revision 6302a28c97, and developed using Qt Creator 3.4.0 (opensource).

In addition, some COTS detailed below were used.

COTS
--------------

The following is the list of COTS needed to build and run QPF v-1.0.  The version numbers are used for reference: those versions or higher with backward-compatibility can be used.

- JSONCpp (v-1.6.2)
- 0MQ Library (v-4.1.1), together with the C++ binding for 0MQ
- PostgreSQL (v-9.4.4.1 x64)
- libsodium (v-1.0.3)

Installation procedure
--------------------------------
