/******************************************************************************
 * File:    r2rpeer.cpp
 *          This file is part of the LibComm communications library
 *
 * Domain:  LibComm.LibComm.Router2RouterPeer
 *
 * Version:  1.2
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015,2016,2017 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implement Router2RouterPeer class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   none
 *
 * Files read / modified:
 *   none
 *
 * History:
 *   See <ChangeLog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#include "r2rpeer.h"
#include "common.h"
#include "tools.h"

#include <cerrno>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <mutex>

////////////////////////////////////////////////////////////////////////////
// Namespace: LibComm
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace LibComm {

/* #define EXTENSIVE_DEBUG */

#ifdef EXTENSIVE_DEBUG
std::mutex m_io;
void dumpDebugInfo(const char* tag, const char* msg)
{
    m_io.lock();
    pid_t id = getpid();
    std::cerr << '[' << id << ':' << tag << ']';
    if (msg) std::cerr << " '" << msg << "'";
    std::cerr << std::endl << std::flush;
    m_io.unlock();
}

#define dumpId(t, m)  if (generateDebugInfo) { dumpDebugInfo(t, m); }
#define GETMSG(s,m) do {\
    std::stringstream sdump; \
    for (unsigned int _ = 0; _ < m.at(2).length(); ++_) { \
        char __ = m.at(2).at(_); \
        if ((__ > 31) && (__ < 127)) { sdump << ' ' << __ << ' '; } \
        else { sdump << std::internal << std::setfill('0') << std::hex \
                     << std::uppercase << std::setw(2) << (int)(__) << ' '; } \
    } \
    std::string ss( "{ PEER: " + m.at(0) + ", TYPE: " + m.at(1) + \
                    ", CONTENT: [" + m.at(2) + "] }"); \
    s = ss; \
  } while (0)

/*       ", CONTENT: '" + sdump.str() + "' }"; \ */
#else
#  define dumpId(t,m)
#  define GETMSG(s,m)
#endif

#define DESTROY_MESSAGE( x )  { delete x; x = 0; }

std::atomic<bool> Router2RouterPeer::generateDebugInfo(true);

//----------------------------------------------------------------------
// Constructor: Router2RouterPeer
//----------------------------------------------------------------------
Router2RouterPeer::Router2RouterPeer() :
    iterations(50),
    communicationsActive(true),
    waitForStart(false),
    startReceived(false)
{
    peers.clear();
}

//----------------------------------------------------------------------
// Destructor: ~Router2RouterPeer
// Virtual destructor
//----------------------------------------------------------------------
Router2RouterPeer::~Router2RouterPeer()
{
    r2rpeerThread.join();
}

//----------------------------------------------------------------------
// Method: addPeer
// Adds a new peer to the set of peers to form the network
//----------------------------------------------------------------------
void Router2RouterPeer::addPeer(Peer * peer, bool isThisPeer)
{
    mapOfPeers[peer->name] = peers.size();
    int numOfPeers = peers.size();
    peers.push_back(peer);
    ackInfo.waitAckFrom[peer->name] = false;
    ackInfo.cycles[peer->name] = 0;
    if (isThisPeer) {
        self.store(peer, std::memory_order_relaxed);
        selfIdx.store(numOfPeers, std::memory_order_relaxed);
        char s[100];
        sprintf(s, "%s index is %d", peer->name.c_str(), numOfPeers);
        dumpId(selfPeer()->name.c_str(), s);
    }
}

//----------------------------------------------------------------------
// Method: selfPeer
// Return self peer
//----------------------------------------------------------------------
Router2RouterPeer::Peer * Router2RouterPeer::selfPeer()
{
    return self;
}

//----------------------------------------------------------------------
// Method: isPeer
// Return TRUE is the named peer is among the peers of this component
//----------------------------------------------------------------------
bool Router2RouterPeer::isPeer(PeerName aPeer)
{
    for (unsigned int i = 0; i < peers.size(); ++i) {
        if (peers.at(i)->name == aPeer) { return true; }
    }
    return false;
}

