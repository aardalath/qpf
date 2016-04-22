#!/bin/bash

#---- Basic variables -------------------------------------------------

COTSDIR=${HOME}/opt

QTVER=5.4
QTDIR=${COTSDIR}/Qt/${QTVER}/gcc_64
QTCREATORDIR=${COTSDIR}/Qt/Tools/QtCreator

SODIUMDIR=${HOME}/opt/libsodium
ZMQDIR=${HOME}/opt/zmq
CURLDIR=${HOME}/opt/curl
PCRE2DIR=${HOME}/opt/pcre2
GTESTDIR=${HOME}/ws/jcgg/cots/gtest-1.7.0
PGSQLDIR=${HOME}/opt/pgsql

WORKAREA=${HOME}/qpf

PARENT=${HOME}/ws/jcgg
QPFWSDIR=${PARENT}/QPF
BUILDDIR=build
QPFBUILD=${QPFWSDIR}/${BUILDDIR}

#---- PATH ------------------------------------------------------------

pth=""

pth="$pth:${COTSDIR}/bin"
pth="$pth:${QTDIR}/bin"
pth="$pth:${QTCREATORDIR}/bin"
pth="$pth:${SODIUMDIR}/bin"
pth="$pth:${ZMQDIR}/bin"
pth="$pth:${CURLDIR}/bin"
pth="$pth:${PCRE2DIR}/bin"
pth="$pth:${PGSQLDIR}/bin"
pth="$pth:$HOME/bin"
pth="$pth:$HOME/local/bin"

pth="$pth:${QPFBUILD}/qpf"
pth="$pth:${QPFBUILD}/qpfhmi"

export PATH="$pth:${PATH}"

#---- LD_LIBRARY_PATH--------------------------------------------------

lpth=""
lpth="$lpth:${HOME}/local/lib"

lpth="$lpth:${COTSDIR}/bin"
lpth="$lpth:${QTDIR}/lib"
lpth="$lpth:${SODIUMDIR}/lib"
lpth="$lpth:${ZMQDIR}/lib"
lpth="$lpth:${CURLDIR}/lib"
lpth="$lpth:${PCRE2DIR}/lib"
lpth="$lpth:${PGSQLDIR}/lib"

lpth="$lpth:${QPFBUILD}/libcomm"
lpth="$lpth:${QPFBUILD}/infix"
lpth="$lpth:${QPFBUILD}/json"
lpth="$lpth:${QPFBUILD}/sdc"
lpth="$lpth:${QPFBUILD}/src"

export LD_LIBRARY_PATH="$lpth:${LD_LIBRARY_PATH}"

#---- PKG_CONFIG_PATH -------------------------------------------------

pkgpth=""

pkgpth="$pkgpth:${QTDIR}/lib/pkgconfig"
pkgpth="$pkgpth:${SODIUMDIR}/lib/pkgconfig"
pkgpth="$pkgpth:${ZMQDIR}/lib/pkgconfig"
pkgpth="$pkgpth:${CURLDIR}/lib/pkgconfig"
pkgpth="$pkgpth:${PCRE2DIR}/lib/pkgconfig"
pkgpth="$pkgpth:${PGSQLDIR}/lib/pkgconfig"

export PKG_CONFIG_PATH="$pkgpth:${PKG_CONFIG_PATH}"

#---- User specific variables -----------------------------------------

export QPFHMI=${QPFBUILD}/qpfhmi/qpfhmi
export QPFDIR=${WORKAREA}
