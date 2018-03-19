##############################################################################
# File       : RunQPFHMI.sh - Run QPF HMI
# Domain     : QPF.scripts
# Version    : 2.0
# Date       : 2017/09/08
# Copyright (C) 2015-2018 J C Gonzalez
#_____________________________________________________________________________
# Purpose    : Run QPF HMI
# Created by : J C Gonzalez
# Status     : Prototype
# History    : See <Changelog>
###############################################################################

###### Script variables

QPFDIR=/home/eucops/qpf
VERSION=2.0
QPF=${QPFDIR}/bin/qpf
QPFHMI=${QPFDIR}/bin/qpfgui
QPF_SESSIONS_DIR=${QPFDIR}/run

#- Messages
_ONHDR="\e[1;49;93m"
_ONMSG="\e[1;49;92m"
_ONRUN="\e[0;49;32m"
_ONERR="\e[1;49;91m"
_OFF="\e[0m"
STEP=0

#- Options
TIMESTAMP=$(date +"%Y%m%d%H%M%S")
LOG_FILE="${QPFDIR}/log/qpfhmi_${TIMESTAMP}.log"

QPFEXE=${QPF}

DBG=""

CFG_FILE="-c ${QPFDIR}/cfg/*.json"
DBG=""
SESSION=""
IPADDR=""
MODE="Core"

###### Handy functions

greetings () {
    say "${_ONHDR}==============================================================================="
    say "${_ONHDR} Euclid QLA Processing Framework"
    say "${_ONHDR} Version ${VERSION}"
    say "${_ONHDR} Run QPF ${MODE}"
    say "${_ONHDR}==============================================================================="
    say ""
}

usage () {
    SCRIPT_NAME=$0
    local opts2="[ -h ] [ -d ] [ -v ] [ -H ]"
    local opts1="[ -h ] [ -d ] [ -v ] [ -c <cfg> ] [ -I <ipAddr> ] [ -s <session> ]"
    say "Usage:   ${SCRIPT_NAME} $opts1"
    say "         ${SCRIPT_NAME} $opts2"
    say "where:"
    say "  -h             Show this usage message"
    say "  -d             Debug session"
    say "  -v             Increases verbosity (can be used more than once)"
    say "  -H             HMI"
    say "  -C             HMI - Show Configuration Tool"
    say "  -c <cfg>       Use config. file <cfg>"
    say "  -s <session>   Re-use existing session tag/folder"
    say "  -I <ipAddr>    Use this IP address for master"
    say ""
    exit 1
}

say () {
    echo -e "${_ONMSG}$*${_OFF}"
    echo "$*" >> ${LOG_FILE}
}

die () {
    say "${_ONERR}ERROR: $*"
    exit 1
}

###### Start

## Define default connection parameters

DBUSER=eucops
DBPASSWD=""
DBHOST=127.0.0.1
DBPORT=5432
DBNAME=qpfdb
DB_CONNECTION="db://${DBUSER}:${DBPASSWD}@${DBHOST}:${DBPORT}/${DBNAME}"

LAST_SESSION=$(basename $(ls -1dt ${QPF_SESSIONS_DIR}/20*|head -1))

THIS_HOST=$(uname -a|cut -d" " -f2)

VERBOSITY=""

## Parse command line
while getopts :hHCdc:s:I:v OPT; do
    case $OPT in
        h|+h) 
	    usage 
	    ;;
        H|+H) 
	    QPFEXE=${QPFHMI} 
	    CFG_FILE="-c ${DB_CONNECTION}"
	    SESSION="-s ${LAST_SESSION}"
	    THIS_HOST=localhost
        MODE="HMI"
        ;;
        C|+C) 
	    QPFEXE=${QPFHMI} 
	    CFG_FILE="-C -c ${DB_CONNECTION}"
	    SESSION="-s ${LAST_SESSION}"
	    THIS_HOST=localhost
	    ;;
        d|+d) 
	    DBG="gdb -ex run --args" 
	    ;;
        c|+c) 
	    CFG_FILE="-c $OPTARG" 
	    ;;
        s|+s) 
	    SESSION="-s $OPTARG" 
	    ;;
        I|+I) 
	    IPADDR="-I $OPTARG"
	    ;;
        v|+v) 
	    VERBOSITY="$VERBOSITY -v"
	    ;;
        *)    usage ; exit 2
    esac
done
shift `expr $OPTIND - 1`
OPTIND=1

## Run
greetings

ALL_OPTS="${CFG_FILE} ${SESSION} ${IPADDR} ${VERBOSITY}"
HOSTNAME=${THIS_HOST} ${DBG} ${QPFEXE} ${ALL_OPTS} 2>&1 | tee ${LOG_FILE}

if [ $? -ne 0 ]; then
    die "Exiting..."
else 
    say "Done."
fi

exit 0
 