//----------------------------------------------------------------------
// Method: broadcast
// Adds a new message to be sent to all peers
//----------------------------------------------------------------------
void Router2RouterPeer::broadcast(PeerMessage * peerMsg)
{
    std::string thisPeerName = self.load(std::memory_order_relaxed)->name;
    int numOfPeers = peers.size();

    for (int i = 0; i < numOfPeers; ++i) {
        Peer * peer = peers.at(i);
        std::string & name = peer->name;

        dumpId(selfPeer()->name.c_str(),
               ("Broadcast: *** Peer name is " + name +
                std::string(", and self name is ") + thisPeerName).c_str());
        if (name == thisPeerName) continue;
        dumpId(selfPeer()->name.c_str(),
               ("broadcast to " + name).c_str());
        PeerMessage * pmsg = new PeerMessage(* peerMsg);
        (*pmsg)[0] = name;
        setTransmissionToPeer(peer, pmsg);
    }
    DESTROY_MESSAGE( peerMsg );
}

//----------------------------------------------------------------------
// Method: setTransmissionToPeer
// Adds a new message to be sent to a given peer
//----------------------------------------------------------------------
void Router2RouterPeer::setTransmissionToPeer(PeerName name, PeerMessage * peerMsg, bool ack)
{
    Peer * peer = 0;
    std::map<PeerName, int>::iterator it = mapOfPeers.find(name);
    if (it != mapOfPeers.end()) {
        peer = peers.at(it->second);
    }
    setTransmissionToPeer(peer, peerMsg, ack);
}

//----------------------------------------------------------------------
// Method: setTransmissionToPeer
// Adds a new message to be sent to a given peer
//----------------------------------------------------------------------
void Router2RouterPeer::setTransmissionToPeer(Peer * peer, PeerMessage * peerMsg, bool ack)
{
    std::string thisPeerName = self.load(std::memory_order_relaxed)->name;

    Frame msgType = peerMsg->type();

    // In case of requested ack, append info to recipient frame
    if (msgType == "START") { ack = false; }
    if (ack) { (*peerMsg)[FRAME_MSG_TYPE].append(":ack"); }

    int io = 0;
    if (peer->name != thisPeerName) {
        // Normal case, transmitting to another peer
        outTx.push_back( std::make_pair(peer, peerMsg) );
    } else {
        // Degenerate case, transmitting to itsef
        // the message is automatically included in inbox
        inTx.push_back( std::make_pair(peer, peerMsg) );
        io = 1;
    }

    msgStats.push_back(MsgStats(peerMsg->peer(), msgType, io,
                                peerMsg->content().size()));
    if (msgStats.size() >= 100) {
        dumpMsgStats("/tmp/" + selfPeer()->name + ".stats");
        msgStats.clear();
    }
}

//----------------------------------------------------------------------
// Method: getNewTransmission
// Returns a new message from the queue of received messages
//----------------------------------------------------------------------
void Router2RouterPeer::getNewTransmission(Peer & peer, PeerMessage & peerMsg)
{
    peerMsg.clear();
    try {
        Transmission tx = inTx.front();
        peer = *(tx.first);
        peerMsg = *(tx.second);
        inTx.pop_front();
    } catch(...) {
        peer.name = "";
        std::cerr << "BOOM!" << std::flush;
        abort();
        throw error_t();
    }
}

//----------------------------------------------------------------------
// Method: thereArePendingTransmissions
// Returns true if there are pending incoming transm. to be processed
//----------------------------------------------------------------------
bool Router2RouterPeer::thereArePendingTransmissions()
{
    int numOfPendingTransmissions = inTx.size();
    return (numOfPendingTransmissions > 0);
}

//----------------------------------------------------------------------
// Method: storeAtBuffer
//----------------------------------------------------------------------
void Router2RouterPeer::storeAtBuffer(Frame & frm, BytePtr & buffer)
{
    int bufLength = frm.length();
    buffer = new Byte[bufLength + 1];
    std::copy(frm.begin(), frm.end(), buffer);
    buffer[bufLength] = 0;
}

//----------------------------------------------------------------------
// Method: buildPeerMsg
//----------------------------------------------------------------------
Router2RouterPeer::PeerMessage *
Router2RouterPeer::buildPeerMsg(std::string to,
                                std::string msg,
                                std::string type)
{
    PeerMessage * msgFrames(new PeerMessage);
    msgFrames->clear();
    msgFrames->push_back(to);
    msgFrames->push_back(type);
    msgFrames->push_back(msg);
    return msgFrames;
}

//----------------------------------------------------------------------
// Method: dump
//----------------------------------------------------------------------
void Router2RouterPeer::dump()
{
}

//----------------------------------------------------------------------
// Method: deactivateCommunications
//----------------------------------------------------------------------
void Router2RouterPeer::deactivateCommunications()
{
    communicationsActive = false;
}

