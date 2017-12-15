// -*- C++ -*-

#ifndef CHANNELS_H
#define CHANNELS_H

#include "nncomm.h"

#define CHNLS_IF_VERSION  "1.0"

#undef T

#define TLISTOF_TX_IDS                          \
        T(CMD),                                 \
        T(EVTMNG),                              \
        T(HMICMD),                              \
        T(INDATA),                              \
        T(TSKSCHED),                            \
        T(TSKREG),                              \
        T(TSKRQST),                             \
        T(TSKPROC),                             \
        T(TSKREP),                              \
        T(FMKMON),                              \
        T(HOSTMON),                             \
        T(UNKNOWN)

#define T(x) TX_ID_ ## x
enum TxId { TLISTOF_TX_IDS };
#undef T

#define T(x) std::string( #x )
const ChannelDescriptor ChannelAcronym[] = { TLISTOF_TX_IDS };
#undef T

const ChannelDescriptor ChnlCmd        (ChannelAcronym[TX_ID_CMD]);
const ChannelDescriptor ChnlEvtMng     (ChannelAcronym[TX_ID_EVTMNG]);
const ChannelDescriptor ChnlHMICmd     (ChannelAcronym[TX_ID_HMICMD]);
const ChannelDescriptor ChnlInData     (ChannelAcronym[TX_ID_INDATA]);
const ChannelDescriptor ChnlTskSched   (ChannelAcronym[TX_ID_TSKSCHED]);
const ChannelDescriptor ChnlTskProc    (ChannelAcronym[TX_ID_TSKPROC]);
//const ChannelDescriptor ChnlTskRqst    (ChannelAcronym[TX_ID_TSKRQST]);
//const ChannelDescriptor ChnlTskRep     (ChannelAcronym[TX_ID_TSKREP]);
const ChannelDescriptor ChnlTskReg     (ChannelAcronym[TX_ID_TSKREG]);
const ChannelDescriptor ChnlFmkMon     (ChannelAcronym[TX_ID_FMKMON]);

enum Message_Tag {
    Tag_ChnlCmd      = 0b0000000000000001,
    Tag_ChnlEvtMng   = 0b0000000000000010,
    Tag_ChnlHMICmd   = 0b0000000000000100,
    Tag_ChnlInData   = 0b0000000000001000,
    Tag_ChnlTskSched = 0b0000000000010000,
    Tag_ChnlTskReg   = 0b0000000000100000,
    Tag_MsgTskRqst   = 0b0000000001000000,
    Tag_MsgTskProc   = 0b0000000010000000,
    Tag_MsgTskRep    = 0b0000000100000000,
    Tag_ChnlFmkMon   = 0b0000001000000000,
    Tag_MsgHostMon   = 0b0000010000000000,
    Tag_UNKNOWN      = 0b1000000000000000,
};    

#endif
