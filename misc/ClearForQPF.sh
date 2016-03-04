#!/bin/bash

## Clear database

cat <<EOF>/tmp/clean-up-qpfdb.sql
delete from products_info where id>0;
delete from transmissions where id>0;
delete from tasks_info where id>0;
EOF

psql -f /tmp/clean-up-qpfdb.sql qpfdb

## Remove old run folders
rm -rf /qpf/run/2015*

## Clear archive and inbox
rm -rf /qpf/data/archive/in/* /qpf/data/archive/out/*
rm -rf /qpf/data/inbox/*
