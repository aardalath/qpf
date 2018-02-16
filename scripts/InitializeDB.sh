#!/bin/bash
##############################################################################
# File       : Initialize.sh - Drops and recreates QPF database
# Domain     : QPF.scripts
# Version    : 1.0
# Date       : 2017/09/08
# Copyright (C) 2015-2018 J C Gonzalez
#_____________________________________________________________________________
# Purpose    : Drops and recreate qpfdb database
# Created by : J C Gonzalez
# Status     : Prototype
# History    : See <Changelog>
###############################################################################

###### Script variables

#- This script path and name
SCRIPT_PATH="${BASH_SOURCE[0]}";
SCRIPT_NAME=$(basename "${SCRIPT_PATH}")
if [ -h "${SCRIPT_PATH}" ]; then
    while [ -h "${SCRIPT_PATH}" ]; do
        SCRIPT_PATH=$(readlink "${SCRIPT_PATH}")
    done
fi
pushd . > /dev/null
cd $(dirname ${SCRIPT_PATH}) > /dev/null
SCRIPT_PATH=$(pwd)
popd  > /dev/null

#- QPF source code paths
QPF_PATH=$(dirname "${SCRIPT_PATH}")
RUN_PATH="${QPF_PATH}"/run
QPF_SQ_SCPT="${RUN_PATH}/qpfdb.sql"

PSQL_OPTS="$*"

psql postgres ${PSQL_OPTS} -q -c "DROP DATABASE qpfdb;" 

( psql postgres ${PSQL_OPTS} -q -c "CREATE DATABASE qpfdb OWNER eucops;" && \
  psql qpfdb    ${PSQL_OPTS} -q -f "${QPF_SQ_SCPT}" ) || exit $?

