# UPD

## Functional Design

UDP is a high speed IP transport layer protocol that is connectionless and
prioritizes speed and efficiency over reliability. The Unid software uses very
simple data architecture consisting of Plan Old Data (POD). It does not
communicate arrays, variable length strings, or large amounts of data, so UDP
is an ideal mechanism for tranfering the data between client, server, and
backup nodes.

Application requirements:

* Efficent transport of small binary based command set
* Bandwidth for a server to serve unid blocks to thousands of clients

QUIC requirements:

* Asynchronous operation
* Transmitted data buffers managed by QUIC
* Received data buffers managed by UDP client/server


* [Asio C++ Documentation](https://think-async.com/Asio/asio-1.36.0/doc/asio/overview/model.html)

The low-level interface to the UDP sockets is optimized to feed the QUIC
network layer with the sources and sinks of data to/from the client.

## Detailed Design

### UDP Client/Server
A high performance UDP client and UDP server with the following features/benifits:

* Asynchronous (non-blocking) operation
  * Higher performance, especially on single-threaded applications
* Buffer pool for memory management
  * No run-time memory allocation for better performance
* Threaded operation for better CPU core usage
  * If needed, multiple threads allow high CPU usage and better performance
* Zero copy potential for received data (io-uring)
* C++20 Co-routines
  * Enhances readability and understanding for developers for async processing
  * Faster execution with shared heap memory versus stack memory
* Common code base for server and client
  * Supports evolution of newer technologies levaraged into both server and
  client

The interface between a UDP server/client and Google's QUICHE library in C++ is a decoupled event-driven, user-space state machine architecture. [1, 2, 3] 
Because Google's QUICHE does not handle system I/O directly, your UDP application layer is entirely responsible for operating the network sockets, driving the event loop, and managing timers. The communication flows through abstract helper classes and packet processing pipelines. [1, 3] 
------------------------------
## 1. Architectural Responsibility Overview

+-------------------------------------------------------------+

|               Your Application C++ Code                     |
|  [UDP Sockets (sendto/recvfrom)]  <--->  [Event Loop / Epoll]|
+-------------------------------------------------------------+

        |                                       |
  Raw UDP Packets                         Clock / Timers
        v                                       v
+-------------------------------------------------------------+

|                      Google QUICHE                          |
|  [QuicDispatcher]   [QuicConnection]   [QuicCryptoStream]   |
+-------------------------------------------------------------+

## 2. The Core Interface Components
To connect your UDP sockets to QUICHE, your C++ application must implement and bind several interface classes provided by the library: [4, 5] 
## A. Inbound Data Interface: QuicDispatcher
The QuicDispatcher is the primary interface for your UDP server logic.

* The Process: Your code calls standard system APIs like recvfrom() or recvmmsg() on the UDP socket to extract raw network buffers.
* The Hand-off: You bundle this payload along with the peer/local IP addresses and a timestamp into a QuicReceivedPacket. You then call QuicDispatcher::ProcessPacket().
* The Routing: The dispatcher reads the QUIC Connection ID inside the packet. If the connection exists, it routes it to the correct session. If it is a new connection ID, it orchestrates the cryptographic handshake to spin up a new session. [6, 7, 8] 

## B. Outbound Data Interface: QuicPacketWriter
QUICHE uses a callback-based interface to push data back out to the UDP socket. You must inherit from QuicPacketWriter and implement its writing methods: [9] 

* WritePacket(const char* buffer, size_t buf_len, const QuicIpAddress& self_address, const QuicSocketAddress& peer_address, ...)
* Inside your custom override of this function, your code calls standard UDP socket writes (e.g., sendto() or sendmmsg()) to transmit the serialized bytes over the wire. [10] 

## C. Time and Event Loop Interface: QuicConnectionHelperInterface & QuicAlarmFactory
QUIC relies heavily on precise timing for congestion control, packet retransmission, and idle timeouts. Because QUICHE cannot create OS threads or register system timers itself, you must bridge your application's event loop (like epoll, libevent, or asio) using two abstractions: [3, 11, 12, 13] 

* QuicClock: An interface you implement so QUICHE can fetch the current monotonic system time.
* QuicAlarmFactory: An interface used by QUICHE to build "alarms" (timers). When QUICHE needs a timeout, it requests an alarm. Your C++ code must schedule this event in your custom event loop. When the event loop fires, your application invokes the alarm's callback to pass control back to QUICHE. [4, 14, 15] 

## D. Stream and Payload Interface: QuicSession & QuicStream
Once a connection is established, the interface shifts away from UDP packets to application streams: [2, 16] 

* QuicSession: Manages the life cycle of a connection. You subclass this to handle events like a peer creating a new multiplexed stream.
* QuicStream: Represents the bidirectional or unidirectional data pipes. You pass application-layer buffers into QuicStream::WriteOrBufferData(), which prompts QUICHE to assemble, encrypt, and hand off new QUIC frames to your QuicPacketWriter. [2, 4, 14, 17, 18] 

------------------------------
## 3. The Sequence of Interaction## Client-Side Workflow

   1. Initiate: Your client creates a QuicConnection and calls a handshake method (e.g., via QuicCryptoClientStream).
   2. First Write: QUICHE realizes it must send a Client Hello. It constructs a packet and calls your QuicPacketWriter::WritePacket().
   3. Transmission: Your writer calls sendto() on your client's UDP socket.
   4. Response: Your event loop detects data on the UDP socket. You call recvfrom() and feed the buffer into QuicConnection::ProcessUdpPacket(). [19, 20, 21, 22] 

## Server-Side Workflow

   1. Listen: Your application binds to a port and waits on an epoll read event.
   2. Ingest: A UDP packet arrives. You read it and pass it to QuicDispatcher::ProcessPacket().
   3. Session Creation: If it's a new client, QuicDispatcher generates a new QuicSession.
   4. Processing: QUICHE processes the cryptographic handshake, generates a server response packet, and throws it down to your registered QuicPacketWriter to be blasted back via sendto(). [23, 24, 25] 

[AI Link](https://share.google/aimode/TEzko0otpvpFdncNv)

### QUIC Data Interface Inbound: QuicDispatcher

1. UDP recvfrom - Extract raw network buffers
1. Handoff - Payload with peer/local IP address and a timestamp into a
  QuicReceived Packet. Then call QuicDispatcher::ProcessPacket().
1. Routing - Dispatcher gets the connectionm ID and routes it. If it is a
new connection, it orchestrates the crypto handshae to spin up a new session.

### QUIC Data Interface Outbound: QuicPacketWriter

1. WritePacket()
1. Override this function, calling standard UDP socket writes to transmit the
serialize bytes over the wire.

### Quiche to UDP Client/Server overview:

'''
+--------------------------------------------------------------+
|               Udp Client/Server C++ Code                     |
|  [UDP Sockets (sendto/recvfrom)]  <--->  [Event Loop / Epoll]|
+--------------------------------------------------------------+
        |                                       |
  Raw UDP Packets                         Clock / Timers
        v                                       v
+-------------------------------------------------------------+
|                      Google QUICHE                          |
|  [QuicDispatcher]   [QuicConnection]   [QuicCryptoStream]   |
+-------------------------------------------------------------+
'''

### Libraries

* [ASIO (standalone)](https://think-async.com) - Async support library
* [Quill](https://quillcpp.readthedocs.io/en/latest/) - High performance logger
* [Libpool++ Wrapper](https://github.com/8dcc/libpool) - A simple buffer pool

### Details

See the well documented code for detailed implementation documentation.

[udp_server.hpp](/server/network/udp_server.hpp)
[udp_client.hpp](/client/network/udp_client.hpp)

