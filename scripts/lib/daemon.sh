# Simple functions to handle a daemonized application from a script
##############################################################################
# File       : daemon.sh
# Domain     : QPF.scripts
# Version    : 2.0
# Date       : 2018/04/02
# Copyright (C) 2015-2018 J C Gonzalez
#_____________________________________________________________________________
# Purpose    : Simple functions to handle a daemonized application from a script
# Created by : J C Gonzalez
# Status     : Prototype
# History    : See <Changelog>
###############################################################################

# Log maxsize in KB
logMaxSize=204800 # 1048576

_ONERR="\x1B[1;49;91m"
_OFF="\x1B[0m"

setupDaemon() {
    local srv=$1
    local pidDir=$2
    local logFile="${pidDir}/${srv}.log"
    # Make sure that the directories work.
    [ ! -d "$pidDir" ] && mkdir -p "$pidDir"
    if [ ! -f "$logFile" ]; then
        touch "$logFile"
    else
        checkForLogRotation "$logFile"
    fi
}

checkForLogRotation() {
    local log=$1
    # Check to see if we need to rotate the logs.
    size=$(ls --block-size=1k -s "$log"|cut -d' ' -f1)
    if [ $size -gt $logMaxSize ]; then
        log "Rotating logs"
        now=$(date +"%Y%m%d%H%M%S")
        gzip -c "$log" > "$log.$now.gz" && cat /dev/null > "$log"
    fi
}

startDaemon() {
    local srv=$1
    local pidDir=$2
    local daemonName=$3
    local cmd=$4
    pidFile="${pidDir}/${srv}.pid"
    logFile="${pidDir}/${srv}.log"
    # Start the daemon.
    setupDaemon "$srv" "$pidDir"
    checkDaemon "$srv" "$pidDir"
    if [[ $? -eq 1 ]]; then
        echo -e "${_ONERR}Error: $daemonName is already running.${_OFF}"
        exit 1
    fi
    log '*** '`date +"%Y-%m-%d"`": Starting up $daemonName."
    $cmd 1>>${logFile} 2>&1 & srvPid=$!
    echo "Starting $daemonName with PID: $srvPid."
    echo "$srvPid" > "$pidFile"
}

stopDaemon() {
    local srv=$1
    local pidDir=$2
    local daemonName=$3
    pidFile="${pidDir}/${srv}.pid"
    logFile="${pidDir}/${srv}.log"
    # Stop the daemon.
    checkDaemon "$srv" "$pidDir"
    if [[ $? -eq 0 ]]; then
        echo -e "${_ONERR}Error: $daemonName is not running.${_OFF}"
        exit 1
    fi
    echo "Stopping $daemonName . . ."
    
    local p=$(cat $pidFile 2>/dev/null)
    if [ ! -z "$p" ]; then
        local gp=$(cat /proc/$p/stat|cut -d' ' -f5)
        if kill -0 $p >/dev/null 2>&1 ; then
            kill -- -$gp ; sleep 2
            if kill -0 $p >/dev/null 2>&1 ; then
                kill -9 -- -$gp ; sleep 2
                if kill -0 $p >/dev/null 2>&1 ; then
                    log "Exec: sudo kill -9 $p"
                    sudo kill -9 -- -$gp ; sleep 2
                fi
            fi
        fi
    fi
    log '*** '`date +"%Y-%m-%d"`": $daemonName stopped."
    rm -f ${pidFile}
}

restartDaemon() {
    local srv=$1
    local pidDir=$2
    local daemonName=$3
    local cmd=$4
    pidFile="${pidDir}/${srv}.pid"
    logFile="${pidDir}/${srv}.log"
    # Restart the daemon.
    checkDaemon "$srv" "$pidDir"
    if [[ $? -eq 0 ]]; then
        # Can't restart it if it isn't running.
        echo "$daemonName isn't running."
        exit 1
    fi
    stopDaemon "$srv" "$pidDir" "$daemonName"
    startDaemon "$srv" "$pidDir" "$daemonName" "$cmd"
}

statusDaemon() {
    local srv=$1
    local pidDir=$2
    local daemonName=$3
    pidFile="${pidDir}/${srv}.pid"
    logFile="${pidDir}/${srv}.log"
    # Query and return whether the daemon is running.
    checkDaemon "$srv" "$pidDir"
    if [[ $? -eq 1 ]]; then
        echo "$daemonName is running."
    else
        echo "$daemonName isn't running."
    fi
    checkForLogRotation "$logFile"
    exit 0
}

checkDaemon() {
    local srv=$1
    local pidDir=$2
    pidFile="${pidDir}/${srv}.pid"
    logFile="${pidDir}/${srv}.log"
    local p=$(cat "$pidFile" 2>/dev/null)
    if [ ! -z "$p" ]; then
        if kill -0 $p >/dev/null 2>&1 ; then
            log "Daemon $srv is runnig with PID $p"
            return 1
        else
            log "Daemon $srv is not running (process PID $p not exists)"
            rm -f ${pidFile}
            return 0
        fi
    else
        log "Daemon $srv is not running"
        return 0
    fi
}

checkIsValidAction() {
    local act=$1
    local validActions="_start_stop_restart_status_"
    local ok=$(echo "$validActions" | grep "_${act}_")
    [ "$ok" = "$validActions" ]; return $?
}

log() {
  # Generic log function.
  echo "$1" >> "$logFile"
}

