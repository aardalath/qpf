#!/bin/bash
##############################################################################
# File       : Build-QDTsrv-image.sh - QDT Service image helper
# Domain     : QPF.run.dockerfiles
# Version    : 2.0
# Date       : 2017/10/23
# Copyright (C) 2015-2018 J C Gonzalez
#_____________________________________________________________________________
# Purpose    : Create package for Dockerfile, in order to build image for QDT
#              service invocation
# Created by : J C Gonzalez
# Status     : Prototype
# History    : See <Changelog>
################################################################################

# This script creates a .tgz file with the QDT service wrapper and the QDT
# actual code, that will be used for the generation of the corresponding image
# for QDT service execution.
#
# It is expected that the first path provided correspond to the path where the
# QDTsrv.,py Python script is placed.  The second path should point to the
# directory where the QDT driver.py script is placed (whatever the version/tag/
# branch we select).
# Under the first directory, a symbolic link to the second will be created with
# the name QDT

# Usage function
usage () {
    echo "Usage:: $0 -w <wrapper-src-dir> -q <qdt-driver-dir>"
}

WRPDIR=""
QDTDIR=""

QDTSRVPKG="QDT.src.tgz"

## Parse command line
while getopts :hw:q: OPT; do
    case $OPT in
        h|+h) usage ;;
        w|+w) WRPDIR="$OPTARG" ;;
        q|+q) QDTDIR="$OPTARG" ;;
        *)    usage ; exit 2
    esac
done
shift `expr $OPTIND - 1`
OPTIND=1

# If no appropriate options provided, show help and exit
if [ \( ! -d "$WRPDIR" \) -o \( ! -d "$QDTDIR" \) ]; then
    usage
    exit 1
fi

curdir=$(pwd)

# Create symbolic link of QDT path into QDTsrv folder
cd "$WRPDIR"
[ -L QDT ] && rm QDT
ln -s "$QDTDIR" QDT

# Create package in current folder
tar cvzf ${curdir}/${QDTSRVPKG} \
    --exclude QDT/tests \
    --exclude QDT/test_functional \
    --exclude QDT/tools \
    --exclude QDT/schema \
    QDTsrv.py \
    qdttask \
    QDT/*

cd ${curdir}
ls -l ${QDTSRVPKG}

exit 0
