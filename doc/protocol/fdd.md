# Unid Protocol Function Design Specification

The system level design includes using the QUIC protocol and transports data
using UDP datagrams. This low level protocols meets the functional design
requirements for performance, latency, security, and long term usability.

This design specification focuses on the Und Protocol command set that is
used to communicate between the client and server, and also as a foundation
for the Server peer-to-peer and Client peer-to-peer protocols used as part
of failsafe operation using consensus to maintain data and state machine
integrity.

## Protocol overview

### Message Framing (The "Wire Format")

Fixed-length headers that contain a command ID and payload length, followed
by the command data.

Zero-copy design with fields aligning with memory boundaries for direct copy
into C++ structures.

### Stream Management

QUIC provides "streams" which are independent, reliable sequences of bytes. For
simplicity, we use a single command per bidirection stream.

### Low latency

A feature of QUIC is the 0-RTT that combines data in the very first UDP packet.
This effectively allows near-zero latency for established client server
interactions. Other transport layer protocols often require multiple round-trips
of data packets to establish the connection and the data encryption keys.

### High security (TLS 1.3)

Another feature of QUIC is that it automatically encodes all data sent over the
internet. This keep hackers, and middle boxes, from sniffing the data and
interfering with the protocol, or the ability to have a safe and reliable
network connection.

## Protocol Functional Design

Unid Server to Client and Peer to Peer protocols are compact, binary designed
for highly efficient use of the "wire" and along with highly efficient for the
Server and Client software to serialize and deserialize.

### Command ID

A command id is an extensible data type that is encoded in one or two bytes.
Somewhat similar to the UTF-8 encoding scheme, the Unid command id coding
scheme uses the most significant bit of the first byte to indicate how many
bytes are needed to express the command ID.

If the command is contained in a single byte, the first bit of the command id
is set to 1, the remaining 78 bytes indicate the command ID giving us a range
of 0 to 128 distinct command ids. For a two byte command, should we need to
expand beyond 128 commands (who knows? I hope not), then the most significant
bit of the first byte is set to 0. If the most signifcant bit of the first
byte is set to zero, then a second byte is added. This allows the number of
command ids to range from 0 to 32768 potential commands.

### Command payload length

After the Command ID (one or two bytes), the length of the payload is
encoded in the same manner, with either one or two bytes. The same as the
Command ID, if the MS bit of the payload length is 1, then the length is
contained in the following 7 bits, with lengths ranging from 0 to 128. If the
MS bit is 0, then the length is a combination of the fist byte and an
additional byte with a maximum payload length of 32768 bytes.

### Design goal

A design goal is to incorporate the entire command set in less than 128 ids,
and to have the maximum command payload size less that 128 bytes. This allows
a very compact payload header of 2 bytes.

Given the mission of Unid, that is providing sequential ordered integers of
up to a range of 4 bytes. This design goal seem easily acheivable. The ability
to increase the number of commands, and the command payload allows expansion
of the infrastructure to provide other data entity types.

### Command payload

The comman payload is the data necessary to the execution of a command. The
data is in binary format (no text!) and is streamed in a manner that reduces
the complexity encoding and decoding the data. Essentially, the command
payload maps directly into a command structure that is implemented within a
C++ data structure. Encoding and decoding is simply the removal of (on the
sender) the padding bytes that are embedded in the C++ structure for alignment
purposes. Upon receipt, the receiver simply maps the data back into the
structure in a way that is memory aligned to match that of the sender.

### How is the command response correlated with the command request?

The nature of UDP protocols is that is it possible that a command or its
response can be delayed and returned out of order. Normally, the application
layer incorporates a sequence number or other mechanism to make sure that
any response can be correlated with the sent command.

QUIC assists the application layer providing a stream ID. The stream id can
be obtained by the sender from QuicStream object. When a response is received,
the quic stream ID can be retrieved allowing the protocol to match the
response to the request. This simplifies and makes more efficient the
Unid command structure

## Commands and Responses

### Command ID 0: The Heartbeat command

The simplest Unid command is designed to let the server monitor the life of
a client node. The hearbeat command is encoded as follows

```
  Cmnd ID    Len
 10000000 | 1000000
```

The command number is 0, and the payload length is 0. This command is sent
from the client to the server. This is the only command that is not
acknowledged by the server. The server may monitor the heartbeat and take
action if one or more heartbeats are missing to log the situation, and
potentially broadcast a notification indicating the issue to the appropriate
person or software.

### Command ID 1: The Get Unid Block command

When the client needs additional unids to distribute to the application, it
send the Get Unid Block command to the associated server.

Request:

```
   Cmnd ID    Len
  10000001 | 1000000

  Command ID: 1
  Command Length: 0
```

Response:

```
Cmnd ID    Len       Numbor of Unids      First Unid
10000001 | 1000011 | 00000000 | 00000000 | 00000000

Command ID: 1
Command Length: 3
Number of Unids: 10
First Unid in the block: 33
```

Notes: 

* The block size is encoded in a 16-big (MSByte first) word
* This implies that the maximum number of unids in a block are 65536
* The payload length implies the width in bytes of the Unid
* Unid width in bytes matches the C++ integer datatypes (see table)
* On the wire, Network Byte Order is used
* The host architecture determins the byte order for the application

| Len | Data type | # of Octets | Max # of Unids |
| --- | --------- | ----------- | -------------- |
|  3  | Octet     |      8      | 256            |
|  4  | Short     |     16      | 65,536         |
|  6  | Int       |     32      | 4,294,967,295  |
| 10  | Long      |     64      | 18,446,744,073,709,551,615 |
| 18  | Long Long |    128      | 340,282,366,920,938,463,463,374,607,431,768,211,455 |

### Command ID 2::