//----------------------------------------------------------------------
// Method: waitForStartSignal
//----------------------------------------------------------------------
void Router2RouterPeer::waitForStartSignal()
{
    waitForStart = true;
}

//----------------------------------------------------------------------
// Method: emitStartSignal
//----------------------------------------------------------------------
void Router2RouterPeer::emitStartSignal()
{
    startReceived = true;
    waitForStart = false;
}

//----------------------------------------------------------------------
// Method: startSignalReceived
//----------------------------------------------------------------------
bool Router2RouterPeer::startSignalReceived()
{
    bool exitFromLoop;
    bool received;
    int iter;
    do {
        received = startReceived;
        iter = iterations;
        exitFromLoop = received || (iter < 1);
        usleep(10000);
    } while (!exitFromLoop);
    return received;
}

//----------------------------------------------------------------------
// Method: establishCommunications
// Creates the appropriate 0MQ sockets for the communication
//----------------------------------------------------------------------
void Router2RouterPeer::establishCommunications()
{
    dumpId(selfPeer()->name.c_str(), "Peers List:");

    int numOfPeers = peers.size();
    std::string thisPeerName = self.load(std::memory_order_relaxed)->name;

    for (int i = 0; i < numOfPeers; ++i) {
        std::string name = peers.at(i)->name;
        dumpId(selfPeer()->name.c_str(),
               (std::string((name == thisPeerName) ? " ** > " : "    > ") + name).c_str());
    }

    r2rpeerThread = std::thread(&Router2RouterPeer::transmissionsHandler, this);
}

//----------------------------------------------------------------------
// Method: launchTransmissionsHandler
// Handler for all incoming and outgoing transmissions
//----------------------------------------------------------------------
void * Router2RouterPeer::launchTransmissionsHandler(void * obj)
{
    Router2RouterPeer * o = (Router2RouterPeer*)(obj);
    o->transmissionsHandler();
    return 0;
}

