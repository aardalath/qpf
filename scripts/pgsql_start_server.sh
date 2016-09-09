#!/bin/bash
##############################################################################
# File       : pgsql_start_server.sh - PostgreSQL Server start
# Domain     : QPF.scripts
# Version    : 1.0
# Date       : 2016/09/01
# Copyright (C) 2015, 2016 J C Gonzalez
#_____________________________________________________________________________
# Purpose    : PostgreSQL Server start
# Created by : J C Gonzalez
# Status     : Prototype
# History    : See <Changelog>
###############################################################################

pg_ctl -D /home/eucops/opt/pgsql/data -l /home/eucops/opt/pgsql/data/log start

 
