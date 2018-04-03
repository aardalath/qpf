# Simple handy functions for QPF scripts
##############################################################################
# File       : tools.sh
# Domain     : QPF.scripts
# Version    : 2.0
# Date       : 2018/04/02
# Copyright (C) 2015-2018 J C Gonzalez
#_____________________________________________________________________________
# Purpose    : Simple Bash functions to use in QPF scripts
# Created by : J C Gonzalez
# Status     : Prototype
# History    : See <Changelog>
###############################################################################

declare cfgkey cfgval
ncfg=0

cfgput () { eval cfg_"$1"='$2'; }
cfgget () { eval echo '${cfg_'"$1"'#hash}'; }

cfgload () {
    while read line ; do
        hasEqual=$(echo $line | fgrep '=')
        if [ -n "$hasEqual" ]; then
            key=$(echo $line | cut -d= -f1)
            val=$(echo $line | cut -d= -f2-)
            cfgput $key "$val"
            ncfg=$((ncfg + 1))
            cfgkey[$ncfg]="$key"
        fi
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

# Set CFG_FILE to the full file name with the configuration entries,
# and use it like this (example):
#
# cfgload ${CFG_FILE}
# orig_ip=$(cfgget ip)
# echo "Original IP: $orig_ip"
# cfgput ip 127.0.0.1
# cfglist


