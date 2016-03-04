#===== Projec dir. =======
PRJ_ROOT_DIR = $$PWD

BUILDROOT = build/debug

#===== External libraries =======
COTSDIR = /opt/cots

exists(/home/eucops) {
ZMQDIR = $$(HOME)/opt/zmq
CURLDIR = $$(HOME)/opt/curl
UUIDDIR = $$(HOME)/opt/uuid
GTESTDIR = $$(HOME)/ws/jcgg/cots/gtest-1.7.0
PSQLDIR = $$(HOME)/opt/pgsql
}

exists(/home/jcgonzalez) {
ZMQDIR = $$COTSDIR
CURLDIR = $$COTSDIR
UUIDDIR = $$COTSDIR
GTESTDIR = $$(HOME)/ws/cots/gtest-1.7.0
PSQLDIR = /opt/pgsql
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

INCLUDEPATH += . $$PRJ_ROOT_DIR/src
INCLUDEPATH += $$LIBCOMMINC $$JSONCPPINC $$INFIXINC $$SDCINC
INCLUDEPATH += $$ZMQDIR/include $$CURLDIR/include $$UUIDDIR/include $$PSQLDIR/include

OLD_LIBDIR = $$QMAKE_LIBDIR
QMAKE_LIBDIR += $$LIBCOMMLIB $$JSONCPPLIB $$INFIXLIB $$SDCLIB
QMAKE_LIBDIR += $$ZMQDIR/lib $$CURLDIR/lib $$UUIDDIR/lib $$PSQLDIR/lib 
QMAKE_LIBDIR += /usr/lib64 $$OLD_LIBDIR



