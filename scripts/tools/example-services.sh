#!/usr/bin/env bash

export PID_FILE_PATH="/tmp/my-service.pid"
export LOG_FILE_PATH="/tmp/my-service.log"
export LOG_ERROR_FILE_PATH="/tmp/my-service.error.log"

. ./services.sh

run-script() {
  local action="$1" # Action

  while true; do
    echo "@@@ Running action '${action}'"
    echo foo
    echo bar >&2

    [ "$action" = "run" ] && return 0
    sleep 5
    [ "$action" = "debug" ] && exit 25
  done
}

before-start() {
  local action="$1" # Action

  echo "* Starting with $action"
}

after-finish() {
  local action="$1" # Action
  local serviceExitCode=$2 # Service exit code

  echo "* Finish with $action. Exit code: $serviceExitCode"
}

action="$1"
serviceName="Example Service"

serviceMenu "$action" "$serviceName" run-script "$workDir" before-start after-finish

