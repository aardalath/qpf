##############################################################################
# File       : Makefile
# Domain     : QPF.run.dockerfiles.LE1sim
# Version    : 1.2
# Date       : 2016/12/18
# Copyright (C) 2015,2016,2017  J C Gonzalez
#_____________________________________________________________________________
# Purpose    : Generates docker images from Dockerfiles
# Created by : J C Gonzalez
# Status     : Prototype
# History    : See <Changelog>
###############################################################################

IMAGE_NAME_BASE=le1-sim-debian:base
IMAGE_NAME=le1-sim-debian:a

.PHONY: image clean

.SILENT:

baseimage:
	docker rmi -f $(IMAGE_NAME_BASE) \
		|| echo "Cannot remove $(IAMGE_NAME_BASE). Continuing."
	docker build -t $(IMAGE_NAME_BASE) -f Dockerfile-base .
	cat /dev/null > baseimage

image: baseimage
	docker rmi -f $(IMAGE_NAME)  \
		|| echo "Cannot remove image $(IMAGE_NAME). Continuing."
	docker build -t $(IMAGE_NAME) .

clean:
	-echo "Cleanning . . ."
	-rm -f baseimage

all: image