//----------------------------------------------------------------------
// Method: transmissionsHandler
// Handler for all incoming and outgoing transmissions
//----------------------------------------------------------------------
void Router2RouterPeer::transmissionsHandler()
{
    std::string id = self.load(std::memory_order_relaxed)->name;
    int selfNum = selfIdx;

    bool waitStart = waitForStart;

    //============================================================
    //==== 1. Create context
    //============================================================

    // Create context
    zmq::context_t context(1);

    dumpId(selfPeer()->name.c_str(), "++++++++++++++++++++++++++++++++++++++++++++++++");
    dumpId(selfPeer()->name.c_str(), ("+ RUNING TRANSMISSIONS HANDLER FOR " + id).c_str());
    dumpId(selfPeer()->name.c_str(), "++++++++++++++++++++++++++++++++++++++++++++++++");

    //============================================================
    //==== 2. Create and connect sockets
    //============================================================
    dumpId(selfPeer()->name.c_str(), ("Identity is " + id).c_str());

    std::string serverAddr = self.load(std::memory_order_relaxed)->serverAddr;

    // Create and Activate server socket
    int routerMand = 1;
    dumpId(selfPeer()->name.c_str(), (id + ":" + serverAddr).c_str());
    zmq::socket_t skServer(context, ZMQ_ROUTER);
    zmq_setsockopt(skServer, ZMQ_IDENTITY, id.c_str(), id.length());
    zmq_setsockopt(skServer, ZMQ_ROUTER_MANDATORY, &routerMand, sizeof(routerMand));

    // Activate server
    skServer.bind(serverAddr.c_str());

    // Create and Activate client socket
    dumpId(selfPeer()->name.c_str(), ("client router: " + id).c_str());
    zmq::socket_t skClient(context, ZMQ_ROUTER);
    zmq_setsockopt(skClient, ZMQ_IDENTITY, id.c_str(), id.length());

    // Connect client socket to peers
    for (unsigned int i = 0; i < peers.size(); ++i) {
        if ((int)(i) == selfNum) continue;
        dumpId(selfPeer()->name.c_str(),
               ("connecting to" + peers.at(i)->name +
                "@" + peers.at(i)->clientAddr).c_str());
        skClient.connect(peers.at(i)->clientAddr.c_str());
    }


    //============================================================
    // 3. Loop
    //============================================================
    bool commsActive = communicationsActive;

    outTx.clear();
    inTx.clear();

    while (commsActive) {

        //----------------------------------------------
        // 3.1 Send outgoing transmissions to their recipients
        //----------------------------------------------
        if (!skClient.connected()) {
            std::cerr << "ERROR: Client socket not connected for "
                      << self.load()->name << std::endl;
        }

        // Send as much as we can during a window of
        int64_t windowSpan = 200000; // 200 us
        int64_t timeSpan = 0;
        std::deque<Transmission>::iterator outIt = outTx.begin();

        setClock(0);

        while ((!outTx.empty()) && (outIt != outTx.end()) &&
               (timeSpan < windowSpan)) {

            Transmission tx = *outIt;
            PeerMessage * peerMsg = tx.second;
            if (peerMsg == 0) {
                outTx.pop_front(); // outTx.erase(outIt);
                outIt = outTx.begin();
                timeSpan = setClock(1);
                continue;
            }

            PeerName recipient;
            try {
                recipient = peerMsg->peer();
            } catch(...) {
                std::cerr << peerMsg << std::endl;
                for (int i = 0; i < peerMsg->size(); ++i)
                    std::cerr << i << ": " << peerMsg->at(i) << std::endl;
            }

            // In case we are still waiting for an ACK from this recipient,
            // do not send more messages
            bool ackFromRecip = ackInfo.waitAckFrom[recipient];
            if (ackFromRecip) {
                ackInfo.cycles[recipient]++;
                dumpId(selfPeer()->name.c_str(),
                       ("Waiting for ACK from " + recipient +
                        " cycle " + toStr<int>(ackInfo.cycles[recipient])).c_str());
                if (ackInfo.cycles[recipient] > 20) {
                    tx = ackInfo.lastTx[recipient];
                    peerMsg = tx.second;
                    ackInfo.cycles[recipient] = 0;
                } else {
                    ++outIt;
                    timeSpan = setClock(1);
                    continue;
                }
            }

            for (unsigned int j = 0; j < peerMsg->size(); ++j) {
                dumpId(selfPeer()->name.c_str(),
                       ("Sending FRAME '" + peerMsg->at(j) + "'").c_str());
            }

            int flags = ZMQ_SNDMORE;
            int bytesSent = 0;
            int numFrames = peerMsg->size();
            for (int i = 0; i < numFrames; ++i) {
                if (i == (numFrames - 1)) { flags = 0; }
                bytesSent += skClient.send(peerMsg->at(i).c_str(),
                                           peerMsg->at(i).length(),
                                           flags);
            }
            std::string mc;
            GETMSG(mc, (*peerMsg));
            char sss[100];
            sprintf(sss, " (%d bytes sent)", bytesSent);
            dumpId(selfPeer()->name.c_str(),
                   ("Sent tx => " + mc + std::string(sss) + " to " +
                    peerMsg->peer()).c_str());

            // Mark ack from recipient if needed
            ackInfo.waitAckFrom[recipient] = peerMsg->ackRqsted();
            ackInfo.cycles[recipient] = 0;
            ackInfo.lastTx[recipient] = tx;

            if (!ackInfo.waitAckFrom[recipient] ) {
                DESTROY_MESSAGE( peerMsg );
            }

            if (outIt != outTx.begin()) {
                std::deque<Transmission>::iterator outItPrev = outIt - 1;
                outTx.erase(outIt);
                outIt = outItPrev + 1;
            } else {
                outTx.erase(outIt);
                outIt = outTx.begin();
            }

            timeSpan = setClock(1);
        }

        //----------------------------------------------
        // 3.2 Retrieves incoming transmissions from clients
        //----------------------------------------------
        zmq::pollitem_t servers[] = { {skServer, 0, ZMQ_POLLIN, 0} };
        int rc = 0;

        if (skServer.connected()) {
            // Check now incoming messages
            try {
                rc = zmq::poll(servers, 1, 100);
            } catch(...) {
                if ((rc < 0) && (zmq_errno() != EINTR)) {
                    perror("zmq_poll");
                }
            }
        } else {
            std::cerr << "ERROR: Server socket not connected for "
                      << self.load()->name << std::endl;
        }

        if (rc > 0) { processIncommingPeerMsg(servers[0], waitStart, skServer); }

        commsActive = communicationsActive;

    } // while communicationsActive

    //============================================================
    // 5. Shutdown connections
    //============================================================
    skClient.close();
    skServer.close();
    context.close();
}

