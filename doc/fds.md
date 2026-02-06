# FHIRFirst Unid Server (FFUS) Functional Design Document

## Terminology

These terms are provided within the context of the FHIRFirst Unid Management
System (FFUMS).

* [Database (FFDB)](#database-id)
* [Master server (FFUMS)]($master-server-id)
* Slave server (FFUS)
* Standby server (FFUSS)
* Unid
* Web service (FFAPI)
* [Paxos algorithm simplifie](https://lamport.azurewebsites.net/pubs/paxos-simple.pdf)
* [Paxos Made Live](https://www.cs.utexas.edu/~lorenzo/corsi/cs380d/papers/paper2-1.pdf)
* [TLA+](?????)
* [Raft algorithm](https://raft.github.io/)
* [Raft thesis](https://raft.github.io/raft.pdf)


## Database {#database-id}

The FFDB is a distributed relational database made by PostgreSQL. The database
is distributed around the world providing access to FHIRFirst customers. The
database is designed to be fail-safe, with redundancy and high level of
availability. The database implements a schema completely compliant with the
FHIR release 6 published in. It is the intent of FHIRFirst to
maintain compatibility with future releases of FHIR.

FHIR (Fast Healthcare Interoperability Resourec) is a modern standard for
securely exchanging healthcaare information, using common we technologies
like APIs and data formats (JSON, XML) to allow different health systems
(EHRs, apps, etc.) to communicate seamlessly. The FFDB is designed specifically
to facilitate the implementation of FHIR application software.

## Master server {#master-server-id}

The master server generates and distributes the unids to the slave servers.
The slave servers request a range of unids when their supply runs low.
Their is only one master server in the distributed environment that is
responsible for providing unique ids for the use of the Web Services API.

Upon request from a slave server, the master server provides the slave
server with a number of nids by specifying the starting nid, and the number
of nids allocated. The information is passed via the internet using TCP.

The number of nids provided are determined by the historical record calculated
by the master server. The intention is to provide a number of unids that will
be consumed within the next hour.

The data set provided through a socket connection is simply a 64 bit integer
that indicates the first unid, and the number of unids reserved for the slave
server (also a 64 bit value).

The slave server distributes an unid and then increments the variable for the
next request. This repeats until all of the unids have been distributed.

The slave server requests an additional set of unids when the supply hits a
particular level so as to provide uninterrupted service of supplying unids to
the web service (api).

## Slave servers {#slave-servers-id}

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

## Unid {#unid_id}

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

## Unid range request {#unid-range-request-id}

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

## Web service {#web-servic-id}

The web service provides the API functionality as defined by the api
specifiation (swagger). The API provides for Creation, Retrieval, Updating,
Deleting of records in the database. Othare functionality provided by the
API is the listing and searching for records for use by the application
program.

See the docs.
