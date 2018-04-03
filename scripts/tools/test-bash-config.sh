#!/bin/bash

#-- This script path and name
sPth="${BASH_SOURCE[0]}"; sName=$(basename "${sPth}")
[ -h "${sPth}" ] && (while [ -h "${sPth}" ]; do sPth=$(readlink "${sPth}"); done)
pushd . > /dev/null && cd $(dirname ${sPth}) > /dev/null
SCRIPT_PATH=$(pwd)
popd  > /dev/null

#-- Config files
CFG_FILE=sample-bash-config.cfg

eval $(grep QPFWA ${CFG_FILE})

#============================================================================

#- Messages
_ONHDR="\e[1;49;93m"
_ONMSG="\e[1;49;92m"
_ONRUN="\e[0;49;32m"
_ONERR="\e[1;49;91m"
_OFF="\e[0m"
STEP=0

TIMESTAMP=$(date +"%Y%m%d%H%M%S")

say () {
    echo -e "${_ONMSG}$*${_OFF}"
    echo "$*" >> ${LOG_FILE}
}

die () {
    say "${_ONERR}ERROR: $*"
    exit 1
}

greetings () {
    local msgs=("$@")
    say "${_ONHDR}==============================================================================="
    for idx in "${!msgs[@]}"; do say "${_ONHDR} ${msgs["$idx"]}"; done
    say "${_ONHDR}==============================================================================="
    say ""
}

#============================================================================

LOG_FILE=logfile_${TIMESTAMP}.log

declare cfgkey cfgval
ncfg=0

cfgput () { eval cfg_"$1"='$2'; }
cfgget () { eval echo '${cfg_'"$1"'#hash}'; }

cfgload () {
    while read line ; do
        key=$(echo $line | cut -d= -f1)
        val=$(echo $line | cut -d= -f2-)
        cfgput $key "$val"
        ncfg=$((ncfg + 1))
        cfgkey[$ncfg]="$key"
    done < $1
}

cfglist () {
    i=1
    while [ $i -lt $((ncfg + 1)) ]; do
        key=${cfgkey[$i]}
        val=$(cfgget $key)
        printf "%s=%s\n" $key "$val"
        i=$((i + 1))
    done
}

echo $QPFWA
cfgload ${CFG_FILE}

arr=( "Hola" "y despues" "adios" )
greetings "${arr[@]}"

orig_ip=$(cfgget ip)
say "Original IP: $orig_ip"

cfgput ip 127.0.0.1

cfglist


