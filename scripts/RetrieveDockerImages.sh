#!/bin/bash
##############################################################################
# File       : RetrieveDockerImages.sh - Retrieve Docker Images from Nexus
# Domain     : QPF.scripts
# Version    : 2.0
# Date       : 2018/02/16
# Copyright (C) 2015-2017 J C Gonzalez
#_____________________________________________________________________________
# Purpose    : Retrieve Docker Images from Nexus Repository
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

usage () {
    local opts="[ -h ] [ -r <url> ] [ -u <user> ] [ -i <imgs> ] [ -f <imgf> ]"
    echo "Usage: ${SCRIPT_NAME} $opts"
    echo "where:"
    echo "  -h         Show this usage message"
    echo "  -r <url>   Repository URL"
    echo "  -u <user>  Repository user"
    echo "  -i <imgs>  Comma separated list of image names: img1,img2,...,imgn"
    echo "  -f <imgf>  Name of file with image names, one per line"
    echo ""
    exit 1
}

#- Default (Euclid) values
NEXUS_URL=scidockreg.esac.esa.int:60400
NEXUS_USER=eucops
DCK_IMAGES=""

############################################################
###### Start
############################################################

## Parse command line
while getopts :hr:u:i:f: OPT; do
    case $OPT in
        h|+h) usage ;;
        r|+r) NEXUS_URL="$OPTARG" ;;
        u|+u) NEXUS_USER="$OPTARG" ;;
        i|+i) DCK_IMAGES=$(echo "$OPTARG" | tr ',' ' ') ;;
        f|+f) DCK_IMAGES=$(cat $OPTARG) ;;
        *)    usage ; exit 2
    esac
done
shift `expr $OPTIND - 1`
OPTIND=1

## Download images

for img in ${DCK_IMAGES}; do

    imgname=${NEXUS_URL}/${NEXUS_USER}/$img

    echo ""
    echo "## Downloading image $imgname . . ."

    docker pull ${imgname} && \
    docker tag ${imgname} ${img}
    
done

echo ""
echo "done."

exit 0
