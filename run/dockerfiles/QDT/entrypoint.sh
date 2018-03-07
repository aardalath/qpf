#!/bin/bash

# Add local user
# Either use the UID if passed in at runtime or fallback to 9001
# Either use the UNAME if passed in at runtime or fallback to lodeen

USER_ID=${UID:-1001}
USER_NAME=${UNAME:-eucops}

echo "Starting with unique ID $USER_ID and username $USER_NAME"
useradd --shell /bin/bash -u $USER_ID -o -c "" -m $USER_NAME
echo "$USER_NAME ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
#echo "autologin-user=$USER_NAME" >> /etc/lightdm/lightdm.conf

export HOME=/home/$USER_NAME
export USER=$USER_NAME

cat <<EOF>> $HOME/.bashrc
alias ll="ls -l"
alias lt="ls -lrt"

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi
EOF

WORKDIR=${WDIR:-$HOME}
cd ${WORKDIR}

exec /usr/bin/gosu $USER_NAME "$@"
