##############################################################################
# File       : Makefile
# Domain     : QPF.run.dockerfiles
# Version    : 2.0
# Date       : 2018/02/06
# Copyright (C) 2015-2018  J C Gonzalez
#_____________________________________________________________________________
# Purpose    : Generates docker images from Dockerfiles
# Created by : J C Gonzalez
# Status     : Prototype
# History    : See <Changelog>
###############################################################################

SUBDIRS = QDT LE1sim QDTsrv

.PHONY: subdirs clean $(SUBDIRS)

.SILENT:

subdirs: $(SUBDIRS)

recreate:
	-for d in $(SUBDIRS); do ($(MAKE) -C $$d clean all ); done

clean:
	-for d in $(SUBDIRS); do ($(MAKE) -C $$d clean ); done

$(SUBDIRS):
	-cd $@ && bash ./createimgs.sh

all: subdirs

