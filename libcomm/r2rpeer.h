/******************************************************************************
 * File:    r2rpeer.h
 *          This file is part of the LibComm communications library
 *
 * Domain:  LibComm.LibComm.Router2RouterPeer
 *
 * Version: 0.0.1
 *
 * Date:    2015/07/01
 *
 * Copyright (C) 2015 J C Gonzalez
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declare Router2RouterPeer class
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

#ifndef R2RPEER_H
#define R2RPEER_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   - string
//   - vector
//   - map
//   - deque
//   - atomic - std::atomic, std::atomic_flag, ATOMIC_FLAG_INIT
//   - thread - std::thread, std::this_thread::yield
//------------------------------------------------------------

#include <string>
#include <vector>
#include <map>
#include <deque>
#include <vector>
#include <atomic>         // std::atomic, std::atomic_flag, ATOMIC_FLAG_INIT
#include <thread>         // std::thread, std::this_thread::yield
#include <chrono>
using namespace std::chrono;

//------------------------------------------------------------
// Topic: External packages
//   - zmq.hpp - Zero-MQ messaging framework
//------------------------------------------------------------

#include <zmq.hpp>

//------------------------------------------------------------
// Topic: Project headers
//   none
//------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////
// Namespace: LibComm
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace LibComm {

//==========================================================================
// Class: Router2RouterPeer
//==========================================================================
class Router2RouterPeer {

public:

    //----------------------------------------------------------------------
    // Typedefs: Router2RouterPeer type definitions
    //   PeerName           - Peer name string type
    //   Peer               - Peer node specification structure
    //   PeerTable          - Vector of peer specification pointers
    //   PeerMap            - Map of node index in peer table to peer names
    //   Frame              - One of the parts of a message
    //   PeerMessage        - Message, vector of Frames
    //   Transmission       - Pair of peer specification and message (pointers)
    //   TransmissionsQueue - Queue to store incomming/outgoing transmissions
    //   Byte               - Single byte (char) element
    //   BytePtr            - Pointer to Byte variable
    //----------------------------------------------------------------------
    typedef std::string                      PeerName;

    struct Peer {
        PeerName     name;
        std::string  type;
        std::string  clientAddr;
        std::string  serverAddr;
    };

    typedef std::vector<Peer*>               PeerTable;
    typedef std::map<PeerName, int>          PeerMap;

    typedef std::string                      Frame;
    //typedef std::vector<Frame>               PeerMessage;

    class PeerMessage {
    public:
        PeerMessage() {}

        size_t size() const { return frames.size(); }
        void clear() { frames.clear(); }

        void push_back(std::string s) { frames.push_back(s); }

        Frame & at(size_t n)             { return frames.at(n); }
        const Frame & at(size_t n) const { return frames.at(n); }

        Frame & operator[](size_t n)             { return frames[n]; }
        const Frame & operator[](size_t n) const { return frames[n]; }

        PeerName peer() { return frames.at(FRAME_PEER_ID); }
        std::string type() {
            if (!ackRqsted()) { return frames.at(FRAME_MSG_TYPE); }
            else { return frames.at(FRAME_MSG_TYPE).substr(0, tagPos); }
        }
        std::string content() { return frames.at(FRAME_MSG_CONTENT); }
        bool isAck() { return frames.at(FRAME_MSG_CONTENT) == "ACK"; }
        bool ackRqsted() {
            tagPos = frames.at(FRAME_MSG_TYPE).find(":ack");
            return (tagPos != std::string::npos);
        }
    private:
        std::vector<Frame> frames;
        size_t tagPos;
    };

    typedef std::pair<Peer *, PeerMessage *> Transmission;

    typedef std::deque<Transmission>         IncomingTransmissions;
    typedef std::deque<Transmission>         OutgoingTransmissions;

    typedef char Byte, * BytePtr;

    //----------------------------------------------------------------------
    // Enum: FrameType
    //   FRAME_PEER_ID     - Target node identifier frame
    //   FRAME_MSG_TYPE    - Msg.Type frame
    //   FRAME_MSG_CONTENT - Content part of the message
    //----------------------------------------------------------------------
    enum FrameType {
        FRAME_PEER_ID = 0,
        FRAME_MSG_TYPE = 1,
        FRAME_MSG_CONTENT = 2
    };

    // In order to gather statistics on messages sent and received,
    // the following structure is defined
    struct MsgStats {
        MsgStats(std::string p, std::string t, int i, int c) :
            peerId(p),msgType(t),io(i),msgContentSize(c) {}
        std::string peerId;
        std::string msgType;
        int         io;
        int         msgContentSize;
    };

    // Class to handle acknowledgement information
    struct AckInfo {
        std::map<PeerName, bool>         waitAckFrom;
        std::map<PeerName, int>          cycles;
        std::map<PeerName, Transmission> lastTx;
    };


public:

    //----------------------------------------------------------------------
    // Constructor: Router2RouterPeer
    //----------------------------------------------------------------------
    Router2RouterPeer();

    //----------------------------------------------------------------------
    // Destructor: ~Router2RouterPeer
    // Virtual destructor
    //----------------------------------------------------------------------
    virtual ~Router2RouterPeer();

    //----------------------------------------------------------------------
    // Method: addPeer
    // Adds a new peer to the set of peers to form the network
    //
    // Parameters:
    //   peer       - (I) Pointer to the peer node specification to be added
    //   isThisPeer - (I) True if peer corresponds to this node
    //----------------------------------------------------------------------
    void addPeer(Peer * peer, bool isThisPeer = false);

    //----------------------------------------------------------------------
    // Method: selfPeer
    // Return self peer
    //
    // Returns:
    //   Pointer to this object node configuration
    //----------------------------------------------------------------------
    Peer * selfPeer();

    //----------------------------------------------------------------------
    // Method: isPeer
    // Return TRUE is the named peer is among the peers of this component
    //----------------------------------------------------------------------
    bool isPeer(PeerName aPeer);

    //----------------------------------------------------------------------
    // Method: broadcast
    // Adds a new message to be sent to all peers
    //
    // Parameters:
    //   peerMsg - (I) Pointer to message to be sent to all the peers
    //----------------------------------------------------------------------
    void broadcast(PeerMessage * peerMsg);

    //----------------------------------------------------------------------
    // Method: setTransmissionToPeer
    // Adds a new message to be sent to a given peer
    //
    // Parameters:
    //   name    - (I) Target peer name (message recipient)
    //   peerMsg - (I) Pointer to message to be sent to the target peer
    //----------------------------------------------------------------------
    void setTransmissionToPeer(PeerName name, PeerMessage * peerMsg, bool ack = false);

    //----------------------------------------------------------------------
    // Method: setTransmissionToPeer
    // Adds a new message to be sent to a given peer
    //
    // Parameters:
    //   peer    - (I) Target peer specification pointer of mpeerthe recipient
    //   peerMsg - (I) Pointer to message to be sent to the target peer
    //----------------------------------------------------------------------
    void setTransmissionToPeer(Peer * peer, PeerMessage * peerMsg, bool ack = false);

    //----------------------------------------------------------------------
    // Method: getNewTransmission
    // Returns a new message from the queue of received messages
    //
    // Parameters:
    //   peer    - (O) Reference to specification of the sender peer
    //   peerMsg - (O) Reference to the message received
    //----------------------------------------------------------------------
    void getNewTransmission(Peer & peer, PeerMessage & peerMsg);

    //----------------------------------------------------------------------
    // Method: thereArePendingTransmissions
    // Returns true if there are pending incoming transm. to be processed
    //
    // Returns:
    //   True if there are pending incoming messages
    //----------------------------------------------------------------------
    bool thereArePendingTransmissions();

    //----------------------------------------------------------------------
    // Method: storeAtBuffer
    // Places the content of a frame into a buffer
    //
    // Parameters:
    //   frm    - (I) Message frame
    //   buffer - (O) Target buffer
    //----------------------------------------------------------------------
    void storeAtBuffer(Frame & frm, BytePtr & buffer);

    //----------------------------------------------------------------------
    // Method: buildPeerMsg
    // Creates a new 3-frame message
    //
    // Parameters:
    //   to        - (I) Name of the target node
    //   msg       - (I) Message content
    //   type      - (I) Message type string
    //
    // Returns pointer to PeerMessage (so the only creator and destroyer of
    // PeerMessages is this class)
    //----------------------------------------------------------------------
    Router2RouterPeer::PeerMessage *
    buildPeerMsg(std::string to,
                 std::string msg = std::string(),
                 std::string type = std::string("MESSAGE"));

    //----------------------------------------------------------------------
    // Method: dump
    // Dumps peer node specification
    //----------------------------------------------------------------------
    void dump();

    //----------------------------------------------------------------------
    // Method: deactivateCommunications
    // Deactivate communicationsActive flag
    //----------------------------------------------------------------------
    void deactivateCommunications();

    //----------------------------------------------------------------------
    // Method: waitForStartSignal
    // Activates waitForStart flag
    //----------------------------------------------------------------------
    void waitForStartSignal();

    //----------------------------------------------------------------------
    // Method: emitStartSignal
    // Reaction upon start signal reception
    //----------------------------------------------------------------------
    void emitStartSignal();

    //----------------------------------------------------------------------
    // Method: setDebugInfo
    // Sets flag for output of debug information
    //----------------------------------------------------------------------
    void setDebugInfo(bool b);

    //----------------------------------------------------------------------
    // Method: getDebugInfo
    // Returns value of flag used to show debug information
    //----------------------------------------------------------------------
    bool getDebugInfo();

    //----------------------------------------------------------------------
    // Method: dumpMsgStats
    // Generates table with the content of the vector of MsgStats
    //----------------------------------------------------------------------
    void dumpMsgStats(std::string fileName);

    void checkIfAckFromRecip(PeerName recipient, std::deque<Transmission>::iterator outIt, Transmission tx, int64_t timeSpan, PeerMessage* peerMsg);
    void processIncomingPeerMsg(static size_t type_size, bool waitStart, zmq::pollitem_t servers, int64_t rcvmore, zmq::socket_t skServer);
    void processIncommingPeerMsg(zmq::pollitem_t servers, bool waitStart, zmq::socket_t skServer);

protected:

    //----------------------------------------------------------------------
    // Method: startSignalReceived
    // Polls whilel waiting for start signal reception
    //----------------------------------------------------------------------
    bool startSignalReceived();

    //----------------------------------------------------------------------
    // Method: establishCommunications
    // Creates the appropriate 0MQ sockets for the communication
    //----------------------------------------------------------------------
    void establishCommunications();

    //----------------------------------------------------------------------
    // Method: transmissionsHandler
    // Delegated Handler for all incoming and outgoing transmissions
    //----------------------------------------------------------------------
    void transmissionsHandler();

    //----------------------------------------------------------------------
    // Method: processIncommingPeerMsg
    // Process incomming transmission
    //----------------------------------------------------------------------
    void processIncommingPeerMsg(zmq::pollitem_t servers,
                                 bool waitStart,
                                 zmq::socket_t skServer);

    //----------------------------------------------------------------------
    // Static Method: launchTransmissionsHandler
    // Handler for all incoming and outgoing transmissions, running in
    // a dedicated thread
    //----------------------------------------------------------------------
    static void * launchTransmissionsHandler(void * obj);

    //----------------------------------------------------------------------
    // Static Method: readEventMsg
    // Reads socket connection events
    //----------------------------------------------------------------------
    static int readEventMsg(void* s, zmq_event_t* event, char* ep);

    //----------------------------------------------------------------------
    // Static Method: routerSocketMonitor
    // Checks the router socket events
    //----------------------------------------------------------------------
    static void * routerSocketMonitor(void * ctx);

    //----------------------------------------------------------------------
    // Method: setDebugInfo
    // Sets flag for output of debug information
    //----------------------------------------------------------------------
    int64_t setClock(int n = 1);

protected:

    //----------------------------------------------------------------------
    // Variables: Protected data members
    //   r2rpeerThreadId - Communications thread identifier
    //   inTx            - Queue with incoming messages
    //   outTx           - Queue with outgoing messages
    //   mutexSelf       - Mutex for thread-safe operations
    //   selfIdx         - Index of this node in peers table
    //   self            - This node specification (pointer)
    //   peers           - Table of peers
    //   mapOfPeers      - Maps indices in table of peers with peer names
    //   iterations      - Number of iterations waiting for start signal
    //----------------------------------------------------------------------

    //pthread_t             r2rpeerThreadId;
    std::thread           r2rpeerThread;

    IncomingTransmissions inTx;
    OutgoingTransmissions outTx;

    pthread_mutex_t       mutexSelf;
    std::atomic<int>      selfIdx;
    std::atomic<Peer *>   self;

    PeerTable             peers;
    PeerMap               mapOfPeers;

    AckInfo               ackInfo;

    std::atomic<int>      iterations;

    std::vector<MsgStats> msgStats;

    //----------------------------------------------------------------------
    // Flags: Protected flags
    //   communicationsActive - Status of communications
    //   waitForStart         - True if waiting for start signal to arrive
    //   startReceived        - True if start was received
    //----------------------------------------------------------------------

    std::atomic<bool>     communicationsActive;
    std::atomic<bool>     waitForStart;
    std::atomic<bool>     startReceived;

    static std::atomic<bool>     generateDebugInfo;
};

void dumpDebugInfo(const char* tag, const char* msg = 0);

}

#endif  /* R2RPEER_H */
