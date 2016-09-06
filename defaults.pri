#===== Projec dir. =======
PRJ_ROOT_DIR = $$PWD 

BUILDROOT = build

#===== External libraries =======
COTSDIR = /opt/cots

exists(/home/eucops) {
ZMQDIR = $$(HOME)/opt/zmq
CURLDIR = $$(HOME)/opt/curl
GTESTDIR = $$(HOME)/ws/jcgg/cots/gtest-1.7.0
PSQLDIR = $$(HOME)/opt/pgsql
PCRE2DIR = $$(HOME)/opt/pcre2
}

exists(/home/jcgonzalez) {
ZMQDIR = $$COTSDIR
CURLDIR = $$COTSDIR
GTESTDIR = $$(HOME)/ws/cots/gtest-1.7.0
PSQLDIR = /opt/pgsql
PCRE2DIR = $$COTSDIR
}

PSQLLIB = pq

#===== Project sections/libraries =======
LIBCOMMPATH = $$PRJ_ROOT_DIR
LIBCOMMLIB = $$LIBCOMMPATH/$$BUILDROOT/libcomm
LIBCOMMINC = $$LIBCOMMPATH/libcomm

JSONCPPPATH = $$PRJ_ROOT_DIR
JSONCPPLIB = $$JSONCPPPATH/$$BUILDROOT/json
JSONCPPINC = $$JSONCPPPATH/json

INFIXPATH = $$PRJ_ROOT_DIR
INFIXLIB = $$INFIXPATH/$$BUILDROOT/infix
INFIXINC = $$INFIXPATH/infix

SDCPATH = $$PRJ_ROOT_DIR
SDCLIB = $$SDCPATH/$$BUILDROOT/sdc
SDCINC = $$SDCPATH/sdc

#===== Compilation/Linkage variables =====
QMAKE_CXXFLAGS += -g3 -std=c++11 -Wall -Wextra -D_GLIBCXX_USE_CXX11_ABI=0
LIBS += -lpcre2-8

INCLUDEPATH += . $$PRJ_ROOT_DIR/src
INCLUDEPATH += $$LIBCOMMINC $$JSONCPPINC $$INFIXINC $$SDCINC
INCLUDEPATH += $$ZMQDIR/include $$CURLDIR/include $$PCRE2DIR/include $$PSQLDIR/include
INCLUDEPATH += $$(HOME)/opt 

OLD_LIBDIR = $$QMAKE_LIBDIR
QMAKE_LIBDIR += $$LIBCOMMLIB $$JSONCPPLIB $$INFIXLIB $$SDCLIB
QMAKE_LIBDIR += $$ZMQDIR/lib $$CURLDIR/lib $$PCRE2DIR/lib $$PSQLDIR/lib
QMAKE_LIBDIR += /usr/lib64 /lib64 $$OLD_LIBDIR
