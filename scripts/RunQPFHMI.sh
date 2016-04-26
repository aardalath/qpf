##############################################################################
# File       : RunQPFHMI.sh - Run QPF HMI
# Domain     : QPF.scripts
# Version    : 1.0
# Date       : 2016/04/01
# Copyright (C) 2015, 2016 J C Gonzalez
#_____________________________________________________________________________
# Purpose    : Run QPF HMI
# Created by : J C Gonzalez
# Status     : Prototype
# History    : See <Changelog>
###############################################################################

###### Script variables

QPFDIR=/home/eucops/qpf
VERSION=1.0
QPFHMI=${QPFDIR}/bin/qpfhmi

#- Messages
_ONHDR="\e[1;49;93m"
_ONMSG="\e[1;49;92m"
_ONRUN="\e[0;49;32m"
_ONERR="\e[1;49;91m"
_OFF="\e[0m"
STEP=0

#- Options
TIMESTAMP=$(date +"%Y%m%d%H%M%S")
LOG_FILE=./qpfhmi_${TIMESTAMP}.log
CFG_FILE="${QPFDIR}/cfg/qpf_v1_rc1_multihost_eucdev02+eucdev03.json"
DBG=""

###### Handy functions

greetings () {
    say "${_ONHDR}==============================================================================="
    say "${_ONHDR} Euclid QLA Processing Framework"
    say "${_ONHDR} Version ${VERSION}"
    say "${_ONHDR} Run QPF HMI"
    say "${_ONHDR}==============================================================================="
    say ""
}

usage () {
    local opts="[ -h ] [ -d ] [ -c <cfg> ]"
    say "Usage: ${SCRIPT_NAME} $opts"
    say "where:"
    say "  -h         Show this usage message"
    say "  -d         Debug session"
    say "  -c <cfg>   Use config. file <cfg>"
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

## Parse command line
while getopts :hdc: OPT; do
    case $OPT in
        h|+h) usage ;;
        d|+d) DBG="gdb -ex run --args" ;;
        c|+c) CFG_FILE="$OPTARG" ;;
        *)    usage ; exit 2
    esac
done
shift `expr $OPTIND - 1`
OPTIND=1

## Run
greetings

${DBG} ${QPFHMI} -c ${CFG_FILE} -t 50000 2>&1 || die "Cannot run qpfhmi" | tee ${LOG_FILE}

exit 0
