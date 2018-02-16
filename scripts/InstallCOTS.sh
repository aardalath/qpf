#!/bin/bash
##############################################################################
# File       : InstallCOTS.sh - COTS Installation and database initialisation
# Domain     : QPF.scripts
# Version    : 2.0
# Date       : 2017/09/08
# Copyright (C) 2015-2018 J C Gonzalez
#_____________________________________________________________________________
# Purpose    : COTS Installation and database initialisation
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

status=0

#- Messages
_ONHDR="\e[1;49;93m"
_ONMSG="\e[1;49;92m"
_ONRUN="\e[0;49;32m"
_ONERR="\e[1;49;91m"
_OFF="\e[0m"
STEP=0

#- Options
QT="no" 
PGSQL="no"
NNMSG="no" 
PCRE="no" 
CURL="no" 
UUID="no"
DOCKER="no"

#- Other
DATE=$(date +"%Y%m%d%H%M%S")
LOG_FILE=./cots_${DATE}.log
VERSION=1.0
LDLIBS=$(echo $LD_LIBRARY_PATH | tr ':' ' ')

###### Handy functions

greetings () {
    say "${_ONHDR}==============================================================================="
    say "${_ONHDR} Euclid QLA Processing Framework"
    say "${_ONHDR} Version ${VERSION}"
    say "${_ONHDR} COTS Installation and database initialisation"
    say "${_ONHDR} Execution time-stamp: ${DATE}"
    say "${_ONHDR}==============================================================================="
    say ""
}

usage () {
    local opts="[ -h ] [ -A ] [ -a ] [ -q ] [ -p ] [ -n ] [ -r ] [ -c ] [ -u ] [ -d ]"
    say "Usage: ${SCRIPT_NAME} $opts"
    say "where:"
    say "  -h         Show this usage message"
    say "  -A         Install all COTS, INCLUDING Qt framework"
    say "  -a         Install all COTS, EXCLUDING Qt framework"
    say "  -q         Install Qt framework"
    say "  -p         Install PostgreSQL, initialize database and restart server"
    say "  -n         Install Nanomsg"
    say "  -r         Install PCRE2"
    say "  -c         Install libcurl"
    say "  -u         Install UUID library"
    say "  -d         Install Docker"
    say ""
    exit 1
}

say () {
    echo -e "$*${_OFF}"
    echo -e "$*" | sed -e 's#.\[[0-9];[0-9][0-9];[0-9][0-9]m##g' >> "${LOG_FILE}"
}

step () {
    say "${_ONMSG}## STEP ${STEP} - $*"
    STEP=$(($STEP + 1))
}

die () {
    say "${_ONERR}ERROR: $*"
    exit 1
}

###### Start

#### Parse command line and display grettings

## Parse command line
while getopts :hAaqpnrcud OPT; do
    case $OPT in
        h|+h) usage 
              ;;
        A|+A) QT="yes" 
              PGSQL="yes"
              NNMSG="yes" 
              PCRE="yes" 
              CURL="yes" 
              UUID="yes" 
              DOCKER="yes" 
              ;;
        a|+a) QT="no" 
              PGSQL="yes"
              NNMSG="yes" 
              PCRE="yes" 
              CURL="yes" 
              UUID="yes" 
              DOCKER="yes" 
              ;;
        q|+q) QT="yes" 
              ;;
        p|+p) PGSQL="yes" 
              ;;
        n|+n) NNMSG="yes" 
              ;;
        r|+r) PCRE="yes" 
              ;;
        c|+c) CURL="yes" 
              ;;
        u|+u) UUID="yes" 
              ;;
        d|+d) DOCKER="yes" 
              ;;
        *)    usage 
              exit 2
              ;;
    esac
done
shift `expr $OPTIND - 1`
OPTIND=1

## Say hello
greetings

## Checking required applications and libraries are installed

#### First, enable EPEL repository

sudo yum install -y epel-release

#### Installing COTS: I - Install PostgreSQL

if [ "${PGSQL}" == "yes" ]; then 
    step "Installing PostgreSQL"

    # We need to install PostgreSQL and then setup the database

    # For LODEEN (CentOS based), the currently available PostgreSQL
    # version by default is 9.2, but we need 9.4+
    # We install a PGDG for 9.6, and then install the 9.6 packages
    PSQL_PGDG="https://download.postgresql.org/pub/repos/yum/9.6/redhat/rhel-7-x86_64/pgdg-centos96-9.6-3.noarch.rpm" 
    PSQL_PKGS="postgresql96.x86_64" 
    PSQL_PKGS="$PSQL_PKGS postgresql96-devel.x86_64" 
    PSQL_PKGS="$PSQL_PKGS postgresql96-libs.x86_64" 
    PSQL_PKGS="$PSQL_PKGS postgresql96-server.x86_64" 
    PSQL_PKGS="$PSQL_PKGS postgresql96-docs.x86_64"
    
    #- 1. Installing PostgreSQL
    say ". Installing packages"
    sudo yum -y install ${PSQL_PGDG}
    sudo yum -y install ${PSQL_PKGS}

    #- 2. Setting up the database

    # Give access to internal folder:
    say ". Preparing internal folders"
    sudo chmod 777 /var/run/postgresql
    sudo chown postgres.postgres /var/run/postgresql
    sudo mkdir -p /opt
    sudo ln -s /usr/pgsql-9.6 /usr/pgsql
    sudo ln -s /usr/pgsql-9.6 /opt/pgsql
    
    export PATH=$PATH:/usr/pgsql/bin
    
    # Then, for the creation of the local folder, initialization and server start, 
    # use the scripts =scripts/pgsql_start_server.sh= and =scripts/pgsql_initdb.sh=

    say ". Initializing database"
    source ${SCRIPT_PATH}/maint/pgsql_initdb.sh

    say ". Starting server"
    source ${SCRIPT_PATH}/maint/pgsql_start_server.sh
