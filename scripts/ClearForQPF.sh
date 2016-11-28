#! /bin/bash

## Clear database

cat <<EOF>/tmp/clean-up-qpfdb.sql
delete from products_info where id>0;
delete from transmissions where id>0;
delete from tasks_info where id>0;
EOF

psql -f /tmp/clean-up-qpfdb.sql qpfdb

## Remove old run folders
rm -rf $HOME/qpf/run/201*

## Clear archive and inbox
rm -rf $HOME/qpf/data/archive/in/* $HOME/qpf/data/archive/out/*
rm -rf $HOME/qpf/data/gateway/in/* $HOME/qpf/data/gateway/out/*
rm -rf $HOME/qpf/data/inbox/*
