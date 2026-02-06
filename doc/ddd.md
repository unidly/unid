# Unid Detailed Design Document

## Toolset
This project is designed to be fully contained without the use of any third
party libraries. Eliminating third party libraries requires re-invention of
many wheels, but our belief is that in a project such as this, it is important
to be independent of third party software that is not under our development
control.

* gcc14-g++ - C++ compiler that supports C++20
* Cppcheck - Static code analyzer
* Valgrind - Runtime leak analyzer
* Catch2 - Unit test framework

## References
### Networking

UDP protocol implemented at the socket level for client/server communications.

* [sample implementation in c++](https://cppcodetips.wordpress.com/2014/01/29/udp-socket-class-in-c)
* [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/)

### Consensus algorithm

Paxos is the default consensus algorithm that is used to provide failover

* [The Part-Time Parlaiment](https://lamport.azurewebsites.net/pubs/lamport-paxos.pdf)
* [Paxos Made Simple](https://lamport.azurewebsites.net/pubs/paxos-simple.pdf)
* [Notes on Paxos Made Simple](https://medium.com/@adityashete009/notes-on-paxos-made-simple-1e693d60f51e)
* [Paxos in Pictures Video](https://www.youtube.com/watch?v=UUQ8xYWR4do&t=322s)

### Interprocess communication

The Unid client passes Unids to the application using shared memory and a
simple circular queue. The queue contains Unid values passed from the unid
client to the application..

Using shared memory for Inter-Process Communication (IPC) in C++ is highly
efficient because it avoids data copying between kernel and user space.
However, it requires careful management of synchronization using mechanisms
like mutexes or semaphores to prevent race conditions and data corruption.

Here are implementation of a class that handles a circular queue in C++:

* [Shared Memory with POSIX API](https://eric-lo.gitbook.io/synchronization/shared-memory)
* [Lock-Free Single-Producer Single-Consumer Circular Queue](https://main.codeproject.com/articles/Lock-Free-Single-Producer-Single-Consumer-Circular#comments-section)

## Overview

Unid is a fault tolerant, distributed, sequential integer server and client
that is used by applications as a central repository of integers.

A primary use of Unid is to generate primary keys (PKs) for a database
application where the application sets the id primary key during INSERT
SQL operations. This alleviates the database from PK generation, and in a
distributed environment, assures that there is no PK conflict across
a global, distributed database application.

Unid provides the following features

* Sequential unique integer provider
  * Integer types -  32-bit, 64-bit and 128-bit integers
  * Global scale - normally installed on each database node
* Failsafe operation designed for global scale database support
  * Redundancy - each node can be a server or client
  * Replication - data mirrored across multiple nodes
  * Consensus algorithm - coordinates nodes during failure or conflict
  * Failover - automatically reconfigures on failure
  * No software dependencies - 
* Unique indexes for globally distributed database
  * Guaranteed to never issue the same integer twice
  * Prevents index conflict
  * Supports indexes that are easily indexed and sequential
* High performance
  * Fast UDP datagram - communication between nodes
  * Shared memory IPC with application for passing id
  * Alleviates need for round-trip to RDBMS for DB generated id

### Networking

Unid uses the UDP protocol for communication between Unid Servrs and Unid
Clients. A protocol is implemented that uses the communication channels for
both communication and failure detection.


### Consensus

A Paxos consensus algirithm is used to detact and adapt the Unid system for
failures, and the addition of new nodes to the system.

### Interprocess Communicaiton

Applications access unique integers using either shared memory IPC for best
performance, or though IP sockets for applications that are detached from the
database node.

### Operation Guarantees

* Unique integers - Unid guarantees to produce unique ascending ids
* MTBF
* MTTR
* MTTF
