# Simple utility functions
##############################################################################
# File       : tools.sh
# Domain     : QPF.scripts
# Version    : 2.0
# Date       : 2018/04/02
# Copyright (C) 2015-2018 J C Gonzalez
#_____________________________________________________________________________
# Purpose    : Simple Bash utility functions to use in QPF scripts
# Created by : J C Gonzalez
# Status     : Prototype
# History    : See <Changelog>
###############################################################################

#- Messages
_ONHDR="\x1B[1;49;93m"
_ONMSG="\x1B[1;49;92m"
_ONRUN="\x1B[0;49;32m"
_ONERR="\x1B[1;49;91m"
_OFF="\x1B[0m"
STEP=0

TIMESTAMP=$(date +"%Y%m%d%H%M%S")

say () {
    echo -e "${_ONMSG}$*${_OFF}"
    if [ -f "$LOG_FILE" ]; then echo "$*" >> ${LOG_FILE}; fi
}

sayh () {
    say "${_ONHDR}$*"
}

sec () {
    say ""
    sayh "$*"
    sayh "------------------------------------------------------------------------------"    
}
    
die () {
    say "${_ONERR}ERROR: $*"
    exit 1
}

greetings () {
    local msgs=("$@")
    sayh "==============================================================================="
    for idx in "${!msgs[@]}"; do sayh " ${msgs["$idx"]}"; done
    sayh "==============================================================================="
    say ""
}

