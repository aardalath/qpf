#!/bin/bash
##############################################################################
# File       : BuildQPF.sh - QPF Compilation and Installation script
# Domain     : QPF.scripts
# Version    : 2.0
# Date       : 2017/09/08
# Copyright (C) 2015-2018 J C Gonzalez
#_____________________________________________________________________________
# Purpose    : Compile and Install QPF binaries in target platform
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
BUILD_PATH="${QPF_PATH}"/build
RUN_PATH="${QPF_PATH}"/run
CONTRIB_PATH="${QPF_PATH}"/contrib

QPF_WA_SRCDIR="${RUN_PATH}/qpf"
QPF_SQ_SCPT="${RUN_PATH}/qpfdb.sql"

status=0

#- Messages
_ONHDR="\e[1;49;93m"
_ONMSG="\e[1;49;92m"
_ONRUN="\e[0;49;32m"
_ONERR="\e[1;49;91m"
_OFF="\e[0m"
STEP=0

#- Options
COMPILE="no"
INSTALL="no"
INSTALL_WA="no"
FAKE="no"
REMOVE="no"
HMI="yes"
RECREATEDB="no"

CHGTODIR=""

WORK_AREA="${HOME}"
QPF_WA_TGTDIR="${WORK_AREA}/qpf"
PSQL_HOST="localhost"
PSQL_PORT="5432"
IP=""
VERBOSE="no"

