#!/bin/bash
##############################################################################
# File       : CreateDockerImgs.sh - Generates docker images from Dockerfiles
# Domain     : QPF.run.dockerfiles
# Version    : 2.0
# Date       : 2018/02/08
# Copyright (C) 2015-2018  J C Gonzalez
#_____________________________________________________________________________
# Purpose    : Generates docker images from Dockerfiles
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

LOG_FILE=${SCRIPT_PATH}/${SCRIPT_NAME}.log

#- Script variables

# NEXUS DOCKER REPOSITORY URL
NEXUS_DOCKER_URL="scidockreg.esac.esa.int:60400"
NEXUS_USER="eucops"

# DOMAINS
DOMAINS="QDT,QDTsrv,LE1sim"
GENERATE="no"
TAG="no"
PUSH="no"
RETRIEVE="no"

#- Process command line options

while getopts :u:d:n:gtpr OPT; do
    case $OPT in
        u|+u)
            NEXUS_USER="$OPTARG"
            ;;
        d|+d)
            DOMAINS="$OPTARG"
            ;;
        n|+n)
            NEXUS_DOCKER_URL="$OPTARG"
            ;;
        g|+g)
            GENERATE="yes"
            RETRIEVE="no"
            ;;
        t|+t)
            TAG="yes"
            RETRIEVE="no"
            ;;
        p|+p)
            TAG="yes"
            PUSH="yes"
            RETRIEVE="no"
            ;;
        r|+r)
            GENERATE="no"
            TAG="no"
            PUSH="no"
            RETRIEVE="yes"
            ;;
        *)
            echo "Usage: "
            echo "  ${0##*/} [ -d <domains> ] [ -u <user> ] [ -n <nexus-url> ] [ -g ] [ -t ] [ -p ]"
            echo "where: "
            echo "  -d <domains>    Comma separated list of domains to generate the images"
            echo "                  Default: ${DOMAINS}"
            echo "  -u <user>       User area in Nexus repository to store the images"
            echo "                  Default: ${NEXUS_USER}"
            echo "  -n <nexus-url>  Nexus repository main URL"
            echo "                  Default: ${NEXUS_DOCKER_URL}"
            echo "  -g              Generate the images (Default: no)"
            echo "  -t              Create the image tags (Default: no)"
            echo "  -p              Push the images to the Nexus repository, implies -t (Default: no)"
            echo "  -r              Retrieve the images from the Nexus repository.  This option"
            echo "                  resets any -g, -t or -p option before (Default: no)"
            exit 2
    esac
done
shift $(( OPTIND - 1 ))
OPTIND=1

#- Loop on Domains
for domain in $(echo ${DOMAINS} | tr ',' ' '); do

    #- Switch to domain
    cd ${domain}
    
    for dckFile in Dockerfile__* ; do

        echo "###______ PROCESSING FILE ${dckFile} ______________________________"
        #- Set image name
        imgName=$(echo $dckFile | sed -e 's/Dockerfile__//g')
        imgId=$(echo ${imgName} | cut -d: -f1)
        imgTag=$(echo ${imgName} | cut -d: -f2)
        
        tagDigit=$(echo ${imgTag} | cut -c1)

        if [ "$GENERATE" = "yes" ]; then        
            #- Generate image
            echo "####### GENERATING IMAGE ${imgName}"
            make -k -f ${SCRIPT_PATH}/Makefile.img -C ${SCRIPT_PATH}/${domain} \
                 DOCKERFILE=${dckFile} IMAGE_NAME=${imgName} clean image
        fi
        
        if [ "$TAG" = "yes" ]; then
            #- Tag image
            if [ ${tagDigit} -gt 0 ]; then
                latestImgName=${imgName}
                latestImgNameTag=${imgId}:latest
            fi
        
            echo "####### TAGGING IMAGE"
            docker tag ${imgName} ${NEXUS_DOCKER_URL}/${NEXUS_USER}/${imgName}
        fi

        if [ "$PUSH" = "yes" ]; then
            #- Push images
            echo "####### PUSH IMAGES TO NEXUS"
            docker push ${NEXUS_DOCKER_URL}/${NEXUS_USER}/${imgName}
        fi

        if [ "$RETRIEVE" = "yes" ]; then
            docker pull ${NEXUS_DOCKER_URL}/${NEXUS_USER}/${imgName}
            docker tag ${NEXUS_DOCKER_URL}/${NEXUS_USER}/${imgName} ${imgName} 
            if [ ${tagDigit} -gt 0 ]; then
                latestImgName=${imgName}
                latestImgNameTag=${imgId}:latest
                docker tag ${latestImgName} ${latestImgNameTag} 
                docker tag ${NEXUS_DOCKER_URL}/${NEXUS_USER}/${latestImgName} \
                           ${NEXUS_DOCKER_URL}/${NEXUS_USER}/${latestImgNameTag}
            fi
        fi
           
    done

    #- Tag and push latest

    if [ "$TAG" = "yes" ]; then
        echo "####### TAGGING LATEST IMAGE"
        docker tag ${latestImgName} ${latestImgNameTag}
        docker tag ${NEXUS_DOCKER_URL}/${NEXUS_USER}/${latestImgName} \
                   ${NEXUS_DOCKER_URL}/${NEXUS_USER}/${latestImgNameTag}
    fi
    
    if [ "$PUSH" = "yes" ]; then
        echo "####### PUSHING LATEST IMAGE"
        docker push ${NEXUS_DOCKER_URL}/${NEXUS_USER}/${latestImgName} 
        docker push ${NEXUS_DOCKER_URL}/${NEXUS_USER}/${latestImgNameTag}
    fi
    
    #- back
    cd ..
    
done

