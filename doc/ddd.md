# Unid Detailed Design Document

## Toolset

The following software tools are used for unid development:

* [gcc14-g++](https://gcc.gnu.org/) - C++ compiler that supports C++20
* [Cppcheck](https://cppcheck.sourceforge.io/) 2.7.4 - Static code analyzer
* [ClangTidy](https://releases.llvm.org/15.0.0/tools/clang/tools/extra/docs/clang-tidy/index.html) 15.0.7 - C++ linter tool
* [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) 15.0.7 - Source code format tool
* [Valgrind](https://valgrind.org/info/) 3.19.0 - Runtime leak analyzer
* [Cmake](https://cmake.org/) 4.2.3 - Build files generator
* [Doxygen](doxygen.nl) 1.16.1 - Documentation generator
* [Conan](https://conan.io) - C/C++ Package manager
* [Benchmark](https://github.com/google/benchmark) - Google's benchmark library
* [Catch2](https://github.com/catchorg/Catch2) 3.13.0 - Unit testing

## Libraries

We use the Conan C/C++ Package Manager for installation of and management of libraries that are required by Unid.

These libraries are used by Unid software:

* [TQuic](https://tquic.net/) - UDP based transport layer protocol
* [Asio](https://think-async.com/Asio/AsioStandalone.html) 1.36.0 - Asynchronous network I/O
* [Quill](https://github.com/odygrd/quill) 11.1.0 - High performance logger

## Technical References
### Networking

* [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/)

### Consensus

The Paxos algorithm is used in distributed computing to achieve consensus
among multiple nodes (servers), ensuring they agree on a single, consistent
value or sequence of operations even if some nodes fail or network partitions
occur. It is fundamental for maintaining consistency in distributed databases,
distributed logs, and fault-tolerant systems.

* [The Part-Time Parlaiment](https://lamport.azurewebsites.net/pubs/lamport-paxos.pdf)
* [Paxos Made Simple](https://lamport.azurewebsites.net/pubs/paxos-simple.pdf)
* [Notes on Paxos Made Simple](https://medium.com/@adityashete009/notes-on-paxos-made-simple-1e693d60f51e)
* [Paxos in Pictures Video](https://www.youtube.com/watch?v=UUQ8xYWR4do&t=322s)
* [PaxosStore: High-availability Storage Made Practical in WeChat](https://www.vldb.org/pvldb/vol10/p1730-lin.pdf)

### Interprocess communication

The Unid client passes Unids to the application using shared memory and a
simple circular queue. The queue contains Unid values passed from the unid
client to the application.

Using shared memory for Inter-Process Communication (IPC) in C++ is highly
efficient because it avoids data copying between kernel and user space.
However, it requires careful management of synchronization using mechanisms
like mutexes or semaphores to prevent race conditions and data corruption.

Here are implementation of a class that handles a circular queue in C++:

* [Shared Memory with POSIX API](https://eric-lo.gitbook.io/synchronization/shared-memory)
* [IPC Shared Memory](https://www.geeksforgeeks.org/operating-systems/ipc-shared-memory/)
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

### Data

A data manager is responsible for providing access to data by the server
logic. The data manager is responsible for assuring data consistency ane
accuracy, and is also responsible for commiting the data when appropriate
to the hard drive log.

### Networking

Unid uses the UDP protocol for communication between Unid Servrs and Unid
Clients. A protocol is implemented that uses the communication channels for
both communication and failure detection.

While transiting the Internet, data must be secure, encrypted and tamper.
Modern AES encryption technology is used for these purposes.


### Consensus

A Paxos consensus algirithm is used to detact and adapt the Unid system for
failures, and the addition of new nodes to the system.

A heartbeat is used indicate life and server health. Heartbeat failure causes
a Paxos election to select a replacement leader server.

As server nodes are added or removed, the process requires a Paxos approval
to synchronize the logs.

### Interprocess Communicaiton

Unid Clients reside on a reliable network cluster, ideally on the machine hosts
the application api. Unids are provided to the application api through sockets,
or using a shared memory queue.

Applications access using either shared memory IPC for best performance, or
though IP sockets for applications that are based in a cluster with a single
Leader Unid Client. (All nodes in a cluster should be provisioned with a Unid
Client for failover operation.

### Operation Guarantees

* Unique integers - Unid guarantees to produce unique ascending ids
* MTBF
* MTTR
* MTTF