#- Other
DATE=$(date +"%Y%m%d%H%M%S")
LOG_FILE=${SCRIPT_PATH}/build_${DATE}.log
VERSION=$(awk -F\" "/APP_RELEASE/{print $2;}" ${QPF_PATH}/version.h)
LDLIBS=$(echo $LD_LIBRARY_PATH | tr ':' ' ')

wsvn=$(which svn 2>/dev/null)
if [ -n "$wsvn" ]; then
    REV_ID=$(svn info ${QPF_PATH} 2>/dev/null | awk '/^Revision:/{print $2;}')
fi

if [ -z "$REV_ID" ]; then
    wgit=$(which git 2>/dev/null)
    if [ -n "$wgit" ]; then
        REV_ID=$(git rev-parse HEAD 2>/dev/null)
    fi
fi

if [ -z "$REV_ID" ]; then
        REV_ID="NON-TRACKED-WORK-COPY-COMPILATION-00000"
fi

echo "Revision number: ${REV_ID}"
BUILD_ID="${DATE}_${REV_ID}"
echo "BUILD_ID: ${BUILD_ID}"
export BUILD_ID

MAKE_OPTS="-k "
BUILD_FLAGS=""
COMP_FLAGS=""

CMAKE_OPTS="-D CMAKE_INSTALL_PREFIX:PATH=${WORK_AREA}/qpf"
CMAKE_OPTS="$CMAKE_OPTS -DCMAKE_BUILD_TYPE=Debug "
CMAKE_OPTS="$CMAKE_OPTS --graphviz=dependencies.dot"

############################################################
###### Handy functions
############################################################

greetings () {
    say "${_ONHDR}==============================================================================="
    say "${_ONHDR} Euclid QLA Processing Framework"
    say "${_ONHDR} Version ${VERSION}"
    say "${_ONHDR} Compilation and Installation process"
    say "${_ONHDR} Build time-stamp: ${DATE}"
    say "${_ONHDR}==============================================================================="
    say ""
}

usage () {
    local opts="[ -h ] [ -c ] [ -i ] [ -n ] [ -r ] [ -b ] [ -w ] [ -D def ] [ -D def ] [ -p ]"
    opts="$opts [ -W <path> ] [ -H <host> ] [ -P <port> ] [ -I <ip> ] [ -v ]"
    say "Usage: ${SCRIPT_NAME} $opts"
    say "where:"
    say "  -h         Show this usage message"
    say "  -c         Compile the source code"
    say "  -i         Install libraries and binary"
    say "  -n         Show the actions without executing them"
    say "  -r         Clear previous build"
    say "  -b         Re-create PostsgreSQL system database"
    say "  -w         Initializes QPF working area"
    say "  -B DEF     Define build macros"
    say "  -D DEF     Define compile macros"
    say "  -p         Processing-only node: do not compile QPF HMI"
    say "  -W <path>  Folder to locate QPF working area (default:HOME)"
    say "  -H <host>  Host where system database is located (default:${PSQL_HOST})"
    say "  -P <port>  Port to access the database (default:${PSQL_PORT})"
    say "  -I <ip>    IP address to use in the sample config. file"
    say "  -v         Make output verbose"
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

perform () {
    if [ "${FAKE}" == "yes" ]; then
        say "${_ONRUN}: $*"
    else
        (((( eval $* 2>&1 ; echo $? >&3) | tee -a "${LOG_FILE}" >&4) 3>&1) | (read xs; echo $xs > /tmp/buildqpf.xs)) 4>&1
        status=$(cat /tmp/buildqpf.xs)
        if [ "${LAZY}" != "yes" ]; then
            if [ $status -gt 0 ]; then
                echo "Exiting . . ."
                exit $status
            fi
        fi
    fi
}

perform_dontexit () {
    if [ "${FAKE}" == "yes" ]; then
        say "${_ONRUN}: $*"
    else
        (((( eval $* 2>&1 ; echo $? >&3) | tee -a "${LOG_FILE}" >&4) 3>&1) | (read xs; echo $xs > /tmp/buildqpf.xs)) 4>&1
        status=$(cat /tmp/buildqpf.xs)
    fi
}

checkapp () {
    local app=$1
    wapp=$(which ${app})
    [ -z "${wapp}" ] && die "Program '${app}' appear is either not installed or not available"
    eval ${app}_exe="${wapp}"
    say "  - Program '${app}' found!"
}

searchlib () {
    local lib=$1
    wlib=""
    for l in ${LDLIBS}; do
        if [ -z "$wlib" ]; then
            wlib=$(find $l \( -name "lib${lib}*.so" -o -name "lib${lib}.a" \) -print 2>/dev/null)
        fi
    done
    if [ -z "${wlib}" ]; then
        searchsyslib $lib
    fi
    say "  - Library '${lib}' found!"
}

searchsyslib () {
    local lib=$1
    wlib=$(ldconfig -p | fgrep lib${lib}.)
    [ -z "${wlib}" ] && die "Library '${lib}' appear is either not installed or not reachable"
    say "  - Library '${lib}' found!"
}

install_exe () {
    local exe=$1
    say "  - Installing executable $exe"
    perform cp "'${BUILD_PATH}/${exe}'" "'${WORK_AREA}/qpf/bin/'"
}

install_lib () {
    local lib=$1
    say "  - Installing library $lib"
    for l in  "${BUILD_PATH}/"${lib}* ; do
        perform cp -d "'$l'" "'${WORK_AREA}/qpf/lib/'"
    done
}

install_scpt () {
    local scpt=$1
    say "  - Installing script $scpt"
    perform cp "'${SCRIPT_PATH}/${scpt}'" "'${WORK_AREA}/qpf/bin/'"
    chmod ugo+x "${WORK_AREA}/qpf/bin/${scpt}"
}

install_contrib () {
    local fil=$1
    local tgtdir=$2
    bn=$(basename "${CONTRIB_PATH}/${fil}")
    say "  - Installing file $fil"
    if [ ! -d "${WORK_AREA}/${tgtdir}" ]; then
        mkdir -p "${WORK_AREA}/${tgtdir}"
    fi
    if [ ! -f "${WORK_AREA}/${tgtdir}/${bn}" ]; then
	perform cp "'${CONTRIB_PATH}/${fil}'" "'${WORK_AREA}/${tgtdir}'"
    fi
}

############################################################
###### Start
############################################################

## Parse command line
while getopts :hcinrbB:C:D:pwW:H:P:I:v OPT; do
    case $OPT in
        h|+h) usage ;;
        c|+c) COMPILE="yes" ;;
        i|+i) INSTALL="yes" ;;
        n|+n) FAKE="yes" ;;
        r|+r) REMOVE="yes" ;;
        b|+b) RECREATEDB="yes" ;;
        w|+w) INSTALL_WA="yes" ;;
        B|+B) BUILD_FLAGS="${BUILD_FLAGS} -D $OPTARG" ;;
        C|+C) CHGTODIR="$OPTARG" ;;
        D|+D) COMP_FLAGS="${COMP_FLAGS} -D$OPTARG" ;;
        p|+p) HMI="no" ;;
        W|+W) WORK_AREA="$OPTARG" ;;
        H|+H) PSQL_HOST="$OPTARG" ;;
        P|+P) PSQL_PORT="$OPTARG" ;;
        I|+I) IP="$OPTARG" ;;
        v|+v) VERBOSE="yes" ;;
        *)    usage ; exit 2
    esac
