# FHIRFirst Unid Server (FFUS) System Requirements Specification

## Overview

FFUS generates and shares unique primary keys for a distributed database system.
These unid is somewhat similar to a uuid in its usage - it provides a unique
identifier used as a primary key for a record (row in a table) across all tables in
the database.

Why is this important? In a database that is distributed within a data center
or around thw world, the generation of Primary Keys used to identify a
record must be distinct. This allows the consolidation of tables into a
single master database that saves the entire database record set for
backup and provisioning and replication purposes.

Databases generally support primary keys that are integers and that can
be automatially incremented by the database. This is very common for
smaller database applications that can utilize a single or small cluster
of database instances. The coordination is usually simplified by havinge
one of the instances perform read/write services  while the others
provide read only services.

For larger databases that run separate instances in remote locations,
another mechanism is used for generating private keys for records within a
table virtually eliminates conflict in distributed systems. These use a uuid,
a 128-bit randomly generated value. The potential for conflict between
remote database instances within a table are almost unlikely given
the size of the uuid and the mechanism with which they are generated.

Unfortunately, a UUID is stored in a non-native data type because of its
size (128 bits). Also, a UUID is random, and not sequential, so its use
within a database index is not optimal. Lastly, the expression of a UUID
is awkward and long, as it is encoded to be a 31-digit hexidecimal string
that can be transmitted using HTTP protocols.

### What is a unid?
A unid is a 64-bit integer that is used as a primary key in a database
table. The unid is provided to the FFWS as it performs a record creation
as the primary key. A unid is a natively supported data type, and is
guaranteed to be unique as it is provided through a "Central Authority"

### How are they better than UUIDs?
They are smaller and can be stored in a natively supported data type in both
the application and the database. They are sequential that makes indexing
efficient within the database. They guarantee uniqueness as they are produced
by a single source of truth, the "Central Authority". Lastly, they are
generated at the application level and not the database level. This allows
the distribution of unids to replicated database servers that are accessed
due to geographic proximity.

### I'm confused, is the unid_server a client, or a server, or both?
Both is the correct answer.

From the perspective of the FFWS, the unid_server
is a server.

It delivers unids, while the FFWD consumes them. The FFWS gets an unid
from a FIFO in shared memory that is created by the unid_server. The FFWS essential
dequeues a unid from the circular queue from its head pointer. As the unid_server
enques additional unid values at the tail. This provides an unending stream of
unids for the use in creation of new database table rows.

From the perspective of associated unid_servers located in a different location,
the unid_server acts as a client to one and only one of its associates, the
master source. The unid_server that is selected to be the master generates and
distributs unids to other client unid_servers over the internet using Sockets.

In this scenario, a client unid_server monitors the number of unids that have
been provided by the master unid_server. As the clients unids are utilized, it
makes a request for additional unids. The master unid_server sends as a response,
the next available unid along with a number of dedicated unids provided. As
the client distributes the unids, it increments the last unid by 1 for each
distribution until the supply is and then another unid request is made.

The master unid_server monitors the requests and timing of the requests to
calculate the requirements of the client in order to determine how many unids
to provide to a given client.

### What happens if there is a failure the prevents the master from supplying unids to the client?

The master has a designated backup unid_server that takes over the role of
being the master. The designated backup receives the next available unid whenever
a unid is distributed by the master server. 

## Objectives

1. Performant
1. Reliable
1. Simple
1. Safe

## Performance

In order to meet the performance requirements of the FHIRFirst database, the
implementation must be extremely fast.

* Implemented with c++ language and support for networking with POCO
* Use shared memory for passing unads to the FHIRFirst Web Service (FFWS)
* Uses sockets to interface with remote servers to distribute unads

## Reliability

## Simplicity

A unad_server is co-located on each server that hosts the FHIRFirst API. The
API implements an abstracted interface to the database using standard HTTP
methods to perform CRUD operations. We call the software that implements the
interface the FHIRFirst Web Service (FFWS).

As an application requests the creation of a new database record, the FFWS
software interfaces with the unad_server to get a unique unad that is used
as the primary key of the record. This unad is guaranteed to be unique not
only in the table, but also across all primary keys in the entire database.

Using replication techniques, the regional database servers are synchronized.

## Safty


