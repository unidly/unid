# Unid Functional Design Document

Within a given distributed network context, a Unid (universal id) is a unique
integer. The unid software guarantees that it is unique through its generation
and distribution.

Multiple Unid Servers are regionally distributed, to provide a failsafe
distribution of Unids to Unid Clients. A Unid Client is associated with
Application Software that needs unique identifiers for use within the
application. The Unid Client interfaces with the application software either
through shared memory, of through a localhost socket. Upon request, the
application obtrains a Unid for its use.

Characteristics of a Unid:

* A Unid is unique
* A Unid is sequential
* A Unid is an integer
* Unids are available in 2-byte, 4-byte, 8-byte or 16-byte widths.
* Unids do not wrap
* Unids are designed for a distributed system where uniqueness is required
* A Unid is easier for humans to read and use

Problems with the Unid alternative, UUID:

* A UUID cannot be guaranteed to be unique
* A UUID takes up 16 bytes for every associated record
* A UUID can be generated in the application, or in the database
* A UUID often provides sub-optimal indexes due to its structure
* Lastly, human readability of UUIDs is problematic

## How does a Unid work

A Unid is provided by a "central authority" to applications that have need of
unique identifiers. Fault tolerant distributed system designers, when faced
with a "central authority" proposal immediately reject the concept as being
too risky to use in a fault tolerant system.

The creators of the Unid reject this misconception. Unid software provides
a higher degree of reliability, speed and efficiency than the systems that
use it. This is possible because of the distributed and redundant Unid servers
that make use of consensus to maintain replicated data across the global
network. Its performance is provided through the unid client software that
operates on the same machine as the database API, and also on the API backup
servers. The channel used to serve a Unid to the application API can be IPC
through shared memory, or IPC using localhost sockets on the machine.

## Unid Leader Server

The Leader Server generates and distributes the unids to the Follower Servers.
The Follower Servers request a range of unids when their supply runs low.
Their is only one Leader Server in the distributed environment that is
responsible for providing Unid Blocks to Follower Servers.

Upon request from a Follower Server, the Leader Server provides the Follower
Server with a block of unids.

The number of nids provided are determined by the historical record calculated
by the Follower server. The intention is to provide a number of unids that will
be consumed within a specific TBD time period. This period should be long enough
to time with margin for server opeartion, but not so long that large blocks of
unids are lost in a node failure.

## Unid Client Server

The slave servers principal job is to pass unids to the web service (api) when
needed to provide a unique id across the entire database. This is accomplished
through the use of shared memory and a simple circular queue. The web service
dequeus a unid from the head of the queue. When the queue is becoming close
to exhausted, the slave server refills the circular queue with values that
made based on the range of unids provided by the master server.

## Standby servers {#standby-servers-id}

In order to provide a fail-safe operaton, standby unid servers are maintaindd on a
geographically distant server. The standby servers provide a fall back should
the master have a hardware failure or network discontinuity.

The standby servers and completely synchronized with the master server through
a socket connection with TCP. The dataset in minimal. The standby servers just
need to know what the next available unid is and the dns records of all of the
slaves servers in the network. The standby server becomes the master server
when the current master server is found to be inaccessible.

A monitoring program is used to determine if the master server is not able to
function due to hardware or network failure. When this monitor program
determines that the master server is not available, it signals the standby
server to take over the responsibilities of the master, and notifies the
slave servers of where to send future requests for unids.

The overall system design provides time for the slave servers to switch to the
new master server is such that delays in providing unids to the web services
is minimal. A Paxos algorthim is used to allow the system to select a new
node that is the master server.

## Unid

An unid is a unique (within the distributed database) 64-bit number that is
provide to the web service as it consumes them to provide an id for a
database primary key record during a create activity.

The purpose of an unid is to make each record within the database have a
unique id similar to a uuid. We use a unid instead of a uuid for the following
reasons:

* Consumes less database space for storage
* Is a native data type that is efficiently handled by the database and the web service
* It's sequential nature lends itself to efficient indexing for database lookup
* It can efficiently be passed through HTML rest as it does not require encoding
* It is much more user friendly, again as it does not require encoding

Note that for medical records, having a unique identifier for each database
record simplifies compliance and information retrieval for the application.

## Unid Block Request

A range request consists of two 64-bit numbers. The first is the unid that is
available for the web service, and the second is the number of consecutive
unids assigned for the distribution from the slave as needed by the service.

For example, the provided tuple consists of 351643 followed by 10000. This
range request response allows the slave to use the first value, 351643 as
the first unid available, and that the next next 99999 sequential numbers that
are a result of an increment. The total number of ids available is set by
the second parameter.

next_unid = 351643;
n = 10000
do {
    wait for request
    return next_unid;
    next_unid++;
    n--;
} while(n);

## Network protocol

Unid communicates plain old data (POD) over the internet with a proprietary
protocol using UDP. Use of UDP provides better throughput, at the cost of
implementing the protocol. Given the simple nature and amount of data passed
between server and client, UDP provides the best throughput and with the
protocol, absolute reliability.

Functional design objectives:

* All communications over the internet use IP protocol UDP.
* All data packets contain a sequence number, and a CRC32.
* C++ data structures consist of POD
* C++ data structures are serialized using Google FlatBuffers

## The future of the Unid

All databases are capable of generating UUIDs during the insertion of a new
record. This is convenient for the application, but the convenience is often
reduced when the application would like to immediately act on the insertion
of a new record, as it must make an extra request of the database for the
UUID of the inserted record.

There is nothing about the technology of the Unid to keep the database from
obtaining an inserting a Unid during new record creation instead of a UUID.
The creators of the Unid are actively working with major DB providers to
support the Unid as a new datatype that can be suppled via the database.`


## Terminology

node - an instance of either a unid server or unid client

## Data alignment

All data structures are aligned in memory based on the target system's
Architecture Binary Interface.

## Network address tables

All nodes  in the distributed network must be accessible over the internet.
The following table displays the nodes network data.

| Name             | Data type | Description |
| ---------------- | --------- | ----------- |
| Id               | uitn64    | A single unique identifier of the node.        |
| IP Address v4/v6 | uint64    | Binary IP address v4 32-bits, v6 64-bit        |
| Port             | uint16    | Client set by server, Server manually assigned |
| Role             | uint8     | The role, 0 = server, 1 = client               |

```c++
struct Node_network_data {
    uint64_t id;
    uint64_t ip;
    uint16_t port;
    uint16_t seq;
    uint8_t role;
}
```

The network address table, consists of all of the servers and clients in the
network. The table is replicated 

## Unid block

A unid is a single integer that is guaranteed to be unique within a
distributed network. Unids are passed from a unid server to a unid client
on request by the client. Unid servers send unids within a structure that
defines the first unid (starting value), and an integer that indicates the
block size of the unids available for passing from the client to the
application.

| Name | Data type | Description                   |
| ---- | --------- | -----------                   |
| unid | uint64    | A unique integer              |
| size | uint16    | Number of unids in this block |
| seq  | uint32    | Sequence number

```c++
struct Unid_block {
  uint64_t unid;
  uint16_t size;
  uint32_t seq;
}
```