done
shift `expr $OPTIND - 1`
OPTIND=1

## Say hello
greetings

## Checking required applications and libraries are installed
step "Checking required applications and libraries are installed"

#checkapp qmake
checkapp make
checkapp psql

if [ "${HMI}" == "yes" ]; then
    searchlib Qt
fi
#searchlib zmq
#searchlib nanomsg
searchlib pcre2
#searchlib sodium
#searchlib curl
searchlib pq

step "Ensuring contributions to COTS are properly installed"

#install_contrib cppzmq-master/zmq.hpp opt/zmq/include
#install_contrib pcre2/PCRegEx.h       opt/pcre2/include

CMAKE_OPTS="$CMAKE_OPTS $BUILD_FLAGS"

if [ "${VERBOSE}" == "yes" ]; then
    CMAKE_OPTS="$CMAKE_OPTS -DCMAKE_VERBOSE_MAKEFILE=ON"
else
    CMAKE_OPTS="$CMAKE_OPTS -DCMAKE_VERBOSE_MAKEFILE=OFF"
fi

if [ -n "${COMP_FLAGS}" ]; then
    CMAKE_OPTS="$CMAKE_OPTS -DCMAKE_C_FLAGS=\"$COMP_FLAGS\""
    CMAKE_OPTS="$CMAKE_OPTS -DCMAKE_CXX_FLAGS=\"$COMP_FLAGS\""
fi

## Creating build folder
step "Creating build folder"

if [ "${REMOVE}" == "yes" ]; then
    perform rm -rf ${BUILD_PATH}
fi
perform mkdir -p "'${BUILD_PATH}'"

cd "${BUILD_PATH}"

## Generating dependencies and setting makefiles
if [ "${REMOVE}" == "yes" ]; then
    step "Generating dependencies and setting makefiles"
    if [ "${HMI}" == "yes" ]; then
        perform cmake -D HMI=ON ${CMAKE_OPTS} ..
    else
        perform cmake -D HMI=OFF ${CMAKE_OPTS} ..
    fi
fi

## Compiling source code
COMPLSTATUS=0
if [ "${COMPILE}" == "yes" ]; then
    step "Compiling source code"
    perform make ${MAKE_OPTS}
    COMPLSTATUS=${status}
fi

