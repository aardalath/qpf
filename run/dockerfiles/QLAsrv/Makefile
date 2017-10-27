##############################################################################
# File       : Makefile
# Domain     : QPF.run.dockerfiles.QLAsrv
# Version    : 1.2
# Date       : 2016/12/18
# Copyright (C) 2015-2017  J C Gonzalez
#_____________________________________________________________________________
# Purpose    : Generates docker images from Dockerfiles
# Created by : J C Gonzalez
# Status     : Prototype
# History    : See <Changelog>
###############################################################################

IMAGE_NAME_BASE=qla-srv-debian:base
IMAGE_NAME_BASE_EXT=qla-srv-debian:baseext
IMAGE_NAME=qla-srv-debian:a

WRP=$(HOME)/ws/QPF/qpf-processors/WA/bin/QDT-srv
QDT=$(HOME)/ws/QDT/tags/V1.2_RC6

.PHONY: image baseimage baseimageext complete mrproper clean

.SILENT:

all: image

complete: mrproper image

baseimage:
	docker image inspect $(IMAGE_NAME_BASE) >/dev/null 2>&1 \
	&& echo "Image $(IMAGE_NAME_BASE) already exists. Continuing." \
	|| docker build -t $(IMAGE_NAME_BASE) -f Dockerfile-base .

baseimageext: baseimage
	docker image inspect $(IMAGE_NAME_BASE_EXT) >/dev/null 2>&1 \
	&& echo "Image $(IMAGE_NAME_BASE_EXT) already exists. Continuing." \
	|| docker build -t $(IMAGE_NAME_BASE_EXT) -f Dockerfile-base-ext .

image: baseimageext
	./Build-QDTsrv-image.sh -w $(WRP) -q $(QDT) 
	docker rmi -f $(IMAGE_NAME)  \
	|| echo "Cannot remove image $(IMAGE_NAME). Continuing."
	docker build -t $(IMAGE_NAME) .

clean:
	-echo "Cleanning . . ."

mrproper:
	docker rmi -f $(IMAGE_NAME_BASE)  \
	|| echo "Cannot remove image $(IMAGE_NAME_BASE). Continuing."
	docker rmi -f $(IMAGE_NAME_NAME_EXT)  \
	|| echo "Cannot remove image $(IMAGE_NAME_BASE_EXT). Continuing."
	docker rmi -f $(IMAGE_NAME)  \
	|| echo "Cannot remove image $(IMAGE_NAME). Continuing."
