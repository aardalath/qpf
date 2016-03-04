#!/bin/bash

pth="$pth:/opt/Qt/5.4/gcc_64/bin"
pth="$pth:/opt/Qt/Tools/QtCreator/bin"
pth="$pth:/opt/cots/bin"
pth="$pth:/opt/anaconda/bin"
pth="$pth:$HOME/bin"
pth="$pth:$HOME/local/bin"

pth="$pth:/home/jcgonzalez/ws/personal/QPF/build/debug/qpf"
pth="$pth:/home/jcgonzalez/ws/personal/QPF/build/debug/qpfhmi"

lpth="$lpth:$HOME/local/lib"
lpth="$lpth:/opt/Qt/5.4/gcc_64/lib"
lpth="$lpth:/opt/cots/lib"

lpth="$lpth:/home/jcgonzalez/ws/personal/libcomm/build/debug/src"
lpth="$lpth:/home/jcgonzalez/ws/personal/QPF/build/debug/src"
lpth="$lpth:/home/jcgonzalez/ws/personal/QPF/build/debug/json"
lpth="$lpth:/home/jcgonzalez/ws/personal/QPF/build/debug/src"

export PATH="$pth:$PATH"
export LD_LIBRARY_PATH="$lpth:$LD_LIBRARY_PATH"
export PKG_CONFIG_PATH="/opt/cots/lib/pkgconfig"

export QPFHMI=/home/jcgonzalez/ws/personal/QPF/build/debug/qpfhmi/qpfhmi