## Setting up Work Area in /tmp
if [ "${INSTALL_WA}" == "yes" ]; then
    step "Setting up Work Area under '${WORK_AREA}'"

    if [ ! -d "${WORK_AREA}" ]; then
         perform mkdir -p "'${WORK_AREA}'"
    fi

    if [ ! -d "${QPF_WA_TGTDIR}" ]; then
         perform mkdir -p "'${QPF_WA_TGTDIR}'"
    fi

    #perform tar xzCf "'${WORK_AREA}'" "'${QPF_WA_SRCDIR}'"
    perform cp -R "'${QPF_WA_SRCDIR}'"/*  "'${QPF_WA_TGTDIR}'"/
    perform cp -R "'${SCRIPT_PATH}'"/maint/*  "'${QPF_WA_TGTDIR}'"/scripts/
    perform cp -R "'${SCRIPT_PATH}'"/lib  "'${QPF_WA_TGTDIR}'"/scripts/
fi

## Installing QPF executable and libraries
if [ "${INSTALL}" == "yes" ]; then
    step "Installing QPF executables and libraries"

    cd "${BUILD_PATH}"
    make install
    if [ "${HMI}" == "yes" ]; then
        ln -sf ${WORK_AREA}/qpf/bin/qpfgui ${WORK_AREA}/qpf/bin/qpfhmi
    fi

    install_scpt RunQPF.sh
    install_scpt qpfapp

    if [ ! -d "'${HOME}/.qpf'" ]; then
        perform mkdir -p "'${HOME}'"/.qpf
        perform cp "'${SCRIPT_PATH}'"/qpfapp.config "'${HOME}'"/.qpf/config
        perform cp -R "'${SCRIPT_PATH}'"/lib "'${WORK_AREA}'"/qpf/scripts
    fi

    #QPF_INI="${RUN_PATH}/QPFHMI.conf"
    #if [ ! -f "${HOME}/.config/QPF/${QPF_INI}" ]; then
    #    mkdir -p ${HOME}/.config/QPF
    #    cp "${QPF_INI}" ${HOME}/.config/QPF
    #fi
fi

## Creating initial config file for current host
if [ "${INSTALL}" == "yes" ]; then
    step "Creating sample config file from template"

    # Get Host name
    hnamefull=$(uname -n)
    hname=$(uname -n|cut -d. -f1)

    if [ -n "$IP" ]; then
        hip="$IP"
    else
        # Get IP addresses
        ipsfile=/tmp/$$.ip
        ip addr | \
            grep 'state UP' -A2 | \
            awk '{net=$2;getline;getline;print net,$2;getline;}' | \
            cut -f1  -d'/' > $ipsfile

        # Select IP address if several interfaces
        nip=$(wc -l $ipsfile | cut -d" " -f1)
        if [ $nip -gt 1 ]; then
            PS3='Select the interface to use to configure QPF: '
            k=1
            while read if ip ; do
                options[$k]="$if - IP address: $ip"
                k=$((k + 1))
            done < $ipsfile
            echo "${options[@]}"
            select opt in "${options[@]}"
            do
                hip=$(echo "$opt"|cut -d" " -f 5)
                if [ -z "$ip" ]; then
                    echo "Invalid option"
                else
                    echo "Selected option: $opt"
                    break
                fi
            done
        else
            cat $ipsfile | read hif hip
        fi
        rm -f $ipsfile
    fi

    say "hname = ${hname}"
    say "hip = ${hip}"
    say "target = ${WORK_AREA}/qpf/cfg/qpf-test-${hname}.json"
    # Finally, generate sample config file from template
    sed -e "s/@THIS_HOST_ADDR@/$hnamefull/g" \
        -e "s/@THIS_HOST_IP@/$hip/g" \
        ${WORK_AREA}/qpf/cfg/tpl.cfg.json \
        > ${WORK_AREA}/qpf/cfg/qpf-test-${hname}.json
fi

## Creating QPFDB database
step "Setting up QPF database"

if [ "${RECREATEDB}" == "yes" ]; then
    perform bash ${SCRIPT_PATH}/InitializeDB.sh \
            -h ${PSQL_HOST} -p ${PSQL_PORT} -o "'${LOG_FILE}.sqlout'"
fi

## Finishing
step "Final comments"

say "Compilation/Installation finished."
say ""
say "-------------------------------------------------------------------------------"
say "Please, do not forget to:"
say "  - include the directory ${WORK_AREA}/qpf/bin in the PATH variable, and"
say "  - include the directory ${WORK_AREA}/qpf/lib in the LD_LIBRARY_PATH variable."
say "To do that, just execute the following commands:"
say "  export PATH=${WORK_AREA}/qpf/bin:${WORK_AREA}/qpf/scripts:${WORK_AREA}/qpf/scripts/lib:\$PATH"
say "  export LD_LIBRARY_PATH=${WORK_AREA}/qpf/lib:\$LD_LIBRARY_PATH"

(cat ~/env_qpf.sh 2>/dev/null ; echo ""; echo "# BuildQPF section") | \
awk '(NR==1),/BuildQPF section/' > /tmp/$$.sh
cat /tmp/$$.sh > ~/env_qpf.sh
cat <<EOF>> ~/env_qpf.sh
export PATH=${WORK_AREA}/qpf/bin:${WORK_AREA}/qpf/scripts:${WORK_AREA}/qpf/scripts/lib:\$PATH
export LD_LIBRARY_PATH=${WORK_AREA}/qpf/lib:\$LD_LIBRARY_PATH
EOF
  
say ""
say "For your convenience, these commands have been saved into the file:"
say "  \$HOME/env_qpf.sh"
say "so that you can just update your environment by typing:"
say "  source \$HOME/env_qpf.sh"
say ""
say "In order to check that the QPF HMI executable and the libraries were correctly"
say "installed, you may run:"
say "  $ qpfapp core status "
say "and see if the application shows a help message."
say ""
say "Initial configuration files (in JSON) are available at ${WORK_AREA}/qpf/cfg."
say "You may need to modify them before launching any processing task from the HMI."
say "(Runing qpfhmi without specifying a configuration file will mean reading"
say "last configuration used from internal database)."
say ""
say "Finally, to start the QPF Core, just type:"
say "$ qpfapp core start"
say "$ qpfapp core status"
say "-------------------------------------------------------------------------------"
say ""

exit $COMPLSTATUS