fi

#### Installing COTS: II - Install Qt

if [ "${QT}" == "yes" ]; then 
    step "Installing Qt framework"

    # Create QT5 list of packages
    sudo yum -y list qt5-*x86_64 | grep -v -- -examples|grep qt5- | awk '{print $1;}' | tee /tmp/qt5pkgs.list

    # Install packages
    sudo yum -y install --skip-broken $(cat /tmp/qt5pkgs.list)
fi

#### Installing COTS: III - Install Nanomsg

if [ "${NNMSG}" == "yes" ]; then 
    step "Installing Nanomsg library"

    NNMSG_NAME="nanomsg-1.0.0"
    NNMSG_PKG="${NNMSG_NAME}.tar.gz"
    NNMSG_URL="https://github.com/nanomsg/nanomsg/archive/1.0.0.tar.gz"

    curdir=$(pwd)
    cd ${SCRIPT_PATH}
    mkdir -p pkgs && cd pkgs
    wget ${NNMSG_URL} -o nanomsg.log -O ${NNMSG_PKG}
    tar xzf ${NNMSG_PKG}
    cd ${NNMSG_NAME}
    mkdir -p build && cd build
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make && sudo make install
    cd ${curdir}
fi

#### Installing COTS: IV - Install PCRE2

if [ "${PCRE}" == "yes" ]; then 
    step "Installing PCRE2 library"
    PCRE2_NAME="pcre2-10.30"
    PCRE2_PKG="${PCRE2_NAME}.tar.gz"
    PCRE2_URL="https://ftp.pcre.org/pub/pcre/${PCRE2_PKG}"

    curdir=$(pwd)
    cd ${SCRIPT_PATH}
    mkdir -p pkgs && cd pkgs
    wget ${PCRE2_URL} -o nanomsg.log -O ${PCRE2_PKG}
    tar xzf ${PCRE2_PKG}
    cd ${PCRE2_NAME}
    ./configure --prefix=/usr/local
    make && sudo make install
    cd ${curdir}
fi

#### Installing COTS: V - Install curl

if [ "${CURL}" == "yes" ]; then 
    step "Installing Curl library"
    sudo yum -y install curl.x86_64 libcurl-devel.x86_64
fi

#### Installing COTS: VI - Install UUID

if [ "${UUID}" == "yes" ]; then 
    step "Installing UUID library"
    sudo yum -y install uuid-devel.x86_64 libuuid-devel.x86_64
fi

#### Installing COTS: VII - Install Docker

if [ "${DOCKER}" == "yes" ]; then 
    step "Installing Docker"
    say ". Installing packages"
    sudo yum -y install docker
    say ". Creating docker group"
    sudo groupadd docker
    say ". Adding user to docker group"
    sudo usermod -aG docker $(whoami)
    say ". Starting Docker service"
    sudo service docker start
fi

#### Finishing
step "Final comments"

cat <<EOF> ${HOME}/env_qpf.sh
#!/bin/bash
# Mini-script to update PATH and LD_LIBRARY_PATH environment variables 
# for the compilation/execution of Euclid QPF
# Creation date: ${DATE}
export PATH=/usr/lib64/qt5/bin:/usr/local/bin:/usr/pgsql/bin:\$PATH
export LD_LIBRARY_PATH=/usr/lib64:/usr/local/lib:/usr/local/lib64:/usr/pgsql/lib:\$LD_LIBRARY_PATH
EOF
say ""
say "Compilation/Installation finished."
say ""
say "-------------------------------------------------------------------------------"
say "Please, do not forget to include folder /usr/lib64 in the LD_LIBRARY_PATH "
say "variable, and the /usr/lib64/qt5/bin folder in the PATH variable, with these"
say "commands:" 
say "  export PATH=/usr/lib64/qt5/bin:/usr/local/bin:/usr/pgsql/bin:\$PATH"
say "  export LD_LIBRARY_PATH=/usr/lib64:/usr/local/lib:/usr/local/lib64:/usr/pgsql/lib:\$LD_LIBRARY_PATH"
say ""
say "For your convenience, these commands have been saved into the file:"
say "  \$HOME/env_qpf.sh"
say "so that you can just update your environment by typing:"
say "  source \$HOME/env_qpf.sh"
say "-------------------------------------------------------------------------------"
say ""
say "Done."