//----------------------------------------------------------------------
// Method: processIncommingPeerMsg
// Process incomming transmission
//----------------------------------------------------------------------
void Router2RouterPeer::processIncommingPeerMsg(zmq::pollitem_t & server,
                                                bool & waitStart,
                                                zmq::socket_t & skServer)
{
    size_t type_size = sizeof(int64_t);
    int64_t rcvmore = 0;
    PeerMessage * frames = new PeerMessage;
    frames->clear();
    Frame frm;
    if (server.revents & ZMQ_POLLIN) {
        do {
            zmq::message_t message;
            if (!skServer.recv(&message, 0)) throw error_t();
            frm.assign(static_cast<const char*>(message.data()),
                       message.size());
            frames->push_back(frm);
            rcvmore = 0;
            skServer.getsockopt(ZMQ_RCVMORE, &rcvmore, &type_size);
        } while (rcvmore);
        server.revents = 0;

        if (waitStart) {
            if (frames->size() > FRAME_MSG_TYPE) {
                if (frames->at(FRAME_MSG_TYPE).compare("START") == 0) {
                    emitStartSignal();
                    waitStart = false;
                }
            }
        } else {
            const PeerName & mpeer = frames->peer();
            dumpId(selfPeer()->name.c_str(),
                   (selfPeer()->name + ": mpeer = " + mpeer).c_str());
            std::map<PeerName, int>::iterator it = mapOfPeers.find(mpeer);
            if (it != mapOfPeers.end()) {
                if (ackInfo.waitAckFrom[mpeer] && frames->isAck()) {
                    // If we were waiting for ACK from peer, and this is
                    // an ACK, we remove the flag, and forget about the
                    // message
                    dumpId(selfPeer()->name.c_str(),
                           ("ACK RECEIVED from " + mpeer).c_str());
                    ackInfo.waitAckFrom[mpeer] = false;
                    ackInfo.cycles[mpeer] = 0;
                    DESTROY_MESSAGE( ackInfo.lastTx[mpeer].second );
                } else if (frames->ackRqsted()) {
                    // If the sender requests an ACK, send it immediately
                    // and remove the ack flag
                    Frame msgType = frames->type();
                    (*frames)[FRAME_MSG_TYPE] = msgType;
                    inTx.push_back(std::make_pair(peers.at(it->second), frames));
                    setTransmissionToPeer(mpeer, buildPeerMsg(mpeer, "ACK"));
                } else {
                    // Else, store in the inbox as normal
                    inTx.push_back(std::make_pair(peers.at(it->second), frames));
                }
            }
        }
        std::string mc;
        GETMSG(mc, (*frames));
        dumpId(selfPeer()->name.c_str(),("Recv tx => " + mc).c_str());
    }
}

//----------------------------------------------------------------------
// Method: setDebugInfo
// Sets flag for output of debug information
//----------------------------------------------------------------------
int64_t Router2RouterPeer::setClock(int n)
{
    static steady_clock::time_point t1;

    if (n == 1) {
        t1 = steady_clock::now();
        return -1;
    } else {
        steady_clock::time_point t2 = steady_clock::now();
        microseconds time_span = duration_cast<microseconds>(t2 - t1);
        return time_span.count();
    }
}

//----------------------------------------------------------------------
// Method: setDebugInfo
// Sets flag for output of debug information
//----------------------------------------------------------------------
void Router2RouterPeer::setDebugInfo(bool b)
{
    generateDebugInfo = b;
}

//----------------------------------------------------------------------
// Method: getDebugInfo
// Returns value of flag used to show debug information
//----------------------------------------------------------------------
bool Router2RouterPeer::getDebugInfo()
{
    return generateDebugInfo;
}

//----------------------------------------------------------------------
// Method: dumpMsgStats
// Generates table with the content of the vector of MsgStats
//----------------------------------------------------------------------
void Router2RouterPeer::dumpMsgStats(std::string fileName)
{
    std::ofstream ofs;
    ofs.open(fileName, std::ofstream::out | std::ofstream::app);
    if (!ofs) { return; }

    for (unsigned int i = 0; i < msgStats.size(); ++i) {
        MsgStats & v = msgStats.at(i);
        ofs << v.peerId << '\t'
            << v.msgType << '\t'
            << v.io << '\t'
            << v.msgContentSize << std::endl;
    }

    ofs.close();
}

}
